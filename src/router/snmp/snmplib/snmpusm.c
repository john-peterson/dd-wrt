/*
 * snmpusm.c
 *
 * Routines to manipulate a information about a "user" as
 * defined by the SNMP-USER-BASED-SM-MIB MIB.
 *
 * All functions usm_set_usmStateReference_*() return 0 on success, -1
 * otherwise.
 *
 * !! Tab stops set to 4 in some parts of this file. !!
 *    (Designated on a per function.)
 */

#include <net-snmp/net-snmp-config.h>

#include <sys/types.h>
#if HAVE_WINSOCK_H
#include <winsock.h>
#endif
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if TIME_WITH_SYS_TIME
# ifdef WIN32
#  include <sys/timeb.h>
# else
#  include <sys/time.h>
# endif
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#if HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#if HAVE_DMALLOC_H
#include <dmalloc.h>
#endif

#include <net-snmp/types.h>
#include <net-snmp/output_api.h>
#include <net-snmp/config_api.h>
#include <net-snmp/utilities.h>

#include <net-snmp/library/asn1.h>
#include <net-snmp/library/snmp_api.h>
#include <net-snmp/library/callback.h>
#include <net-snmp/library/keytools.h>
#include <net-snmp/library/snmpv3.h>
#include <net-snmp/library/lcd_time.h>
#include <net-snmp/library/scapi.h>
#include <net-snmp/library/callback.h>
#include <net-snmp/library/snmp_secmod.h>
#include <net-snmp/library/snmpusm.h>

oid             usmNoAuthProtocol[10] = { 1, 3, 6, 1, 6, 3, 10, 1, 1, 1 };
oid             usmHMACMD5AuthProtocol[10] =
    { 1, 3, 6, 1, 6, 3, 10, 1, 1, 2 };
oid             usmHMACSHA1AuthProtocol[10] =
    { 1, 3, 6, 1, 6, 3, 10, 1, 1, 3 };
oid             usmNoPrivProtocol[10] = { 1, 3, 6, 1, 6, 3, 10, 1, 2, 1 };
oid             usmDESPrivProtocol[10] = { 1, 3, 6, 1, 6, 3, 10, 1, 2, 2 };
oid             usmAES128PrivProtocol[10] = { 1, 3, 6, 1, 4, 1, 8072, 876,876,128 };
oid             usmAES192PrivProtocol[10] = { 1, 3, 6, 1, 4, 1, 8072, 876,876,192 };
oid             usmAES256PrivProtocol[10] = { 1, 3, 6, 1, 4, 1, 8072, 876,876,256 };

static u_int    dummy_etime, dummy_eboot;       /* For ISENGINEKNOWN(). */

/*
 * Globals.
 */
static u_int    salt_integer;
#ifdef HAVE_AES
static u_int    salt_integer64_1, salt_integer64_2;
#endif
        /*
         * 1/2 of seed for the salt.   Cf. RFC2274, Sect 8.1.1.1.
         */

static struct usmUser *noNameUser = NULL;
/*
 * Local storage (LCD) of the default user list.
 */
static struct usmUser *userList = NULL;

/*
 * Prototypes
 */
int
                usm_check_secLevel_vs_protocols(int level,
                                                const oid * authProtocol,
                                                u_int authProtocolLen,
                                                const oid * privProtocol,
                                                u_int privProtocolLen);
int
                usm_calc_offsets(size_t globalDataLen,
                                 int secLevel, size_t secEngineIDLen,
                                 size_t secNameLen, size_t scopedPduLen,
                                 u_long engineboots, long engine_time,
                                 size_t * theTotalLength,
                                 size_t * authParamsOffset,
                                 size_t * privParamsOffset,
                                 size_t * dataOffset, size_t * datalen,
                                 size_t * msgAuthParmLen,
                                 size_t * msgPrivParmLen, size_t * otstlen,
                                 size_t * seq_len, size_t * msgSecParmLen);
/*
 * Set a given field of the secStateRef.
 *
 * Allocate <len> bytes for type <type> pointed to by ref-><field>.
 * Then copy in <item> and record its length in ref-><field_len>.
 *
 * Return 0 on success, -1 otherwise.
 */
#define MAKE_ENTRY( type, item, len, field, field_len )			\
{									\
	if (ref == NULL)						\
		return -1;						\
	if (ref->field != NULL)	{					\
		SNMP_ZERO(ref->field, ref->field_len);			\
		SNMP_FREE(ref->field);					\
	}								\
	ref->field_len = 0;						\
        if (len == 0 || item == NULL) {					\
		return 0;						\
	}					 			\
	if ((ref->field = (type*) malloc (len * sizeof(type))) == NULL)	\
	{								\
		return -1;						\
	}								\
									\
	memcpy (ref->field, item, len * sizeof(type));			\
	ref->field_len = len;						\
									\
	return 0;							\
}


struct usmStateReference *
usm_malloc_usmStateReference(void)
{
    struct usmStateReference *retval = (struct usmStateReference *)
        calloc(1, sizeof(struct usmStateReference));

    return retval;
}                               /* end usm_malloc_usmStateReference() */


void
usm_free_usmStateReference(void *old)
{
    struct usmStateReference *old_ref = (struct usmStateReference *) old;

    if (old_ref) {

        SNMP_FREE(old_ref->usr_name);
        SNMP_FREE(old_ref->usr_engine_id);
        SNMP_FREE(old_ref->usr_auth_protocol);
        SNMP_FREE(old_ref->usr_priv_protocol);

        if (old_ref->usr_auth_key) {
            SNMP_ZERO(old_ref->usr_auth_key, old_ref->usr_auth_key_length);
            SNMP_FREE(old_ref->usr_auth_key);
        }
        if (old_ref->usr_priv_key) {
            SNMP_ZERO(old_ref->usr_priv_key, old_ref->usr_priv_key_length);
            SNMP_FREE(old_ref->usr_priv_key);
        }

        SNMP_ZERO(old_ref, sizeof(*old_ref));
        SNMP_FREE(old_ref);

    }

}                               /* end usm_free_usmStateReference() */

struct usmUser *
usm_get_userList(void)
{
    return userList;
}

int
usm_set_usmStateReference_name(struct usmStateReference *ref,
                               char *name, size_t name_len)
{
    MAKE_ENTRY(char, name, name_len, usr_name, usr_name_length);
}

int
usm_set_usmStateReference_engine_id(struct usmStateReference *ref,
                                    u_char * engine_id,
                                    size_t engine_id_len)
{
    MAKE_ENTRY(u_char, engine_id, engine_id_len,
               usr_engine_id, usr_engine_id_length);
}

int
usm_set_usmStateReference_auth_protocol(struct usmStateReference *ref,
                                        oid * auth_protocol,
                                        size_t auth_protocol_len)
{
    MAKE_ENTRY(oid, auth_protocol, auth_protocol_len,
               usr_auth_protocol, usr_auth_protocol_length);
}

int
usm_set_usmStateReference_auth_key(struct usmStateReference *ref,
                                   u_char * auth_key, size_t auth_key_len)
{
    MAKE_ENTRY(u_char, auth_key, auth_key_len,
               usr_auth_key, usr_auth_key_length);
}

int
usm_set_usmStateReference_priv_protocol(struct usmStateReference *ref,
                                        oid * priv_protocol,
                                        size_t priv_protocol_len)
{
    MAKE_ENTRY(oid, priv_protocol, priv_protocol_len,
               usr_priv_protocol, usr_priv_protocol_length);
}

int
usm_set_usmStateReference_priv_key(struct usmStateReference *ref,
                                   u_char * priv_key, size_t priv_key_len)
{
    MAKE_ENTRY(u_char, priv_key, priv_key_len,
               usr_priv_key, usr_priv_key_length);
}

int
usm_set_usmStateReference_sec_level(struct usmStateReference *ref,
                                    int sec_level)
{
    if (ref == NULL)
        return -1;
    ref->usr_sec_level = sec_level;
    return 0;
}



#ifdef SNMP_TESTING_CODE
/*******************************************************************-o-******
 * emergency_print
 *
 * Parameters:
 *	*field
 *	 length
 *      
 *	This is a print routine that is solely included so that it can be
 *	used in gdb.  Don't use it as a function, it will be pulled before
 *	a real release of the code.
 *
 *	tab stop 4
 *
 *	XXX fflush() only works on FreeBSD; core dumps on Sun OS's
 */
void
emergency_print(u_char * field, u_int length)
{
    int             iindex;
    int             start = 0;
    int             stop = 25;

    while (start < stop) {
        for (iindex = start; iindex < stop; iindex++)
            printf("%02X ", field[iindex]);

        printf("\n");
        start = stop;
        stop = stop + 25 < length ? stop + 25 : length;
    }
    fflush(0);

}                               /* end emergency_print() */
#endif                          /* SNMP_TESTING_CODE */


/*******************************************************************-o-******
 * asn_predict_int_length
 *
 * Parameters:
 *	type	(UNUSED)
 *	number
 *	len
 *      
 * Returns:
 *	Number of bytes necessary to store the ASN.1 encoded value of 'number'.
 *
 *
 *	This gives the number of bytes that the ASN.1 encoder (in asn1.c) will
 *	use to encode a particular integer value.
 *
 *	Returns the length of the integer -- NOT THE HEADER!
 *
 *	Do this the same way as asn_build_int()...
 */
int
asn_predict_int_length(int type, long number, size_t len)
{
    register u_long mask;


    if (len != sizeof(long))
        return -1;

    mask = ((u_long) 0x1FF) << ((8 * (sizeof(long) - 1)) - 1);
    /*
     * mask is 0xFF800000 on a big-endian machine 
     */

    while ((((number & mask) == 0) || ((number & mask) == mask))
           && len > 1) {
        len--;
        number <<= 8;
    }

    return len;

}                               /* end asn_predict_length() */




/*******************************************************************-o-******
 * asn_predict_length
 *
 * Parameters:
 *	 type
 *	*ptr
 *	 u_char_len
 *      
 * Returns:
 *	Length in bytes:	1 + <n> + <u_char_len>, where
 *
 *		1		For the ASN.1 type.
 *		<n>		# of bytes to store length of data.
 *		<u_char_len>	Length of data associated with ASN.1 type.
 *
 *	This gives the number of bytes that the ASN.1 encoder (in asn1.c) will
 *	use to encode a particular integer value.  This is as broken as the
 *	currently used encoder.
 *
 * XXX	How is <n> chosen, exactly??
 */
int
asn_predict_length(int type, u_char * ptr, size_t u_char_len)
{

    if (type & ASN_SEQUENCE)
        return 1 + 3 + u_char_len;

    if (type & ASN_INTEGER) {
        u_long          value;
        memcpy(&value, ptr, u_char_len);
        u_char_len = asn_predict_int_length(type, value, u_char_len);
    }

    if (u_char_len < 0x80)
        return 1 + 1 + u_char_len;
    else if (u_char_len < 0xFF)
        return 1 + 2 + u_char_len;
    else
        return 1 + 3 + u_char_len;

}                               /* end asn_predict_length() */




/*******************************************************************-o-******
 * usm_calc_offsets
 *
 * Parameters:
 *	(See list below...)
 *      
 * Returns:
 *	0	On success,
 *	-1	Otherwise.
 *
 *
 *	This routine calculates the offsets into an outgoing message buffer
 *	for the necessary values.  The outgoing buffer will generically
 *	look like this:
 *
 *	SNMPv3 Message
 *	SEQ len[11]
 *		INT len version
 *	Header
 *		SEQ len
 *			INT len MsgID
 *			INT len msgMaxSize
 *			OST len msgFlags (OST = OCTET STRING)
 *			INT len msgSecurityModel
 *	MsgSecurityParameters
 *		[1] OST len[2]
 *			SEQ len[3]
 *				OST len msgAuthoritativeEngineID
 *				INT len msgAuthoritativeEngineBoots
 *				INT len msgAuthoritativeEngineTime
 *				OST len msgUserName
 *				OST len[4] [5] msgAuthenticationParameters
 *				OST len[6] [7] msgPrivacyParameters
 *	MsgData
 *		[8] OST len[9] [10] encryptedPDU
 *		or
 *		[8,10] SEQUENCE len[9] scopedPDU
 *	[12]
 *
 *	The bracketed points will be needed to be identified ([x] is an index
 *	value, len[x] means a length value).  Here is a semantic guide to them:
 *
 *	[1] = globalDataLen (input)
 *	[2] = otstlen
 *	[3] = seq_len
 *	[4] = msgAuthParmLen (may be 0 or 12)
 *	[5] = authParamsOffset
 *	[6] = msgPrivParmLen (may be 0 or 8)
 *	[7] = privParamsOffset
 *	[8] = globalDataLen + msgSecParmLen
 *	[9] = datalen
 *	[10] = dataOffset
 *	[11] = theTotalLength - the length of the header itself
 *	[12] = theTotalLength
 */
int
usm_calc_offsets(size_t globalDataLen,  /* SNMPv3Message + HeaderData */
                 int secLevel, size_t secEngineIDLen, size_t secNameLen, size_t scopedPduLen,   /* An BER encoded sequence. */
                 u_long engineboots,    /* XXX (asn1.c works in long, not int.) */
                 long engine_time,      /* XXX (asn1.c works in long, not int.) */
                 size_t * theTotalLength,       /* globalDataLen + msgSecurityP. + msgData */
                 size_t * authParamsOffset,     /* Distance to auth bytes.                 */
                 size_t * privParamsOffset,     /* Distance to priv bytes.                 */
                 size_t * dataOffset,   /* Distance to scopedPdu SEQ  -or-  the
                                         *   crypted (data) portion of msgData.    */
                 size_t * datalen,      /* Size of msgData OCTET STRING encoding.  */
                 size_t * msgAuthParmLen,       /* Size of msgAuthenticationParameters.    */
                 size_t * msgPrivParmLen,       /* Size of msgPrivacyParameters.           */
                 size_t * otstlen,      /* Size of msgSecurityP. O.S. encoding.    */
                 size_t * seq_len,      /* Size of msgSecurityP. SEQ data.         */
                 size_t * msgSecParmLen)
{                               /* Size of msgSecurityP. SEQ.              */
    int             engIDlen,   /* Sizes of OCTET STRING and SEQ encodings */
                    engBtlen,   /*   for fields within                     */
                    engTmlen,   /*   msgSecurityParameters portion of      */
                    namelen,    /*   SNMPv3Message.                        */
                    authlen, privlen;

    /*
     * If doing authentication, msgAuthParmLen = 12 else msgAuthParmLen = 0.
     * If doing encryption,     msgPrivParmLen = 8  else msgPrivParmLen = 0.
     */
    *msgAuthParmLen = (secLevel == SNMP_SEC_LEVEL_AUTHNOPRIV
                       || secLevel == SNMP_SEC_LEVEL_AUTHPRIV) ? 12 : 0;

    *msgPrivParmLen = (secLevel == SNMP_SEC_LEVEL_AUTHPRIV) ? 8 : 0;


    /*
     * Calculate lengths.
     */
    if ((engIDlen = asn_predict_length(ASN_OCTET_STR,
                                       0, secEngineIDLen)) == -1) {
        return -1;
    }

    if ((engBtlen = asn_predict_length(ASN_INTEGER,
                                       (u_char *) & engineboots,
                                       sizeof(long))) == -1) {
        return -1;
    }

    if ((engTmlen = asn_predict_length(ASN_INTEGER,
                                       (u_char *) & engine_time,
                                       sizeof(long))) == -1) {
        return -1;
    }

    if ((namelen = asn_predict_length(ASN_OCTET_STR, 0, secNameLen)) == -1) {
        return -1;
    }

    if ((authlen = asn_predict_length(ASN_OCTET_STR,
                                      0, *msgAuthParmLen)) == -1) {
        return -1;
    }

    if ((privlen = asn_predict_length(ASN_OCTET_STR,
                                      0, *msgPrivParmLen)) == -1) {
        return -1;
    }

    *seq_len =
        engIDlen + engBtlen + engTmlen + namelen + authlen + privlen;

    if ((*otstlen = asn_predict_length(ASN_SEQUENCE, 0, *seq_len)) == -1) {
        return -1;
    }

    if ((*msgSecParmLen = asn_predict_length(ASN_OCTET_STR,
                                             0, *otstlen)) == -1) {
        return -1;
    }

    *authParamsOffset = globalDataLen + +(*msgSecParmLen - *seq_len)
        + engIDlen + engBtlen + engTmlen + namelen
        + (authlen - *msgAuthParmLen);

    *privParamsOffset = *authParamsOffset + *msgAuthParmLen
        + (privlen - *msgPrivParmLen);


    /*
     * Compute the size of the plaintext.  Round up to account for cipher
     * block size, if necessary.
     *
     * XXX  This is hardwired for 1DES... If scopedPduLen is already
     *      a multiple of 8, then *add* 8 more; otherwise, round up
     *      to the next multiple of 8.
     *
     * FIX  Calculation of encrypted portion of msgData and consequent
     *      setting and sanity checking of theTotalLength, et al. should
     *      occur *after* encryption has taken place.
     */
    if (secLevel == SNMP_SEC_LEVEL_AUTHPRIV) {
        scopedPduLen = ROUNDUP8(scopedPduLen);

        if ((*datalen =
             asn_predict_length(ASN_OCTET_STR, 0, scopedPduLen)) == -1) {
            return -1;
        }
    } else {
        *datalen = scopedPduLen;
    }

    *dataOffset = globalDataLen + *msgSecParmLen +
        (*datalen - scopedPduLen);
    *theTotalLength = globalDataLen + *msgSecParmLen + *datalen;

    return 0;

}                               /* end usm_calc_offsets() */





/*******************************************************************-o-******
 * usm_set_salt
 *
 * Parameters:
 *	*iv		  (O)   Buffer to contain IV.
 *	*iv_length	  (O)   Length of iv.
 *	*priv_salt	  (I)   Salt portion of private key.
 *	 priv_salt_length (I)   Length of priv_salt.
 *	*msgSalt	  (I/O) Pointer salt portion of outgoing msg buffer.
 *      
 * Returns:
 *	0	On success,
 *	-1	Otherwise.
 *
 *	Determine the initialization vector for the DES-CBC encryption.
 *	(Cf. RFC 2274, 8.1.1.1.)
 *
 *	iv is defined as the concatenation of engineBoots and the
 *		salt integer.
 *	The salt integer is incremented.
 *	The resulting salt is copied into the msgSalt buffer.
 *	The result of the concatenation is then XORed with the salt
 *		portion of the private key (last 8 bytes).
 *	The IV result is returned individually for further use.
 */
int
usm_set_salt(u_char * iv,
             size_t * iv_length,
             u_char * priv_salt, size_t priv_salt_length, u_char * msgSalt)
{
    size_t          propersize_salt = BYTESIZE(USM_DES_SALT_LENGTH);
    int             net_boots;
    int             net_salt_int;
    /*
     * net_* should be encoded in network byte order.  XXX  Why?
     */
    int             iindex;


    /*
     * Sanity check.
     */
    if (!iv || !iv_length || !priv_salt || (*iv_length != propersize_salt)
        || (priv_salt_length < propersize_salt)) {
        return -1;
    }


    net_boots = htonl(snmpv3_local_snmpEngineBoots());
    net_salt_int = htonl(salt_integer);

    salt_integer += 1;

    memcpy(iv, &net_boots, propersize_salt / 2);
    memcpy(iv + (propersize_salt / 2), &net_salt_int, propersize_salt / 2);

    if (msgSalt)
        memcpy(msgSalt, iv, propersize_salt);


    /*
     * Turn the salt into an IV: XOR <boots, salt_int> with salt
     * portion of priv_key.
     */
    for (iindex = 0; iindex < (int) propersize_salt; iindex++)
        iv[iindex] ^= priv_salt[iindex];


    return 0;

}                               /* end usm_set_salt() */

#ifdef HAVE_AES
/*******************************************************************-o-******
 * usm_set_aes_iv
 *
 * Parameters:
 *	*iv		  (O)   Buffer to contain IV.
 *	*iv_length	  (O)   Length of iv.
 *      net_boots         (I)   the network byte order of the authEng boots val
 *      net_time         (I)   the network byte order of the authEng time val
 *      *salt             (O)   A buffer for the outgoing salt (= 8 bytes of iv)
 *      
 * Returns:
 *	0	On success,
 *	-1	Otherwise.
 *
 *	Determine the initialization vector for AES encryption.
 *	(draft-blumenthal-aes-usm-03.txt, 3.1.2.2)
 *
 *	iv is defined as the concatenation of engineBoots, engineTime
  	and a 64 bit salt-integer.
 *	The 64 bit salt integer is incremented.
 *	The resulting salt is copied into the salt buffer.
 *	The IV result is returned individually for further use.
 */
int
usm_set_aes_iv(u_char * iv,
               size_t * iv_length,
               u_int net_boots,
               u_int net_time,
               u_char * salt)
{
    /*
     * net_* should be encoded in network byte order.
     */
    int             net_salt_int1, net_salt_int2;
#define PROPER_AES_IV_SIZE 64

    /*
     * Sanity check.
     */
    if (!iv || !iv_length) {
        return -1;
    }

    net_salt_int1 = htonl(salt_integer64_1);
    net_salt_int2 = htonl(salt_integer64_2);

    if ((salt_integer64_2 += 1) == 0)
        salt_integer64_2 += 1;
    
    /* XXX: warning: hard coded proper lengths */
    memcpy(iv, &net_boots, 4);
    memcpy(iv+4, &net_time, 4);
    memcpy(iv+8, &net_salt_int1, 4);
    memcpy(iv+12, &net_salt_int2, 4);

    memcpy(salt, iv+8, 8); /* only copy the needed portion */
    return 0;
}                               /* end usm_set_salt() */
#endif /* HAVE_AES */

int
usm_secmod_generate_out_msg(struct snmp_secmod_outgoing_params *parms)
{
    if (!parms)
        return SNMPERR_GENERR;

    return usm_generate_out_msg(parms->msgProcModel,
                                parms->globalData, parms->globalDataLen,
                                parms->maxMsgSize, parms->secModel,
                                parms->secEngineID, parms->secEngineIDLen,
                                parms->secName, parms->secNameLen,
                                parms->secLevel,
                                parms->scopedPdu, parms->scopedPduLen,
                                parms->secStateRef,
                                parms->secParams, parms->secParamsLen,
                                parms->wholeMsg, parms->wholeMsgLen);
}

/*******************************************************************-o-******
 * usm_generate_out_msg
 *
 * Parameters:
 *	(See list below...)
 *      
 * Returns:
 *	SNMPERR_SUCCESS			On success.
 *	SNMPERR_USM_AUTHENTICATIONFAILURE
 *	SNMPERR_USM_ENCRYPTIONERROR
 *	SNMPERR_USM_GENERICERROR
 *	SNMPERR_USM_UNKNOWNSECURITYNAME
 *	SNMPERR_USM_GENERICERROR
 *	SNMPERR_USM_UNSUPPORTEDSECURITYLEVEL
 *	
 *
 * Generates an outgoing message.
 *
 * XXX	Beware of misnomers!
 */
int
usm_generate_out_msg(int msgProcModel,  /* (UNUSED) */
                     u_char * globalData,       /* IN */
                     /*
                      * Pointer to msg header data will point to the beginning
                      * * of the entire packet buffer to be transmitted on wire,
                      * * memory will be contiguous with secParams, typically
                      * * this pointer will be passed back as beginning of
                      * * wholeMsg below.  asn seq. length is updated w/ new length.
                      * *
                      * * While this points to a buffer that should be big enough
                      * * for the whole message, only the first two parts
                      * * of the message are completed, namely SNMPv3Message and
                      * * HeaderData.  globalDataLen (next parameter) represents
                      * * the length of these two completed parts.
                      */
                     size_t globalDataLen,      /* IN - Length of msg header data.      */
                     int maxMsgSize,    /* (UNUSED) */
                     int secModel,      /* (UNUSED) */
                     u_char * secEngineID,      /* IN - Pointer snmpEngineID.           */
                     size_t secEngineIDLen,     /* IN - SnmpEngineID length.            */
                     char *secName,     /* IN - Pointer to securityName.        */
                     size_t secNameLen, /* IN - SecurityName length.            */
                     int secLevel,      /* IN - AuthNoPriv, authPriv etc.       */
                     u_char * scopedPdu,        /* IN */
                     /*
                      * Pointer to scopedPdu will be encrypted by USM if needed
                      * * and written to packet buffer immediately following
                      * * securityParameters, entire msg will be authenticated by
                      * * USM if needed.
                      */
                     size_t scopedPduLen,       /* IN - scopedPdu length. */
                     void *secStateRef, /* IN */
                     /*
                      * secStateRef, pointer to cached info provided only for
                      * * Response, otherwise NULL.
                      */
                     u_char * secParams,        /* OUT */
                     /*
                      * BER encoded securityParameters pointer to offset within
                      * * packet buffer where secParams should be written, the
                      * * entire BER encoded OCTET STRING (including header) is
                      * * written here by USM secParams = globalData +
                      * * globalDataLen.
                      */
                     size_t * secParamsLen,     /* IN/OUT - Len available, len returned. */
                     u_char ** wholeMsg,        /* OUT */
                     /*
                      * Complete authenticated/encrypted message - typically
                      * * the pointer to start of packet buffer provided in
                      * * globalData is returned here, could also be a separate
                      * * buffer.
                      */
                     size_t * wholeMsgLen)
{                               /* IN/OUT - Len available, len returned. */
    size_t          otstlen;
    size_t          seq_len;
    size_t          msgAuthParmLen;
    size_t          msgPrivParmLen;
    size_t          msgSecParmLen;
    size_t          authParamsOffset;
    size_t          privParamsOffset;
    size_t          datalen;
    size_t          dataOffset;
    size_t          theTotalLength;

    u_char         *ptr;
    size_t          ptr_len;
    size_t          remaining;
    size_t          offSet;
    u_int           boots_uint;
    u_int           time_uint;
    long            boots_long;
    long            time_long;

    /*
     * Indirection because secStateRef values override parameters.
     * 
     * None of these are to be free'd - they are either pointing to
     * what's in the secStateRef or to something either in the
     * actual prarmeter list or the user list.
     */

    char           *theName = NULL;
    u_int           theNameLength = 0;
    u_char         *theEngineID = NULL;
    u_int           theEngineIDLength = 0;
    u_char         *theAuthKey = NULL;
    u_int           theAuthKeyLength = 0;
    const oid      *theAuthProtocol = NULL;
    u_int           theAuthProtocolLength = 0;
    u_char         *thePrivKey = NULL;
    u_int           thePrivKeyLength = 0;
    const oid      *thePrivProtocol = NULL;
    u_int           thePrivProtocolLength = 0;
    int             theSecLevel = 0;    /* No defined const for bad
                                         * value (other then err).
                                         */

    DEBUGMSGTL(("usm", "USM processing has begun.\n"));

    if (secStateRef != NULL) {
        /*
         * To hush the compiler for now.  XXX 
         */
        struct usmStateReference *ref
            = (struct usmStateReference *) secStateRef;

        theName = ref->usr_name;
        theNameLength = ref->usr_name_length;
        theEngineID = ref->usr_engine_id;
        theEngineIDLength = ref->usr_engine_id_length;

        if (!theEngineIDLength) {
            theEngineID = secEngineID;
            theEngineIDLength = secEngineIDLen;
        }

        theAuthProtocol = ref->usr_auth_protocol;
        theAuthProtocolLength = ref->usr_auth_protocol_length;
        theAuthKey = ref->usr_auth_key;
        theAuthKeyLength = ref->usr_auth_key_length;
        thePrivProtocol = ref->usr_priv_protocol;
        thePrivProtocolLength = ref->usr_priv_protocol_length;
        thePrivKey = ref->usr_priv_key;
        thePrivKeyLength = ref->usr_priv_key_length;
        theSecLevel = ref->usr_sec_level;
    }

    /*
     * Identify the user record.
     */
    else {
        struct usmUser *user;

        /*
         * we do allow an unknown user name for
         * unauthenticated requests. 
         */
        if ((user = usm_get_user(secEngineID, secEngineIDLen, secName))
            == NULL && secLevel != SNMP_SEC_LEVEL_NOAUTH) {
            DEBUGMSGTL(("usm", "Unknown User(%s)\n", secName));
            usm_free_usmStateReference(secStateRef);
            return SNMPERR_USM_UNKNOWNSECURITYNAME;
        }

        theName = secName;
        theNameLength = secNameLen;
        theEngineID = secEngineID;
        theSecLevel = secLevel;
        theEngineIDLength = secEngineIDLen;
        if (user) {
            theAuthProtocol = user->authProtocol;
            theAuthProtocolLength = user->authProtocolLen;
            theAuthKey = user->authKey;
            theAuthKeyLength = user->authKeyLen;
            thePrivProtocol = user->privProtocol;
            thePrivProtocolLength = user->privProtocolLen;
            thePrivKey = user->privKey;
            thePrivKeyLength = user->privKeyLen;
        } else {
            /*
             * unknown users can not do authentication (obviously) 
             */
            theAuthProtocol = usmNoAuthProtocol;
            theAuthProtocolLength =
                sizeof(usmNoAuthProtocol) / sizeof(oid);
            theAuthKey = NULL;
            theAuthKeyLength = 0;
            thePrivProtocol = usmNoPrivProtocol;
            thePrivProtocolLength =
                sizeof(usmNoPrivProtocol) / sizeof(oid);
            thePrivKey = NULL;
            thePrivKeyLength = 0;
        }
    }                           /* endif -- secStateRef==NULL */


    /*
     * From here to the end of the function, avoid reference to
     * secName, secEngineID, secLevel, and associated lengths.
     */


    /*
     * Check to see if the user can use the requested sec services.
     */
    if (usm_check_secLevel_vs_protocols(theSecLevel,
                                        theAuthProtocol,
                                        theAuthProtocolLength,
                                        thePrivProtocol,
                                        thePrivProtocolLength) == 1) {
        DEBUGMSGTL(("usm", "Unsupported Security Level (%d)\n",
                    theSecLevel));
        usm_free_usmStateReference(secStateRef);
        return SNMPERR_USM_UNSUPPORTEDSECURITYLEVEL;
    }


    /*
     * Retrieve the engine information.
     *
     * XXX  No error is declared in the EoP when sending messages to
     *      unknown engines, processing continues w/ boots/time == (0,0).
     */
    if (get_enginetime(theEngineID, theEngineIDLength,
                       &boots_uint, &time_uint, FALSE) == -1) {
        DEBUGMSGTL(("usm", "%s\n", "Failed to find engine data."));
    }

    boots_long = boots_uint;
    time_long = time_uint;


    /*
     * Set up the Offsets.
     */
    if (usm_calc_offsets(globalDataLen, theSecLevel, theEngineIDLength,
                         theNameLength, scopedPduLen, boots_long,
                         time_long, &theTotalLength, &authParamsOffset,
                         &privParamsOffset, &dataOffset, &datalen,
                         &msgAuthParmLen, &msgPrivParmLen, &otstlen,
                         &seq_len, &msgSecParmLen) == -1) {
        DEBUGMSGTL(("usm", "Failed calculating offsets.\n"));
        usm_free_usmStateReference(secStateRef);
        return SNMPERR_USM_GENERICERROR;
    }

    /*
     * So, we have the offsets for the three parts that need to be
     * determined, and an overall length.  Now we need to make
     * sure all of this would fit in the outgoing buffer, and
     * whether or not we need to make a new buffer, etc.
     */


    /*
     * Set wholeMsg as a pointer to globalData.  Sanity check for
     * the proper size.
     * 
     * Mark workspace in the message with bytes of all 1's to make it
     * easier to find mistakes in raw message dumps.
     */
    ptr = *wholeMsg = globalData;
    if (theTotalLength > *wholeMsgLen) {
        DEBUGMSGTL(("usm", "Message won't fit in buffer.\n"));
        usm_free_usmStateReference(secStateRef);
        return SNMPERR_USM_GENERICERROR;
    }

    ptr_len = *wholeMsgLen = theTotalLength;

#ifdef SNMP_TESTING_CODE
    memset(&ptr[globalDataLen], 0xFF, theTotalLength - globalDataLen);
#endif                          /* SNMP_TESTING_CODE */

    /*
     * Do the encryption.
     */
    if (theSecLevel == SNMP_SEC_LEVEL_AUTHPRIV) {
        size_t          encrypted_length = theTotalLength - dataOffset;
        size_t          salt_length = BYTESIZE(USM_MAX_SALT_LENGTH);
        u_char          salt[BYTESIZE(USM_MAX_SALT_LENGTH)];

        /*
         * XXX  Hardwired to seek into a 1DES private key!
         */
#ifdef HAVE_AES
        if (ISTRANSFORM(thePrivProtocol, AES128Priv) &&
            ISTRANSFORM(thePrivProtocol, AES192Priv) &&
            ISTRANSFORM(thePrivProtocol, AES256Priv)) {
            if (!thePrivKey ||
                usm_set_aes_iv(salt, &salt_length,
                               htonl(boots_uint), htonl(time_uint),
                               &ptr[privParamsOffset]) == -1) {
                DEBUGMSGTL(("usm", "Can't set AES iv.\n"));
                usm_free_usmStateReference(secStateRef);
                return SNMPERR_USM_GENERICERROR;
            }
        } else if (ISTRANSFORM(thePrivProtocol, DESPriv)) {
#endif
        if (!thePrivKey ||
            (usm_set_salt(salt, &salt_length,
                          thePrivKey + 8, thePrivKeyLength - 8,
                          &ptr[privParamsOffset])
             == -1)) {
            DEBUGMSGTL(("usm", "Can't set DES-CBC salt.\n"));
            usm_free_usmStateReference(secStateRef);
            return SNMPERR_USM_GENERICERROR;
        }
#ifdef HAVE_AES
        }
#endif

        if (sc_encrypt(thePrivProtocol, thePrivProtocolLength,
                       thePrivKey, thePrivKeyLength,
                       salt, salt_length,
                       scopedPdu, scopedPduLen,
                       &ptr[dataOffset], &encrypted_length)
            != SNMP_ERR_NOERROR) {
            DEBUGMSGTL(("usm", "DES-CBC error.\n"));
            usm_free_usmStateReference(secStateRef);
            return SNMPERR_USM_ENCRYPTIONERROR;
        }
#ifdef SNMP_TESTING_CODE
        if (debug_is_token_registered("usm/dump") == SNMPERR_SUCCESS) {
            dump_chunk("usm/dump", "This data was encrypted:",
                       scopedPdu, scopedPduLen);
            dump_chunk("usm/dump", "salt + Encrypted form:",
                       salt, salt_length);
            dump_chunk("usm/dump", NULL,
                       &ptr[dataOffset], encrypted_length);
            dump_chunk("usm/dump", "*wholeMsg:",
                       *wholeMsg, theTotalLength);
        }
#endif


        ptr = *wholeMsg;
        ptr_len = *wholeMsgLen = theTotalLength;


        /*
         * XXX  Sanity check for salt length should be moved up
         *      under usm_calc_offsets() or tossed.
         */
        if ((encrypted_length != (theTotalLength - dataOffset))
            || (salt_length != msgPrivParmLen)) {
            DEBUGMSGTL(("usm", "DES-CBC length error.\n"));
            usm_free_usmStateReference(secStateRef);
            return SNMPERR_USM_ENCRYPTIONERROR;
        }

        DEBUGMSGTL(("usm", "Encryption successful.\n"));
    }

    /*
     * No encryption for you!
     */
    else {
        memcpy(&ptr[dataOffset], scopedPdu, scopedPduLen);
    }



    /*
     * Start filling in the other fields (in prep for authentication).
     * 
     * offSet is an octet string header, which is different from all
     * the other headers.
     */
    remaining = ptr_len - globalDataLen;

    offSet = ptr_len - remaining;
    asn_build_header(&ptr[offSet], &remaining,
                     (u_char) (ASN_UNIVERSAL | ASN_PRIMITIVE |
                               ASN_OCTET_STR), otstlen);

    offSet = ptr_len - remaining;
    asn_build_sequence(&ptr[offSet], &remaining,
                       (u_char) (ASN_SEQUENCE | ASN_CONSTRUCTOR), seq_len);

    offSet = ptr_len - remaining;
    DEBUGDUMPHEADER("send", "msgAuthoritativeEngineID");
    asn_build_string(&ptr[offSet], &remaining,
                     (u_char) (ASN_UNIVERSAL | ASN_PRIMITIVE |
                               ASN_OCTET_STR), theEngineID,
                     theEngineIDLength);
    DEBUGINDENTLESS();

    offSet = ptr_len - remaining;
    DEBUGDUMPHEADER("send", "msgAuthoritativeEngineBoots");
    asn_build_int(&ptr[offSet], &remaining,
                  (u_char) (ASN_UNIVERSAL | ASN_PRIMITIVE | ASN_INTEGER),
                  &boots_long, sizeof(long));
    DEBUGINDENTLESS();

    offSet = ptr_len - remaining;
    DEBUGDUMPHEADER("send", "msgAuthoritativeEngineTime");
    asn_build_int(&ptr[offSet], &remaining,
                  (u_char) (ASN_UNIVERSAL | ASN_PRIMITIVE | ASN_INTEGER),
                  &time_long, sizeof(long));
    DEBUGINDENTLESS();

    offSet = ptr_len - remaining;
    DEBUGDUMPHEADER("send", "msgUserName");
    asn_build_string(&ptr[offSet], &remaining,
                     (u_char) (ASN_UNIVERSAL | ASN_PRIMITIVE |
                               ASN_OCTET_STR), (u_char *) theName,
                     theNameLength);
    DEBUGINDENTLESS();


    /*
     * Note: if there is no authentication being done,
     * msgAuthParmLen is 0, and there is no effect (other than
     * inserting a zero-length header) of the following
     * statements.
     */

    offSet = ptr_len - remaining;
    asn_build_header(&ptr[offSet],
                     &remaining,
                     (u_char) (ASN_UNIVERSAL | ASN_PRIMITIVE |
                               ASN_OCTET_STR), msgAuthParmLen);

    if (theSecLevel == SNMP_SEC_LEVEL_AUTHNOPRIV
        || theSecLevel == SNMP_SEC_LEVEL_AUTHPRIV) {
        offSet = ptr_len - remaining;
        memset(&ptr[offSet], 0, msgAuthParmLen);
    }

    remaining -= msgAuthParmLen;


    /*
     * Note: if there is no encryption being done, msgPrivParmLen
     * is 0, and there is no effect (other than inserting a
     * zero-length header) of the following statements.
     */

    offSet = ptr_len - remaining;
    asn_build_header(&ptr[offSet],
                     &remaining,
                     (u_char) (ASN_UNIVERSAL | ASN_PRIMITIVE |
                               ASN_OCTET_STR), msgPrivParmLen);

    remaining -= msgPrivParmLen;        /* Skipping the IV already there. */


    /*
     * For privacy, need to add the octet string header for it.
     */
    if (theSecLevel == SNMP_SEC_LEVEL_AUTHPRIV) {
        offSet = ptr_len - remaining;
        asn_build_header(&ptr[offSet],
                         &remaining,
                         (u_char) (ASN_UNIVERSAL | ASN_PRIMITIVE |
                                   ASN_OCTET_STR),
                         theTotalLength - dataOffset);
    }


    /*
     * Adjust overall length and store it as the first SEQ length
     * of the SNMPv3Message.
     *
     * FIX  4 is a magic number!
     */
    remaining = theTotalLength;
    asn_build_sequence(ptr, &remaining,
                       (u_char) (ASN_SEQUENCE | ASN_CONSTRUCTOR),
                       theTotalLength - 4);


    /*
     * Now, time to consider / do authentication.
     */
    if (theSecLevel == SNMP_SEC_LEVEL_AUTHNOPRIV
        || theSecLevel == SNMP_SEC_LEVEL_AUTHPRIV) {
        size_t          temp_sig_len = msgAuthParmLen;
        u_char         *temp_sig = (u_char *) malloc(temp_sig_len);

        if (temp_sig == NULL) {
            DEBUGMSGTL(("usm", "Out of memory.\n"));
            usm_free_usmStateReference(secStateRef);
            return SNMPERR_USM_GENERICERROR;
        }

        if (sc_generate_keyed_hash(theAuthProtocol, theAuthProtocolLength,
                                   theAuthKey, theAuthKeyLength,
                                   ptr, ptr_len, temp_sig, &temp_sig_len)
            != SNMP_ERR_NOERROR) {
            /*
             * FIX temp_sig_len defined?!
             */
            SNMP_ZERO(temp_sig, temp_sig_len);
            SNMP_FREE(temp_sig);
            DEBUGMSGTL(("usm", "Signing failed.\n"));
            usm_free_usmStateReference(secStateRef);
            return SNMPERR_USM_AUTHENTICATIONFAILURE;
        }

        if (temp_sig_len != msgAuthParmLen) {
            SNMP_ZERO(temp_sig, temp_sig_len);
            SNMP_FREE(temp_sig);
            DEBUGMSGTL(("usm", "Signing lengths failed.\n"));
            usm_free_usmStateReference(secStateRef);
            return SNMPERR_USM_AUTHENTICATIONFAILURE;
        }

        memcpy(&ptr[authParamsOffset], temp_sig, msgAuthParmLen);

        SNMP_ZERO(temp_sig, temp_sig_len);
        SNMP_FREE(temp_sig);

    }

    /*
     * endif -- create keyed hash 
     */
    usm_free_usmStateReference(secStateRef);

    DEBUGMSGTL(("usm", "USM processing completed.\n"));

    return SNMPERR_SUCCESS;

}                               /* end usm_generate_out_msg() */

#ifdef USE_REVERSE_ASNENCODING
int
usm_secmod_rgenerate_out_msg(struct snmp_secmod_outgoing_params *parms)
{
    if (!parms)
        return SNMPERR_GENERR;

    return usm_rgenerate_out_msg(parms->msgProcModel,
                                 parms->globalData, parms->globalDataLen,
                                 parms->maxMsgSize, parms->secModel,
                                 parms->secEngineID, parms->secEngineIDLen,
                                 parms->secName, parms->secNameLen,
                                 parms->secLevel,
                                 parms->scopedPdu, parms->scopedPduLen,
                                 parms->secStateRef,
                                 parms->wholeMsg, parms->wholeMsgLen,
                                 parms->wholeMsgOffset);
}

int
usm_rgenerate_out_msg(int msgProcModel, /* (UNUSED) */
                      u_char * globalData,      /* IN */
                      /*
                       * points at the msgGlobalData, which is of length given by next 
                       * parameter.  
                       */
                      size_t globalDataLen,     /* IN - Length of msg header data.      */
                      int maxMsgSize,   /* (UNUSED) */
                      int secModel,     /* (UNUSED) */
                      u_char * secEngineID,     /* IN - Pointer snmpEngineID.           */
                      size_t secEngineIDLen,    /* IN - SnmpEngineID length.            */
                      char *secName,    /* IN - Pointer to securityName.        */
                      size_t secNameLen,        /* IN - SecurityName length.            */
                      int secLevel,     /* IN - AuthNoPriv, authPriv etc.       */
                      u_char * scopedPdu,       /* IN */
                      /*
                       * Pointer to scopedPdu will be encrypted by USM if needed
                       * * and written to packet buffer immediately following
                       * * securityParameters, entire msg will be authenticated by
                       * * USM if needed.
                       */
                      size_t scopedPduLen,      /* IN - scopedPdu length. */
                      void *secStateRef,        /* IN */
                      /*
                       * secStateRef, pointer to cached info provided only for
                       * * Response, otherwise NULL.
                       */
                      u_char ** wholeMsg,       /*  IN/OUT  */
                      /*
                       * Points at the pointer to the packet buffer, which might get extended
                       * if necessary via realloc().  
                       */
                      size_t * wholeMsgLen,     /*  IN/OUT  */
                      /*
                       * Length of the entire packet buffer, **not** the length of the
                       * packet.  
                       */
                      size_t * offset   /*  IN/OUT  */
                      /*
                       * Offset from the end of the packet buffer to the start of the packet,
                       * also known as the packet length.  
                       */
    )
{
    size_t          msgAuthParmLen = 0;
#ifdef SNMP_TESTING_CODE
    size_t          theTotalLength;
#endif

    u_int           boots_uint;
    u_int           time_uint;
    long            boots_long;
    long            time_long;

    /*
     * Indirection because secStateRef values override parameters.
     * 
     * None of these are to be free'd - they are either pointing to
     * what's in the secStateRef or to something either in the
     * actual prarmeter list or the user list.
     */

    char           *theName = NULL;
    u_int           theNameLength = 0;
    u_char         *theEngineID = NULL;
    u_int           theEngineIDLength = 0;
    u_char         *theAuthKey = NULL;
    u_int           theAuthKeyLength = 0;
    const oid      *theAuthProtocol = NULL;
    u_int           theAuthProtocolLength = 0;
    u_char         *thePrivKey = NULL;
    u_int           thePrivKeyLength = 0;
    const oid      *thePrivProtocol = NULL;
    u_int           thePrivProtocolLength = 0;
    int             theSecLevel = 0;    /* No defined const for bad
                                         * value (other then err). */
    size_t          salt_length = 0, save_salt_length = 0, save_salt_offset = 0;
    u_char          salt[BYTESIZE(USM_MAX_SALT_LENGTH)];
    u_char          authParams[USM_MAX_AUTHSIZE];
    u_char          iv[BYTESIZE(USM_MAX_SALT_LENGTH)];
    size_t          sp_offset = 0, mac_offset = 0;
    int             rc = 0;

    DEBUGMSGTL(("usm", "USM processing has begun (offset %d)\n", *offset));

    if (secStateRef != NULL) {
        /*
         * To hush the compiler for now.  XXX 
         */
        struct usmStateReference *ref
            = (struct usmStateReference *) secStateRef;

        theName = ref->usr_name;
        theNameLength = ref->usr_name_length;
        theEngineID = ref->usr_engine_id;
        theEngineIDLength = ref->usr_engine_id_length;

        if (!theEngineIDLength) {
            theEngineID = secEngineID;
            theEngineIDLength = secEngineIDLen;
        }

        theAuthProtocol = ref->usr_auth_protocol;
        theAuthProtocolLength = ref->usr_auth_protocol_length;
        theAuthKey = ref->usr_auth_key;
        theAuthKeyLength = ref->usr_auth_key_length;
        thePrivProtocol = ref->usr_priv_protocol;
        thePrivProtocolLength = ref->usr_priv_protocol_length;
        thePrivKey = ref->usr_priv_key;
        thePrivKeyLength = ref->usr_priv_key_length;
        theSecLevel = ref->usr_sec_level;
    }

    /*
     * * Identify the user record.
     */
    else {
        struct usmUser *user;

        /*
         * we do allow an unknown user name for
         * unauthenticated requests. 
         */
        if ((user = usm_get_user(secEngineID, secEngineIDLen, secName))
            == NULL && secLevel != SNMP_SEC_LEVEL_NOAUTH) {
            DEBUGMSGTL(("usm", "Unknown User\n"));
            usm_free_usmStateReference(secStateRef);
            return SNMPERR_USM_UNKNOWNSECURITYNAME;
        }

        theName = secName;
        theNameLength = secNameLen;
        theEngineID = secEngineID;
        theSecLevel = secLevel;
        theEngineIDLength = secEngineIDLen;
        if (user) {
            theAuthProtocol = user->authProtocol;
            theAuthProtocolLength = user->authProtocolLen;
            theAuthKey = user->authKey;
            theAuthKeyLength = user->authKeyLen;
            thePrivProtocol = user->privProtocol;
            thePrivProtocolLength = user->privProtocolLen;
            thePrivKey = user->privKey;
            thePrivKeyLength = user->privKeyLen;
        } else {
            /*
             * unknown users can not do authentication (obviously) 
             */
            theAuthProtocol = usmNoAuthProtocol;
            theAuthProtocolLength =
                sizeof(usmNoAuthProtocol) / sizeof(oid);
            theAuthKey = NULL;
            theAuthKeyLength = 0;
            thePrivProtocol = usmNoPrivProtocol;
            thePrivProtocolLength =
                sizeof(usmNoPrivProtocol) / sizeof(oid);
            thePrivKey = NULL;
            thePrivKeyLength = 0;
        }
    }                           /* endif -- secStateRef==NULL */


    /*
     * From here to the end of the function, avoid reference to
     * secName, secEngineID, secLevel, and associated lengths.
     */


    /*
     * Check to see if the user can use the requested sec services.
     */
    if (usm_check_secLevel_vs_protocols(theSecLevel,
                                        theAuthProtocol,
                                        theAuthProtocolLength,
                                        thePrivProtocol,
                                        thePrivProtocolLength) == 1) {
        DEBUGMSGTL(("usm", "Unsupported Security Level or type (%d)\n",
                    theSecLevel));

        usm_free_usmStateReference(secStateRef);
        return SNMPERR_USM_UNSUPPORTEDSECURITYLEVEL;
    }


    /*
     * * Retrieve the engine information.
     * *
     * * XXX    No error is declared in the EoP when sending messages to
     * *        unknown engines, processing continues w/ boots/time == (0,0).
     */
    if (get_enginetime(theEngineID, theEngineIDLength,
                       &boots_uint, &time_uint, FALSE) == -1) {
        DEBUGMSGTL(("usm", "%s\n", "Failed to find engine data."));
    }

    boots_long = boots_uint;
    time_long = time_uint;

    if (theSecLevel == SNMP_SEC_LEVEL_AUTHPRIV) {
        /*
         * Initially assume that the ciphertext will end up the same size as
         * the plaintext plus some padding.  Really sc_encrypt ought to be able
         * to grow this for us, a la asn_realloc_rbuild_<type> functions, but
         * this will do for now.  
         */
        u_char         *ciphertext = NULL;
        size_t          ciphertextlen = scopedPduLen + 64;

        if ((ciphertext = (u_char *) malloc(ciphertextlen)) == NULL) {
            DEBUGMSGTL(("usm",
                        "couldn't malloc %d bytes for encrypted PDU\n",
                        ciphertextlen));
            usm_free_usmStateReference(secStateRef);
            return SNMPERR_MALLOC;
        }

        /*
         * XXX Hardwired to seek into a 1DES private key!  
         */
#ifdef HAVE_AES
        if (ISTRANSFORM(thePrivProtocol, AES128Priv) ||
            ISTRANSFORM(thePrivProtocol, AES192Priv) ||
            ISTRANSFORM(thePrivProtocol, AES256Priv)) {
            salt_length = BYTESIZE(USM_AES_SALT_LENGTH);
            save_salt_length = BYTESIZE(USM_AES_SALT_LENGTH)/2;
            save_salt_offset = 0;
            if (!thePrivKey ||
                usm_set_aes_iv(salt, &salt_length,
                               htonl(boots_uint), htonl(time_uint),
                               iv) == -1) {
                DEBUGMSGTL(("usm", "Can't set AES iv.\n"));
                usm_free_usmStateReference(secStateRef);
                free(ciphertext);
                return SNMPERR_USM_GENERICERROR;
            }
        } else if (ISTRANSFORM(thePrivProtocol, DESPriv)) {
#endif
            salt_length = BYTESIZE(USM_DES_SALT_LENGTH);
            save_salt_length = BYTESIZE(USM_DES_SALT_LENGTH);
            save_salt_offset = 0;
            if (!thePrivKey || (usm_set_salt(salt, &salt_length,
                                             thePrivKey + 8,
                                             thePrivKeyLength - 8,
                                             iv) == -1)) {
                DEBUGMSGTL(("usm", "Can't set DES-CBC salt.\n"));
                usm_free_usmStateReference(secStateRef);
                free(ciphertext);
                return SNMPERR_USM_GENERICERROR;
            }
#ifdef HAVE_AES
        }
#endif
#ifdef SNMP_TESTING_CODE
        if (debug_is_token_registered("usm/dump") == SNMPERR_SUCCESS) {
            dump_chunk("usm/dump", "This data was encrypted:",
                       scopedPdu, scopedPduLen);
        }
#endif

        if (sc_encrypt(thePrivProtocol, thePrivProtocolLength,
                       thePrivKey, thePrivKeyLength,
                       salt, salt_length,
                       scopedPdu, scopedPduLen,
                       ciphertext, &ciphertextlen) != SNMP_ERR_NOERROR) {
            DEBUGMSGTL(("usm", "DES-CBC error.\n"));
            usm_free_usmStateReference(secStateRef);
            free(ciphertext);
            return SNMPERR_USM_ENCRYPTIONERROR;
        }

        /*
         * Write the encrypted scopedPdu back into the packet buffer.  
         */

#ifdef SNMP_TESTING_CODE
        theTotalLength = *wholeMsgLen;
#endif
        *offset = 0;
        rc = asn_realloc_rbuild_string(wholeMsg, wholeMsgLen, offset, 1,
                                       (u_char) (ASN_UNIVERSAL |
                                                 ASN_PRIMITIVE |
                                                 ASN_OCTET_STR),
                                       ciphertext, ciphertextlen);
#ifdef SNMP_TESTING_CODE
        if (debug_is_token_registered("usm/dump") == SNMPERR_SUCCESS) {
            dump_chunk("usm/dump", "salt + Encrypted form: ", salt,
                       salt_length);
            dump_chunk("usm/dump", "wholeMsg:",
                       (*wholeMsg + *wholeMsgLen - *offset), *offset);
        }
#endif

        DEBUGMSGTL(("usm", "Encryption successful.\n"));
        free(ciphertext);
    } else {
        /*
         * theSecLevel != SNMP_SEC_LEVEL_AUTHPRIV  
         */
    }

    /*
     * Start encoding the msgSecurityParameters.  
     */

    sp_offset = *offset;

    DEBUGDUMPHEADER("send", "msgPrivacyParameters");
    /*
     * msgPrivacyParameters (warning: assumes DES salt).  
     */
    rc = asn_realloc_rbuild_string(wholeMsg, wholeMsgLen, offset, 1,
                                   (u_char) (ASN_UNIVERSAL | ASN_PRIMITIVE
                                             | ASN_OCTET_STR),
                                   iv,
                                   save_salt_length);
    DEBUGINDENTLESS();
    if (rc == 0) {
        DEBUGMSGTL(("usm", "building privParams failed.\n"));
        usm_free_usmStateReference(secStateRef);
        return SNMPERR_TOO_LONG;
    }

    DEBUGDUMPHEADER("send", "msgAuthenticationParameters");
    /*
     * msgAuthenticationParameters (warnings assumes 0x00 by 12).  
     */
    if (theSecLevel == SNMP_SEC_LEVEL_AUTHNOPRIV
        || theSecLevel == SNMP_SEC_LEVEL_AUTHPRIV) {
        memset(authParams, 0, USM_MD5_AND_SHA_AUTH_LEN);
        msgAuthParmLen = USM_MD5_AND_SHA_AUTH_LEN;
    }

    rc = asn_realloc_rbuild_string(wholeMsg, wholeMsgLen, offset, 1,
                                   (u_char) (ASN_UNIVERSAL | ASN_PRIMITIVE
                                             | ASN_OCTET_STR), authParams,
                                   msgAuthParmLen);
    DEBUGINDENTLESS();
    if (rc == 0) {
        DEBUGMSGTL(("usm", "building authParams failed.\n"));
        usm_free_usmStateReference(secStateRef);
        return SNMPERR_TOO_LONG;
    }

    /*
     * Remember where to put the actual HMAC we calculate later on.  An
     * encoded OCTET STRING of length USM_MD5_AND_SHA_AUTH_LEN has an ASN.1
     * header of length 2, hence the fudge factor.  
     */

    mac_offset = *offset - 2;

    /*
     * msgUserName.  
     */
    DEBUGDUMPHEADER("send", "msgUserName");
    rc = asn_realloc_rbuild_string(wholeMsg, wholeMsgLen, offset, 1,
                                   (u_char) (ASN_UNIVERSAL | ASN_PRIMITIVE
                                             | ASN_OCTET_STR),
                                   (u_char *) theName, theNameLength);
    DEBUGINDENTLESS();
    if (rc == 0) {
        DEBUGMSGTL(("usm", "building authParams failed.\n"));
        usm_free_usmStateReference(secStateRef);
        return SNMPERR_TOO_LONG;
    }

    /*
     * msgAuthoritativeEngineTime.  
     */
    DEBUGDUMPHEADER("send", "msgAuthoritativeEngineTime");
    rc = asn_realloc_rbuild_int(wholeMsg, wholeMsgLen, offset, 1,
                                (u_char) (ASN_UNIVERSAL | ASN_PRIMITIVE |
                                          ASN_INTEGER), &time_long,
                                sizeof(long));
    DEBUGINDENTLESS();
    if (rc == 0) {
        DEBUGMSGTL(("usm",
                    "building msgAuthoritativeEngineTime failed.\n"));
        usm_free_usmStateReference(secStateRef);
        return SNMPERR_TOO_LONG;
    }

    /*
     * msgAuthoritativeEngineBoots.  
     */
    DEBUGDUMPHEADER("send", "msgAuthoritativeEngineBoots");
    rc = asn_realloc_rbuild_int(wholeMsg, wholeMsgLen, offset, 1,
                                (u_char) (ASN_UNIVERSAL | ASN_PRIMITIVE |
                                          ASN_INTEGER), &boots_long,
                                sizeof(long));
    DEBUGINDENTLESS();
    if (rc == 0) {
        DEBUGMSGTL(("usm",
                    "building msgAuthoritativeEngineBoots failed.\n"));
        usm_free_usmStateReference(secStateRef);
        return SNMPERR_TOO_LONG;
    }

    DEBUGDUMPHEADER("send", "msgAuthoritativeEngineID");
    rc = asn_realloc_rbuild_string(wholeMsg, wholeMsgLen, offset, 1,
                                   (u_char) (ASN_UNIVERSAL | ASN_PRIMITIVE
                                             | ASN_OCTET_STR), theEngineID,
                                   theEngineIDLength);
    DEBUGINDENTLESS();
    if (rc == 0) {
        DEBUGMSGTL(("usm", "building msgAuthoritativeEngineID failed.\n"));
        usm_free_usmStateReference(secStateRef);
        return SNMPERR_TOO_LONG;
    }

    /*
     * USM msgSecurityParameters sequence header  
     */
    rc = asn_realloc_rbuild_sequence(wholeMsg, wholeMsgLen, offset, 1,
                                     (u_char) (ASN_SEQUENCE |
                                               ASN_CONSTRUCTOR),
                                     *offset - sp_offset);
    if (rc == 0) {
        DEBUGMSGTL(("usm", "building usm security parameters failed.\n"));
        usm_free_usmStateReference(secStateRef);
        return SNMPERR_TOO_LONG;
    }

    /*
     * msgSecurityParameters OCTET STRING wrapper.  
     */
    rc = asn_realloc_rbuild_header(wholeMsg, wholeMsgLen, offset, 1,
                                   (u_char) (ASN_UNIVERSAL | ASN_PRIMITIVE
                                             | ASN_OCTET_STR),
                                   *offset - sp_offset);

    if (rc == 0) {
        DEBUGMSGTL(("usm", "building msgSecurityParameters failed.\n"));
        usm_free_usmStateReference(secStateRef);
        return SNMPERR_TOO_LONG;
    }

    /*
     * Copy in the msgGlobalData and msgVersion.  
     */
    while ((*wholeMsgLen - *offset) < globalDataLen) {
        if (!asn_realloc(wholeMsg, wholeMsgLen)) {
            DEBUGMSGTL(("usm", "building global data failed.\n"));
            usm_free_usmStateReference(secStateRef);
            return SNMPERR_TOO_LONG;
        }
    }

    *offset += globalDataLen;
    memcpy(*wholeMsg + *wholeMsgLen - *offset, globalData, globalDataLen);

    /*
     * Total packet sequence.  
     */
    rc = asn_realloc_rbuild_sequence(wholeMsg, wholeMsgLen, offset, 1,
                                     (u_char) (ASN_SEQUENCE |
                                               ASN_CONSTRUCTOR), *offset);
    if (rc == 0) {
        DEBUGMSGTL(("usm", "building master packet sequence failed.\n"));
        usm_free_usmStateReference(secStateRef);
        return SNMPERR_TOO_LONG;
    }

    /*
     * Now consider / do authentication.  
     */

    if (theSecLevel == SNMP_SEC_LEVEL_AUTHNOPRIV ||
        theSecLevel == SNMP_SEC_LEVEL_AUTHPRIV) {
        size_t          temp_sig_len = msgAuthParmLen;
        u_char         *temp_sig = (u_char *) malloc(temp_sig_len);
        u_char         *proto_msg = *wholeMsg + *wholeMsgLen - *offset;
        size_t          proto_msg_len = *offset;


        if (temp_sig == NULL) {
            DEBUGMSGTL(("usm", "Out of memory.\n"));
            usm_free_usmStateReference(secStateRef);
            return SNMPERR_USM_GENERICERROR;
        }

        if (sc_generate_keyed_hash(theAuthProtocol, theAuthProtocolLength,
                                   theAuthKey, theAuthKeyLength,
                                   proto_msg, proto_msg_len,
                                   temp_sig, &temp_sig_len)
            != SNMP_ERR_NOERROR) {
            SNMP_FREE(temp_sig);
            DEBUGMSGTL(("usm", "Signing failed.\n"));
            usm_free_usmStateReference(secStateRef);
            return SNMPERR_USM_AUTHENTICATIONFAILURE;
        }

        if (temp_sig_len != msgAuthParmLen) {
            SNMP_FREE(temp_sig);
            DEBUGMSGTL(("usm", "Signing lengths failed.\n"));
            usm_free_usmStateReference(secStateRef);
            return SNMPERR_USM_AUTHENTICATIONFAILURE;
        }

        memcpy(*wholeMsg + *wholeMsgLen - mac_offset, temp_sig,
               msgAuthParmLen);
        SNMP_FREE(temp_sig);
    }
    /*
     * endif -- create keyed hash 
     */
    usm_free_usmStateReference(secStateRef);
    DEBUGMSGTL(("usm", "USM processing completed.\n"));
    return SNMPERR_SUCCESS;
}                               /* end usm_rgenerate_out_msg() */

#endif                          /* */



/*******************************************************************-o-******
 * usm_parse_security_parameters
 *
 * Parameters:
 *	(See list below...)
 *      
 * Returns:
 *	0	On success,
 *	-1	Otherwise.
 *
 *	tab stop 4
 *
 *	Extracts values from the security header and data portions of the
 *	incoming buffer.
 */
int
usm_parse_security_parameters(u_char * secParams,
                              size_t remaining,
                              u_char * secEngineID,
                              size_t * secEngineIDLen,
                              u_int * boots_uint,
                              u_int * time_uint,
                              char *secName,
                              size_t * secNameLen,
                              u_char * signature,
                              size_t * signature_length,
                              u_char * salt,
                              size_t * salt_length, u_char ** data_ptr)
{
    u_char         *parse_ptr = secParams;
    u_char         *value_ptr;
    u_char         *next_ptr;
    u_char          type_value;

    size_t          octet_string_length = remaining;
    size_t          sequence_length;
    size_t          remaining_bytes;

    long            boots_long;
    long            time_long;

    u_int           origNameLen;


    /*
     * Eat the first octet header.
     */
    if ((value_ptr = asn_parse_sequence(parse_ptr, &octet_string_length,
                                        &type_value,
                                        (ASN_UNIVERSAL | ASN_PRIMITIVE |
                                         ASN_OCTET_STR),
                                        "usm first octet")) == NULL) {
        /*
         * RETURN parse error 
         */ return -1;
    }


    /*
     * Eat the sequence header.
     */
    parse_ptr = value_ptr;
    sequence_length = octet_string_length;

    if ((value_ptr = asn_parse_sequence(parse_ptr, &sequence_length,
                                        &type_value,
                                        (ASN_SEQUENCE | ASN_CONSTRUCTOR),
                                        "usm sequence")) == NULL) {
        /*
         * RETURN parse error 
         */ return -1;
    }


    /*
     * Retrieve the engineID.
     */
    parse_ptr = value_ptr;
    remaining_bytes = sequence_length;

    DEBUGDUMPHEADER("recv", "msgAuthoritativeEngineID");
    if ((next_ptr
         = asn_parse_string(parse_ptr, &remaining_bytes, &type_value,
                            secEngineID, secEngineIDLen)) == NULL) {
        DEBUGINDENTLESS();
        /*
         * RETURN parse error 
         */ return -1;
    }
    DEBUGINDENTLESS();

    if (type_value !=
        (u_char) (ASN_UNIVERSAL | ASN_PRIMITIVE | ASN_OCTET_STR)) {
        /*
         * RETURN parse error 
         */ return -1;
    }


    /*
     * Retrieve the engine boots, notice switch in the way next_ptr and
     * remaining_bytes are used (to accomodate the asn code).
     */
    DEBUGDUMPHEADER("recv", "msgAuthoritativeEngineBoots");
    if ((next_ptr = asn_parse_int(next_ptr, &remaining_bytes, &type_value,
                                  &boots_long, sizeof(long))) == NULL) {
        DEBUGINDENTLESS();
        /*
         * RETURN parse error 
         */ return -1;
    }
    DEBUGINDENTLESS();

    if (type_value !=
        (u_char) (ASN_UNIVERSAL | ASN_PRIMITIVE | ASN_INTEGER)) {
        DEBUGINDENTLESS();
        /*
         * RETURN parse error 
         */ return -1;
    }

    *boots_uint = (u_int) boots_long;


    /*
     * Retrieve the time value.
     */
    DEBUGDUMPHEADER("recv", "msgAuthoritativeEngineTime");
    if ((next_ptr = asn_parse_int(next_ptr, &remaining_bytes, &type_value,
                                  &time_long, sizeof(long))) == NULL) {
        /*
         * RETURN parse error 
         */ return -1;
    }
    DEBUGINDENTLESS();

    if (type_value !=
        (u_char) (ASN_UNIVERSAL | ASN_PRIMITIVE | ASN_INTEGER)) {
        /*
         * RETURN parse error 
         */ return -1;
    }

    *time_uint = (u_int) time_long;


    /*
     * Retrieve the secName.
     */
    origNameLen = *secNameLen;


    DEBUGDUMPHEADER("recv", "msgUserName");
    if ((next_ptr
         = asn_parse_string(next_ptr, &remaining_bytes, &type_value,
                            (u_char *) secName, secNameLen)) == NULL) {
        DEBUGINDENTLESS();
        /*
         * RETURN parse error 
         */ return -1;
    }
    DEBUGINDENTLESS();

    /*
     * FIX -- doesn't this also indicate a buffer overrun?
     */
    if ((int) origNameLen < *secNameLen + 1) {
        /*
         * RETURN parse error, but it's really a parameter error 
         */
        return -1;
    }

    if (*secNameLen > 32) {
        /*
         * This is a USM-specific limitation over and above the above
         * limitation (which will probably default to the length of an
         * SnmpAdminString, i.e. 255).  See RFC 2574, sec. 2.4.  
         */
        return -1;
    }

    secName[*secNameLen] = '\0';

    if (type_value !=
        (u_char) (ASN_UNIVERSAL | ASN_PRIMITIVE | ASN_OCTET_STR)) {
        /*
         * RETURN parse error 
         */ return -1;
    }


    /*
     * Retrieve the signature and blank it if there.
     */
    DEBUGDUMPHEADER("recv", "msgAuthenticationParameters");
    if ((next_ptr
         = asn_parse_string(next_ptr, &remaining_bytes, &type_value,
                            signature, signature_length)) == NULL) {
        DEBUGINDENTLESS();
        /*
         * RETURN parse error 
         */ return -1;
    }
    DEBUGINDENTLESS();

    if (type_value !=
        (u_char) (ASN_UNIVERSAL | ASN_PRIMITIVE | ASN_OCTET_STR)) {
        /*
         * RETURN parse error 
         */ return -1;
    }

    if (*signature_length != 0) {       /* Blanking for authentication step later */
        memset(next_ptr - (u_long) * signature_length,
               0, *signature_length);
    }


    /*
     * Retrieve the salt.
     *
     * Note that the next ptr is where the data section starts.
     */
    DEBUGDUMPHEADER("recv", "msgPrivacyParameters");
    if ((*data_ptr
         = asn_parse_string(next_ptr, &remaining_bytes, &type_value,
                            salt, salt_length)) == NULL) {
        DEBUGINDENTLESS();
        /*
         * RETURN parse error 
         */ return -2;
    }
    DEBUGINDENTLESS();

    if (type_value !=
        (u_char) (ASN_UNIVERSAL | ASN_PRIMITIVE | ASN_OCTET_STR)) {
        /*
         * RETURN parse error 
         */ return -2;
    }

    return 0;

}                               /* end usm_parse_security_parameters() */




/*******************************************************************-o-******
 * usm_check_and_update_timeliness
 *
 * Parameters:
 *	*secEngineID
 *	 secEngineIDen
 *	 boots_uint
 *	 time_uint
 *	*error
 *      
 * Returns:
 *	0	On success,
 *	-1	Otherwise.
 *	
 *
 * Performs the incoming timeliness checking and setting.
 */
int
usm_check_and_update_timeliness(u_char * secEngineID,
                                size_t secEngineIDLen,
                                u_int boots_uint,
                                u_int time_uint, int *error)
{
    u_char          myID[USM_MAX_ID_LENGTH];
    u_long          myIDLength =
        snmpv3_get_engineID(myID, USM_MAX_ID_LENGTH);
    u_int           myBoots;
    u_int           myTime;



    if ((myIDLength > USM_MAX_ID_LENGTH) || (myIDLength == 0)) {
        /*
         * We're probably already screwed...buffer overwrite.  XXX? 
         */
        DEBUGMSGTL(("usm", "Buffer overflow.\n"));
        *error = SNMPERR_USM_GENERICERROR;
        return -1;
    }

    myBoots = snmpv3_local_snmpEngineBoots();
    myTime = snmpv3_local_snmpEngineTime();


    /*
     * IF the time involved is local
     *     Make sure  message is inside the time window 
     * ELSE 
     *      IF boots is higher or boots is the same and time is higher
     *              remember this new data
     *      ELSE
     *              IF !(boots same and time within USM_TIME_WINDOW secs)
     *                      Message is too old 
     *              ELSE    
     *                      Message is ok, but don't take time
     *              ENDIF
     *      ENDIF
     * ENDIF
     */

    /*
     * This is a local reference.
     */
    if ((int) secEngineIDLen == myIDLength
        && memcmp(secEngineID, myID, myIDLength) == 0) {
        u_int           time_difference = myTime > time_uint ?
            myTime - time_uint : time_uint - myTime;

        if (boots_uint == ENGINEBOOT_MAX
            || boots_uint != myBoots
            || time_difference > USM_TIME_WINDOW) {
            if (snmp_increment_statistic(STAT_USMSTATSNOTINTIMEWINDOWS) ==
                0) {
                DEBUGMSGTL(("usm", "%s\n",
                            "Failed to increment statistic."));
            }

            DEBUGMSGTL(("usm",
                        "boot_uint %u myBoots %u time_diff %u => not in time window\n",
                        boots_uint, myBoots, time_difference));
            *error = SNMPERR_USM_NOTINTIMEWINDOW;
            return -1;
        }

        *error = SNMPERR_SUCCESS;
        return 0;
    }

    /*
     * This is a remote reference.
     */
    else {
        u_int           theirBoots, theirTime, theirLastTime;
        u_int           time_difference;

        if (get_enginetime_ex(secEngineID, secEngineIDLen,
                              &theirBoots, &theirTime,
                              &theirLastTime, TRUE)
            != SNMPERR_SUCCESS) {
            DEBUGMSGTL(("usm", "%s\n",
                        "Failed to get remote engine's times."));

            *error = SNMPERR_USM_GENERICERROR;
            return -1;
        }

        time_difference = theirTime > time_uint ?
            theirTime - time_uint : time_uint - theirTime;


        /*
         * XXX  Contrary to the pseudocode:
         *      See if boots is invalid first.
         */
        if (theirBoots == ENGINEBOOT_MAX || theirBoots > boots_uint) {
            DEBUGMSGTL(("usm", "%s\n", "Remote boot count invalid."));

            *error = SNMPERR_USM_NOTINTIMEWINDOW;
            return -1;
        }


        /*
         * Boots is ok, see if the boots is the same but the time
         * is old.
         */
        if (theirBoots == boots_uint && time_uint < theirLastTime) {
            if (time_difference > USM_TIME_WINDOW) {
                DEBUGMSGTL(("usm", "%s\n", "Message too old."));
                *error = SNMPERR_USM_NOTINTIMEWINDOW;
                return -1;
            }

            else {              /* Old, but acceptable */

                *error = SNMPERR_SUCCESS;
                return 0;
            }
        }


        /*
         * Message is ok, either boots has been advanced, or
         * time is greater than before with the same boots.
         */

        if (set_enginetime(secEngineID, secEngineIDLen,
                           boots_uint, time_uint, TRUE)
            != SNMPERR_SUCCESS) {
            DEBUGMSGTL(("usm", "%s\n",
                        "Failed updating remote boot/time."));
            *error = SNMPERR_USM_GENERICERROR;
            return -1;
        }

        *error = SNMPERR_SUCCESS;
        return 0;               /* Fresh message and time updated */

    }                           /* endif -- local or remote time reference. */


}                               /* end usm_check_and_update_timeliness() */



int
usm_secmod_process_in_msg(struct snmp_secmod_incoming_params *parms)
{
    if (!parms)
        return SNMPERR_GENERR;

    return usm_process_in_msg(parms->msgProcModel,
                              parms->maxMsgSize,
                              parms->secParams,
                              parms->secModel,
                              parms->secLevel,
                              parms->wholeMsg,
                              parms->wholeMsgLen,
                              parms->secEngineID,
                              parms->secEngineIDLen,
                              parms->secName,
                              parms->secNameLen,
                              parms->scopedPdu,
                              parms->scopedPduLen,
                              parms->maxSizeResponse,
                              parms->secStateRef,
                              parms->sess, parms->msg_flags);
}

/*******************************************************************-o-******
 * usm_process_in_msg
 *
 * Parameters:
 *	(See list below...)
 *      
 * Returns:
 *	SNMPERR_SUCCESS			On success.
 *	SNMPERR_USM_AUTHENTICATIONFAILURE
 *	SNMPERR_USM_DECRYPTIONERROR
 *	SNMPERR_USM_GENERICERROR
 *	SNMPERR_USM_PARSEERROR
 *	SNMPERR_USM_UNKNOWNENGINEID
 *	SNMPERR_USM_PARSEERROR
 *	SNMPERR_USM_UNKNOWNSECURITYNAME
 *	SNMPERR_USM_UNSUPPORTEDSECURITYLEVEL
 *
 *
 * ASSUMES size of decrypt_buf will always be >= size of encrypted sPDU.
 *
 * FIX  Memory leaks if secStateRef is allocated and a return occurs
 *	without cleaning up.  May contain secrets...
 */
int
usm_process_in_msg(int msgProcModel,    /* (UNUSED) */
                   size_t maxMsgSize,   /* IN     - Used to calc maxSizeResponse.  */
                   u_char * secParams,  /* IN     - BER encoded securityParameters. */
                   int secModel,        /* (UNUSED) */
                   int secLevel,        /* IN     - AuthNoPriv, authPriv etc.      */
                   u_char * wholeMsg,   /* IN     - Original v3 message.           */
                   size_t wholeMsgLen,  /* IN     - Msg length.                    */
                   u_char * secEngineID,        /* OUT    - Pointer snmpEngineID.          */
                   size_t * secEngineIDLen,     /* IN/OUT - Len available, len returned.   */
                   /*
                    * NOTE: Memory provided by caller.      
                    */
                   char *secName,       /* OUT    - Pointer to securityName.       */
                   size_t * secNameLen, /* IN/OUT - Len available, len returned.   */
                   u_char ** scopedPdu, /* OUT    - Pointer to plaintext scopedPdu. */
                   size_t * scopedPduLen,       /* IN/OUT - Len available, len returned.   */
                   size_t * maxSizeResponse,    /* OUT    - Max size of Response PDU.      */
                   void **secStateRf,   /* OUT    - Ref to security state.         */
                   netsnmp_session * sess,      /* IN     - session which got the message  */
                   u_char msg_flags)
{                               /* IN     - v3 Message flags.              */
    size_t          remaining = wholeMsgLen - (u_int)
        ((u_long) * secParams - (u_long) * wholeMsg);
    u_int           boots_uint;
    u_int           time_uint;
#ifdef HAVE_AES
    u_int           net_boots, net_time;
#endif
    u_char          signature[BYTESIZE(USM_MAX_KEYEDHASH_LENGTH)];
    size_t          signature_length = BYTESIZE(USM_MAX_KEYEDHASH_LENGTH);
    u_char          salt[BYTESIZE(USM_MAX_SALT_LENGTH)];
    size_t          salt_length = BYTESIZE(USM_MAX_SALT_LENGTH);
    u_char          iv[BYTESIZE(USM_MAX_SALT_LENGTH)];
    u_int           iv_length = BYTESIZE(USM_MAX_SALT_LENGTH);
    u_char         *data_ptr;
    u_char         *value_ptr;
    u_char          type_value;
    u_char         *end_of_overhead;
    int             error;
    int             i, rc = 0;
    struct usmStateReference **secStateRef =
        (struct usmStateReference **) secStateRf;

    struct usmUser *user;


    DEBUGMSGTL(("usm", "USM processing begun...\n"));


    if (secStateRef) {
        usm_free_usmStateReference(*secStateRef);
        *secStateRef = usm_malloc_usmStateReference();
        if (*secStateRef == NULL) {
            DEBUGMSGTL(("usm", "Out of memory.\n"));
            return SNMPERR_USM_GENERICERROR;
        }
    }


    /*
     * Make sure the *secParms is an OCTET STRING.
     * Extract the user name, engine ID, and security level.
     */
    if ((rc = usm_parse_security_parameters(secParams, remaining,
                                            secEngineID, secEngineIDLen,
                                            &boots_uint, &time_uint,
                                            secName, secNameLen,
                                            signature, &signature_length,
                                            salt, &salt_length,
                                            &data_ptr)) < 0) {
        DEBUGMSGTL(("usm", "Parsing failed (rc %d).\n", rc));
        if (rc == -2) {
            /*
             * This indicates a decryptionError.  
             */
            if (snmp_increment_statistic(STAT_USMSTATSDECRYPTIONERRORS) ==
                0) {
                DEBUGMSGTL(("usm", "%s\n",
                            "Failed to increment statistic."));
            }
            return SNMPERR_USM_DECRYPTIONERROR;
        }
        if (snmp_increment_statistic(STAT_SNMPINASNPARSEERRS) == 0) {
            DEBUGMSGTL(("usm", "%s\n", "Failed to increment statistic."));
        }
        return SNMPERR_USM_PARSEERROR;
    }


    if (secLevel != SNMP_SEC_LEVEL_AUTHPRIV) {
        /*
         * pull these out now so reports can use them 
         */
        *scopedPdu = data_ptr;
        *scopedPduLen = wholeMsgLen - (data_ptr - wholeMsg);
        end_of_overhead = data_ptr;
    }

    if (secStateRef) {
        /*
         * Cache the name, engine ID, and security level,
         * * per step 2 (section 3.2)
         */
        if (usm_set_usmStateReference_name
            (*secStateRef, secName, *secNameLen) == -1) {
            DEBUGMSGTL(("usm", "%s\n", "Couldn't cache name."));
            return SNMPERR_USM_GENERICERROR;
        }

        if (usm_set_usmStateReference_engine_id
            (*secStateRef, secEngineID, *secEngineIDLen) == -1) {
            DEBUGMSGTL(("usm", "%s\n", "Couldn't cache engine id."));
            return SNMPERR_USM_GENERICERROR;
        }

        if (usm_set_usmStateReference_sec_level(*secStateRef, secLevel) ==
            -1) {
            DEBUGMSGTL(("usm", "%s\n", "Couldn't cache security level."));
            return SNMPERR_USM_GENERICERROR;
        }
    }


    /*
     * Locate the engine ID record.
     * If it is unknown, then either create one or note this as an error.
     */
    if ((sess && (sess->isAuthoritative == SNMP_SESS_AUTHORITATIVE ||
                  (sess->isAuthoritative == SNMP_SESS_UNKNOWNAUTH &&
                   (msg_flags & SNMP_MSG_FLAG_RPRT_BIT)))) ||
        (!sess && (msg_flags & SNMP_MSG_FLAG_RPRT_BIT))) {
        if (ISENGINEKNOWN(secEngineID, *secEngineIDLen) == FALSE) {
            DEBUGMSGTL(("usm", "Unknown Engine ID.\n"));
            if (snmp_increment_statistic(STAT_USMSTATSUNKNOWNENGINEIDS) ==
                0) {
                DEBUGMSGTL(("usm", "%s\n",
                            "Failed to increment statistic."));
            }
            return SNMPERR_USM_UNKNOWNENGINEID;
        }
    } else {
        if (ENSURE_ENGINE_RECORD(secEngineID, *secEngineIDLen)
            != SNMPERR_SUCCESS) {
            DEBUGMSGTL(("usm", "%s\n", "Couldn't ensure engine record."));
            return SNMPERR_USM_GENERICERROR;
        }

    }


    /*
     * Locate the User record.
     * If the user/engine ID is unknown, report this as an error.
     */
    if ((user = usm_get_user_from_list(secEngineID, *secEngineIDLen,
                                       secName, userList,
                                       (sess->isAuthoritative ==
                                        SNMP_SESS_AUTHORITATIVE) ? 0 : 1))
        == NULL) {
        DEBUGMSGTL(("usm", "Unknown User(%s)\n", secName));
        if (snmp_increment_statistic(STAT_USMSTATSUNKNOWNUSERNAMES) == 0) {
            DEBUGMSGTL(("usm", "%s\n", "Failed to increment statistic."));
        }
        return SNMPERR_USM_UNKNOWNSECURITYNAME;
    }


    /*
     * Make sure the security level is appropriate.
     */
    if (usm_check_secLevel(secLevel, user) == 1) {
        DEBUGMSGTL(("usm", "Unsupported Security Level (%d).\n",
                    secLevel));
        if (snmp_increment_statistic
            (STAT_USMSTATSUNSUPPORTEDSECLEVELS) == 0) {
            DEBUGMSGTL(("usm", "%s\n", "Failed to increment statistic."));
        }
        return SNMPERR_USM_UNSUPPORTEDSECURITYLEVEL;
    }


    /*
     * Check the authentication credentials of the message.
     */
    if (secLevel == SNMP_SEC_LEVEL_AUTHNOPRIV
        || secLevel == SNMP_SEC_LEVEL_AUTHPRIV) {
        if (sc_check_keyed_hash(user->authProtocol, user->authProtocolLen,
                                user->authKey, user->authKeyLen,
                                wholeMsg, wholeMsgLen,
                                signature, signature_length)
            != SNMP_ERR_NOERROR) {
            DEBUGMSGTL(("usm", "Verification failed.\n"));
            if (snmp_increment_statistic(STAT_USMSTATSWRONGDIGESTS) == 0) {
                DEBUGMSGTL(("usm", "%s\n",
                            "Failed to increment statistic."));
            }
            return SNMPERR_USM_AUTHENTICATIONFAILURE;
        }

        DEBUGMSGTL(("usm", "Verification succeeded.\n"));
    }


    /*
     * Steps 10-11  user is already set - relocated before timeliness 
     * check in case it fails - still save user data for response.
     *
     * Cache the keys and protocol oids, per step 11 (s3.2).
     */
    if (secStateRef) {
        if (usm_set_usmStateReference_auth_protocol(*secStateRef,
                                                    user->authProtocol,
                                                    user->
                                                    authProtocolLen) ==
            -1) {
            DEBUGMSGTL(("usm", "%s\n",
                        "Couldn't cache authentication protocol."));
            return SNMPERR_USM_GENERICERROR;
        }

        if (usm_set_usmStateReference_auth_key(*secStateRef,
                                               user->authKey,
                                               user->authKeyLen) == -1) {
            DEBUGMSGTL(("usm", "%s\n",
                        "Couldn't cache authentiation key."));
            return SNMPERR_USM_GENERICERROR;
        }

        if (usm_set_usmStateReference_priv_protocol(*secStateRef,
                                                    user->privProtocol,
                                                    user->
                                                    privProtocolLen) ==
            -1) {
            DEBUGMSGTL(("usm", "%s\n",
                        "Couldn't cache privacy protocol."));
            return SNMPERR_USM_GENERICERROR;
        }

        if (usm_set_usmStateReference_priv_key(*secStateRef,
                                               user->privKey,
                                               user->privKeyLen) == -1) {
            DEBUGMSGTL(("usm", "%s\n", "Couldn't cache privacy key."));
            return SNMPERR_USM_GENERICERROR;
        }
    }


    /*
     * Perform the timeliness/time manager functions.
     */
    if (secLevel == SNMP_SEC_LEVEL_AUTHNOPRIV
        || secLevel == SNMP_SEC_LEVEL_AUTHPRIV) {
        if (usm_check_and_update_timeliness(secEngineID, *secEngineIDLen,
                                            boots_uint, time_uint,
                                            &error) == -1) {
            return error;
        }
    }
#ifdef							LCD_TIME_SYNC_OPT
    /*
     * Cache the unauthenticated time to use in case we don't have
     * anything better - this guess will be no worse than (0,0)
     * that we normally use.
     */
    else {
        set_enginetime(secEngineID, *secEngineIDLen,
                       boots_uint, time_uint, FALSE);
    }
#endif                          /* LCD_TIME_SYNC_OPT */


    /*
     * If needed, decrypt the scoped PDU.
     */
    if (secLevel == SNMP_SEC_LEVEL_AUTHPRIV) {
        remaining = wholeMsgLen - (data_ptr - wholeMsg);

        if ((value_ptr = asn_parse_sequence(data_ptr, &remaining,
                                            &type_value,
                                            (ASN_UNIVERSAL | ASN_PRIMITIVE
                                             | ASN_OCTET_STR),
                                            "encrypted sPDU")) == NULL) {
            DEBUGMSGTL(("usm", "%s\n",
                        "Failed while parsing encrypted sPDU."));
            if (snmp_increment_statistic(STAT_SNMPINASNPARSEERRS) == 0) {
                DEBUGMSGTL(("usm", "%s\n", "Failed increment statistic."));
            }
            usm_free_usmStateReference(*secStateRef);
            *secStateRef = NULL;
            return SNMPERR_USM_PARSEERROR;
        }

        if (ISTRANSFORM(user->privProtocol, DESPriv)) {
            /*
             * From RFC2574:
             * 
             * "Before decryption, the encrypted data length is verified.
             * If the length of the OCTET STRING to be decrypted is not
             * an integral multiple of 8 octets, the decryption process
             * is halted and an appropriate exception noted."  
             */

            if (remaining % 8 != 0) {
                DEBUGMSGTL(("usm",
                            "Ciphertext is %lu bytes, not an integer multiple of 8 (rem %d)\n",
                            remaining, remaining % 8));
                if (snmp_increment_statistic(STAT_USMSTATSDECRYPTIONERRORS) ==
                    0) {
                    DEBUGMSGTL(("usm", "%s\n", "Failed increment statistic."));
                }
                usm_free_usmStateReference(*secStateRef);
                *secStateRef = NULL;
                return SNMPERR_USM_DECRYPTIONERROR;
            }

            end_of_overhead = value_ptr;

            /*
             * XOR the salt with the last (iv_length) bytes
             * of the priv_key to obtain the IV.
             */
            iv_length = BYTESIZE(USM_DES_SALT_LENGTH);
            for (i = 0; i < (int) iv_length; i++)
                iv[i] = salt[i] ^ user->privKey[iv_length + i];
        }
#ifdef HAVE_AES
        else if (ISTRANSFORM(user->privProtocol, AES128Priv) ||
                 ISTRANSFORM(user->privProtocol, AES192Priv) ||
                 ISTRANSFORM(user->privProtocol, AES256Priv)) {
            iv_length = BYTESIZE(USM_AES_SALT_LENGTH);
            net_boots = ntohl(boots_uint);
            net_time = ntohl(time_uint);
            memcpy(iv, &net_boots, 4);
            memcpy(iv+4, &net_time, 4);
            memcpy(iv+8, salt, salt_length);
        }
#endif
        
        if (sc_decrypt(user->privProtocol, user->privProtocolLen,
                       user->privKey, user->privKeyLen,
                       iv, iv_length,
                       value_ptr, remaining, *scopedPdu, scopedPduLen)
            != SNMP_ERR_NOERROR) {
            DEBUGMSGTL(("usm", "%s\n", "Failed decryption."));
            if (snmp_increment_statistic
                (STAT_USMSTATSDECRYPTIONERRORS) == 0) {
                DEBUGMSGTL(("usm", "%s\n", "Failed increment statistic."));
            }
            return SNMPERR_USM_DECRYPTIONERROR;
        }
#ifdef SNMP_TESTING_CODE
        if (debug_is_token_registered("usm/dump") == SNMPERR_SUCCESS) {
            dump_chunk("usm/dump", "Cypher Text", value_ptr, remaining);
            dump_chunk("usm/dump", "salt + Encrypted form:",
                       salt, salt_length);
            dump_chunk("usm/dump", "IV + Encrypted form:", iv, iv_length);
            dump_chunk("usm/dump", "Decrypted chunk:",
                       *scopedPdu, *scopedPduLen);
        }
#endif
    }
    /*
     * sPDU is plaintext.
     */
    else {
        *scopedPdu = data_ptr;
        *scopedPduLen = wholeMsgLen - (data_ptr - wholeMsg);
        end_of_overhead = data_ptr;

    }                           /* endif -- PDU decryption */


    /*
     * Calculate the biggest sPDU for the response (i.e., whole - ovrhd).
     *
     * FIX  Correct? 
     */
    *maxSizeResponse = maxMsgSize - (int)
        ((u_long) end_of_overhead - (u_long) wholeMsg);


    DEBUGMSGTL(("usm", "USM processing completed.\n"));

    return SNMPERR_SUCCESS;

}                               /* end usm_process_in_msg() */

void
init_usm(void)
{
    struct snmp_secmod_def *def;

    DEBUGMSGTL(("init_usm", "unit_usm: %d %d\n", usmNoPrivProtocol[0],
                usmNoPrivProtocol[1]));

    sc_init();                  /* initalize scapi code */

    /*
     * register ourselves as a security service 
     */
    def = SNMP_MALLOC_STRUCT(snmp_secmod_def);
    /*
     * XXX: def->init_sess_secmod move stuff from snmp_api.c 
     */
    def->encode_reverse = usm_secmod_rgenerate_out_msg;
    def->encode_forward = usm_secmod_generate_out_msg;
    def->decode = usm_secmod_process_in_msg;
    def->pdu_free_state_ref = usm_free_usmStateReference;
    register_sec_mod(USM_SEC_MODEL_NUMBER, "usm", def);

    snmp_register_callback(SNMP_CALLBACK_LIBRARY,
                           SNMP_CALLBACK_POST_PREMIB_READ_CONFIG,
                           init_usm_post_config, NULL);
}

/*
 * initializations for the USM.
 *
 * Should be called after the (engineid) configuration files have been read.
 *
 * Set "arbitrary" portion of salt to a random number.
 */
int
init_usm_post_config(int majorid, int minorid, void *serverarg,
                     void *clientarg)
{
    size_t          salt_integer_len = sizeof(salt_integer);

    if (sc_random((u_char *) & salt_integer, &salt_integer_len) !=
        SNMPERR_SUCCESS) {
        DEBUGMSGTL(("usm", "sc_random() failed: using time() as salt.\n"));
        salt_integer = (u_int) time(NULL);
    }

#ifdef HAVE_AES
    salt_integer_len = sizeof (salt_integer64_1);
    if (sc_random((u_char *) & salt_integer64_1, &salt_integer_len) !=
        SNMPERR_SUCCESS) {
        DEBUGMSGTL(("usm", "sc_random() failed: using time() as aes1 salt.\n"));
        salt_integer64_1 = (u_int) time(NULL);
    }
    salt_integer_len = sizeof (salt_integer64_1);
    if (sc_random((u_char *) & salt_integer64_2, &salt_integer_len) !=
        SNMPERR_SUCCESS) {
        DEBUGMSGTL(("usm", "sc_random() failed: using time() as aes2 salt.\n"));
        salt_integer64_2 = (u_int) time(NULL);
    }
#endif
    
    noNameUser = usm_create_initial_user("", usmHMACMD5AuthProtocol,
                                         USM_LENGTH_OID_TRANSFORM,
                                         usmDESPrivProtocol,
                                         USM_LENGTH_OID_TRANSFORM);

    SNMP_FREE(noNameUser->engineID);
    noNameUser->engineIDLen = 0;

    return SNMPERR_SUCCESS;
}                               /* end init_usm_post_config() */


/*******************************************************************-o-******
 * usm_check_secLevel
 *
 * Parameters:
 *	 level
 *	*user
 *      
 * Returns:
 *	0	On success,
 *	-1	Otherwise.
 *
 * Checks that a given security level is valid for a given user.
 */
int
usm_check_secLevel(int level, struct usmUser *user)
{

    DEBUGMSGTL(("comparex", "Comparing: %d %d ", usmNoPrivProtocol[0],
                usmNoPrivProtocol[1]));
    DEBUGMSGOID(("comparex", usmNoPrivProtocol,
                 sizeof(usmNoPrivProtocol) / sizeof(oid)));
    DEBUGMSG(("comparex", "\n"));
    if (level == SNMP_SEC_LEVEL_AUTHPRIV
        && (netsnmp_oid_equals(user->privProtocol, user->privProtocolLen,
                             usmNoPrivProtocol,
                             sizeof(usmNoPrivProtocol) / sizeof(oid)) ==
            0)) {
        DEBUGMSGTL(("usm", "Level: %d\n", level));
        DEBUGMSGTL(("usm", "User (%s) Auth Protocol: ", user->name));
        DEBUGMSGOID(("usm", user->authProtocol, user->authProtocolLen));
        DEBUGMSG(("usm", ", User Priv Protocol: "));
        DEBUGMSGOID(("usm", user->privProtocol, user->privProtocolLen));
        DEBUGMSG(("usm", "\n"));
        return 1;
    }
    if ((level == SNMP_SEC_LEVEL_AUTHPRIV
         || level == SNMP_SEC_LEVEL_AUTHNOPRIV)
        &&
        (netsnmp_oid_equals
         (user->authProtocol, user->authProtocolLen, usmNoAuthProtocol,
          sizeof(usmNoAuthProtocol) / sizeof(oid)) == 0)) {
        DEBUGMSGTL(("usm", "Level: %d\n", level));
        DEBUGMSGTL(("usm", "User (%s) Auth Protocol: ", user->name));
        DEBUGMSGOID(("usm", user->authProtocol, user->authProtocolLen));
        DEBUGMSG(("usm", ", User Priv Protocol: "));
        DEBUGMSGOID(("usm", user->privProtocol, user->privProtocolLen));
        DEBUGMSG(("usm", "\n"));
        return 1;
    }

    return 0;

}                               /* end usm_check_secLevel() */




/*******************************************************************-o-******
 * usm_check_secLevel_vs_protocols
 *
 * Parameters:
 *	 level
 *	*authProtocol
 *	 authProtocolLen
 *	*privProtocol
 *	 privProtocolLen
 *      
 * Returns:
 *	0	On success,
 *	1	Otherwise.
 *
 * Same as above but with explicitly named transform types instead of taking
 * from the usmUser structure.
 */
int
usm_check_secLevel_vs_protocols(int level,
                                const oid * authProtocol,
                                u_int authProtocolLen,
                                const oid * privProtocol,
                                u_int privProtocolLen)
{

    if (level == SNMP_SEC_LEVEL_AUTHPRIV
        &&
        (netsnmp_oid_equals
         (privProtocol, privProtocolLen, usmNoPrivProtocol,
          sizeof(usmNoPrivProtocol) / sizeof(oid)) == 0)) {
        DEBUGMSGTL(("usm", "Level: %d\n", level));
        DEBUGMSGTL(("usm", "Auth Protocol: "));
        DEBUGMSGOID(("usm", authProtocol, authProtocolLen));
        DEBUGMSG(("usm", ", Priv Protocol: "));
        DEBUGMSGOID(("usm", privProtocol, privProtocolLen));
        DEBUGMSG(("usm", "\n"));
        return 1;
    }
    if ((level == SNMP_SEC_LEVEL_AUTHPRIV
         || level == SNMP_SEC_LEVEL_AUTHNOPRIV)
        &&
        (netsnmp_oid_equals
         (authProtocol, authProtocolLen, usmNoAuthProtocol,
          sizeof(usmNoAuthProtocol) / sizeof(oid)) == 0)) {
        DEBUGMSGTL(("usm", "Level: %d\n", level));
        DEBUGMSGTL(("usm", "Auth Protocol: "));
        DEBUGMSGOID(("usm", authProtocol, authProtocolLen));
        DEBUGMSG(("usm", ", Priv Protocol: "));
        DEBUGMSGOID(("usm", privProtocol, privProtocolLen));
        DEBUGMSG(("usm", "\n"));
        return 1;
    }

    return 0;

}                               /* end usm_check_secLevel_vs_protocols() */




/*
 * usm_get_user(): Returns a user from userList based on the engineID,
 * engineIDLen and name of the requested user. 
 */

struct usmUser *
usm_get_user(u_char * engineID, size_t engineIDLen, char *name)
{
    DEBUGMSGTL(("usm", "getting user %s\n", name));
    return usm_get_user_from_list(engineID, engineIDLen, name, userList,
                                  1);
}

struct usmUser *
usm_get_user_from_list(u_char * engineID, size_t engineIDLen,
                       char *name, struct usmUser *puserList,
                       int use_default)
{
    struct usmUser *ptr;
    char            noName[] = "";
    if (name == NULL)
        name = noName;
    for (ptr = puserList; ptr != NULL; ptr = ptr->next) {
        if (!strcmp(ptr->name, name) &&
            ptr->engineIDLen == engineIDLen &&
            ((ptr->engineID == NULL && engineID == NULL) ||
             (ptr->engineID != NULL && engineID != NULL &&
              memcmp(ptr->engineID, engineID, engineIDLen) == 0)))
            return ptr;
    }

    /*
     * return "" user used to facilitate engineID discovery 
     */
    if (use_default && !strcmp(name, ""))
        return noNameUser;
    return NULL;
}

/*
 * usm_add_user(): Add's a user to the userList, sorted by the
 * engineIDLength then the engineID then the name length then the name
 * to facilitate getNext calls on a usmUser table which is indexed by
 * these values.
 * 
 * Note: userList must not be NULL (obviously), as thats a rather trivial
 * addition and is left to the API user.
 * 
 * returns the head of the list (which could change due to this add).
 */

struct usmUser *
usm_add_user(struct usmUser *user)
{
    struct usmUser *uptr;
    uptr = usm_add_user_to_list(user, userList);
    if (uptr != NULL)
        userList = uptr;
    return uptr;
}

struct usmUser *
usm_add_user_to_list(struct usmUser *user, struct usmUser *puserList)
{
    struct usmUser *nptr, *pptr, *optr;

    /*
     * loop through puserList till we find the proper, sorted place to
     * insert the new user 
     */
    for (nptr = puserList, pptr = NULL; nptr != NULL;
         pptr = nptr, nptr = nptr->next) {
        if (nptr->engineIDLen > user->engineIDLen)
            break;

        if (user->engineID == NULL && nptr->engineID != NULL)
            break;

        if (nptr->engineIDLen == user->engineIDLen &&
            (nptr->engineID != NULL && user->engineID != NULL &&
             memcmp(nptr->engineID, user->engineID,
                    user->engineIDLen) > 0))
            break;

        if (!(nptr->engineID == NULL && user->engineID != NULL)) {
            if (nptr->engineIDLen == user->engineIDLen &&
                ((nptr->engineID == NULL && user->engineID == NULL) ||
                 memcmp(nptr->engineID, user->engineID,
                        user->engineIDLen) == 0)
                && strlen(nptr->name) > strlen(user->name))
                break;

            if (nptr->engineIDLen == user->engineIDLen &&
                ((nptr->engineID == NULL && user->engineID == NULL) ||
                 memcmp(nptr->engineID, user->engineID,
                        user->engineIDLen) == 0)
                && strlen(nptr->name) == strlen(user->name)
                && strcmp(nptr->name, user->name) > 0)
                break;

            if (nptr->engineIDLen == user->engineIDLen &&
                ((nptr->engineID == NULL && user->engineID == NULL) ||
                 memcmp(nptr->engineID, user->engineID,
                        user->engineIDLen) == 0)
                && strlen(nptr->name) == strlen(user->name)
                && strcmp(nptr->name, user->name) == 0) {
                /*
                 * the user is an exact match of a previous entry.
                 * Credentials may be different, though, so remove
                 * the old entry (and add the new one)!
                 */
                if (pptr) { /* change prev's next pointer */
                  pptr->next = nptr->next;
                }
                if (nptr->next) { /* change next's prev pointer */
                  nptr->next->prev = pptr;
                } 
                optr = nptr;
                nptr = optr->next; /* add new user at this position */
                /* free the old user */
                optr->next=NULL;
                optr->prev=NULL;
                usm_free_user(optr); 
                break; /* new user will be added below */
            }
        }
    }

    /*
     * nptr should now point to the user that we need to add ourselves
     * in front of, and pptr should be our new 'prev'. 
     */

    /*
     * change our pointers 
     */
    user->prev = pptr;
    user->next = nptr;

    /*
     * change the next's prev pointer 
     */
    if (user->next)
        user->next->prev = user;

    /*
     * change the prev's next pointer 
     */
    if (user->prev)
        user->prev->next = user;

    /*
     * rewind to the head of the list and return it (since the new head
     * could be us, we need to notify the above routine who the head now is. 
     */
    for (pptr = user; pptr->prev != NULL; pptr = pptr->prev);
    return pptr;
}

/*
 * usm_remove_user(): finds and removes a user from a list 
 */
struct usmUser *
usm_remove_user(struct usmUser *user)
{
    return usm_remove_user_from_list(user, &userList);
}

struct usmUser *
usm_remove_user_from_list(struct usmUser *user,
                          struct usmUser **ppuserList)
{
    struct usmUser *nptr, *pptr;

    /*
     * NULL pointers aren't allowed 
     */
    if (ppuserList == NULL)
        return NULL;

    /*
     * find the user in the list 
     */
    for (nptr = *ppuserList, pptr = NULL; nptr != NULL;
         pptr = nptr, nptr = nptr->next) {
        if (nptr == user)
            break;
    }

    if (nptr) {
        /*
         * remove the user from the linked list 
         */
        if (pptr) {
            pptr->next = nptr->next;
        }
        if (nptr->next) {
            nptr->next->prev = pptr;
        }
    } else {
        /*
         * user didn't exist 
         */
        return NULL;
    }
    if (nptr == *ppuserList)    /* we're the head of the list, need to change
                                 * * the head to the next user */
        *ppuserList = nptr->next;
    return *ppuserList;
}                               /* end usm_remove_user_from_list() */




/*
 * usm_free_user():  calls free() on all needed parts of struct usmUser and
 * the user himself.
 * 
 * Note: This should *not* be called on an object in a list (IE,
 * remove it from the list first, and set next and prev to NULL), but
 * will try to reconnect the list pieces again if it is called this
 * way.  If called on the head of the list, the entire list will be
 * lost. 
 */
struct usmUser *
usm_free_user(struct usmUser *user)
{
    if (user == NULL)
        return NULL;

    SNMP_FREE(user->engineID);
    SNMP_FREE(user->name);
    SNMP_FREE(user->secName);
    SNMP_FREE(user->cloneFrom);
    SNMP_FREE(user->userPublicString);
    SNMP_FREE(user->authProtocol);
    SNMP_FREE(user->privProtocol);

    if (user->authKey != NULL) {
        SNMP_ZERO(user->authKey, user->authKeyLen);
        SNMP_FREE(user->authKey);
    }

    if (user->privKey != NULL) {
        SNMP_ZERO(user->privKey, user->privKeyLen);
        SNMP_FREE(user->privKey);
    }


    /*
     * FIX  Why not put this check *first?*
     */
    if (user->prev != NULL) {   /* ack, this shouldn't happen */
        user->prev->next = user->next;
    }
    if (user->next != NULL) {
        user->next->prev = user->prev;
        if (user->prev != NULL) /* ack this is really bad, because it means
                                 * * we'll loose the head of some structure tree */
            DEBUGMSGTL(("usm",
                        "Severe: Asked to free the head of a usmUser tree somewhere."));
    }


    SNMP_ZERO(user, sizeof(*user));
    SNMP_FREE(user);

    return NULL;                /* for convenience to returns from calling functions */

}                               /* end usm_free_user() */




/*
 * take a given user and clone the security info into another 
 */
struct usmUser *
usm_cloneFrom_user(struct usmUser *from, struct usmUser *to)
{
    /*
     * copy the authProtocol oid row pointer 
     */
    SNMP_FREE(to->authProtocol);

    if ((to->authProtocol =
         snmp_duplicate_objid(from->authProtocol,
                              from->authProtocolLen)) != NULL)
        to->authProtocolLen = from->authProtocolLen;
    else
        to->authProtocolLen = 0;


    /*
     * copy the authKey 
     */
    SNMP_FREE(to->authKey);

    if (from->authKeyLen > 0 &&
        (to->authKey = (u_char *) malloc(from->authKeyLen))
        != NULL) {
        to->authKeyLen = from->authKeyLen;
        memcpy(to->authKey, from->authKey, to->authKeyLen);
    } else {
        to->authKey = NULL;
        to->authKeyLen = 0;
    }


    /*
     * copy the privProtocol oid row pointer 
     */
    SNMP_FREE(to->privProtocol);

    if ((to->privProtocol =
         snmp_duplicate_objid(from->privProtocol,
                              from->privProtocolLen)) != NULL)
        to->privProtocolLen = from->privProtocolLen;
    else
        to->privProtocolLen = 0;

    /*
     * copy the privKey 
     */
    SNMP_FREE(to->privKey);

    if (from->privKeyLen > 0 &&
        (to->privKey = (u_char *) malloc(from->privKeyLen))
        != NULL) {
        to->privKeyLen = from->privKeyLen;
        memcpy(to->privKey, from->privKey, to->privKeyLen);
    } else {
        to->privKey = NULL;
        to->privKeyLen = 0;
    }
    return to;
}

/*
 * usm_create_user(void):
 * create a default empty user, instantiating only the auth/priv
 * protocols to noAuth and noPriv OID pointers
 */
struct usmUser *
usm_create_user(void)
{
    struct usmUser *newUser;

    /*
     * create the new user 
     */
    newUser = (struct usmUser *) calloc(1, sizeof(struct usmUser));
    if (newUser == NULL)
        return NULL;

    /*
     * fill the auth/priv protocols 
     */
    if ((newUser->authProtocol =
         snmp_duplicate_objid(usmNoAuthProtocol,
                              sizeof(usmNoAuthProtocol) / sizeof(oid))) ==
        NULL)
        return usm_free_user(newUser);
    newUser->authProtocolLen = sizeof(usmNoAuthProtocol) / sizeof(oid);

    if ((newUser->privProtocol =
         snmp_duplicate_objid(usmNoPrivProtocol,
                              sizeof(usmNoPrivProtocol) / sizeof(oid))) ==
        NULL)
        return usm_free_user(newUser);
    newUser->privProtocolLen = sizeof(usmNoPrivProtocol) / sizeof(oid);

    /*
     * set the storage type to nonvolatile, and the status to ACTIVE 
     */
    newUser->userStorageType = ST_NONVOLATILE;
    newUser->userStatus = RS_ACTIVE;
    return newUser;

}                               /* end usm_clone_user() */




/*
 * usm_create_initial_user(void):
 * creates an initial user, filled with the defaults defined in the
 * USM document.
 */
struct usmUser *
usm_create_initial_user(const char *name,
                        const oid * authProtocol, size_t authProtocolLen,
                        const oid * privProtocol, size_t privProtocolLen)
{
    struct usmUser *newUser = usm_create_user();
    if (newUser == NULL)
        return NULL;

    if ((newUser->name = strdup(name)) == NULL)
        return usm_free_user(newUser);

    if ((newUser->secName = strdup(name)) == NULL)
        return usm_free_user(newUser);

    if ((newUser->engineID =
         snmpv3_generate_engineID(&newUser->engineIDLen)) == NULL)
        return usm_free_user(newUser);

    if ((newUser->cloneFrom = (oid *) malloc(sizeof(oid) * 2)) == NULL)
        return usm_free_user(newUser);
    newUser->cloneFrom[0] = 0;
    newUser->cloneFrom[1] = 0;
    newUser->cloneFromLen = 2;

    SNMP_FREE(newUser->privProtocol);
    if ((newUser->privProtocol = snmp_duplicate_objid(privProtocol,
                                                      privProtocolLen)) ==
        NULL) {
        return usm_free_user(newUser);
    }
    newUser->privProtocolLen = privProtocolLen;

    SNMP_FREE(newUser->authProtocol);
    if ((newUser->authProtocol = snmp_duplicate_objid(authProtocol,
                                                      authProtocolLen)) ==
        NULL) {
        return usm_free_user(newUser);
    }
    newUser->authProtocolLen = authProtocolLen;

    newUser->userStatus = RS_ACTIVE;
    newUser->userStorageType = ST_READONLY;

    return newUser;
}

/*
 * this is a callback that can store all known users based on a
 * previously registered application ID 
 */
int
usm_store_users(int majorID, int minorID, void *serverarg, void *clientarg)
{
    /*
     * figure out our application name 
     */
    char           *appname = (char *) clientarg;
    if (appname == NULL) {
        appname = netsnmp_ds_get_string(NETSNMP_DS_LIBRARY_ID, 
					NETSNMP_DS_LIB_APPTYPE);
    }

    /*
     * save the user base 
     */
    usm_save_users("usmUser", appname);

    /*
     * never fails 
     */
    return SNMPERR_SUCCESS;
}


/*
 * usm_save_users(): saves a list of users to the persistent cache 
 */
void
usm_save_users(const char *token, const char *type)
{
    usm_save_users_from_list(userList, token, type);
}

void
usm_save_users_from_list(struct usmUser *puserList, const char *token,
                         const char *type)
{
    struct usmUser *uptr;
    for (uptr = puserList; uptr != NULL; uptr = uptr->next) {
        if (uptr->userStorageType == ST_NONVOLATILE)
            usm_save_user(uptr, token, type);
    }
}

/*
 * usm_save_user(): saves a user to the persistent cache 
 */
void
usm_save_user(struct usmUser *user, const char *token, const char *type)
{
    char            line[4096];
    char           *cptr;

    memset(line, 0, sizeof(line));

    sprintf(line, "%s %d %d ", token, user->userStatus,
            user->userStorageType);
    cptr = &line[strlen(line)]; /* the NULL */
    cptr =
        read_config_save_octet_string(cptr, user->engineID,
                                      user->engineIDLen);
    *cptr++ = ' ';
    cptr = read_config_save_octet_string(cptr, (u_char *) user->name,
                                         (user->name == NULL) ? 0 :
                                         strlen(user->name) + 1);
    *cptr++ = ' ';
    cptr = read_config_save_octet_string(cptr, (u_char *) user->secName,
                                         (user->secName == NULL) ? 0 :
                                         strlen(user->secName) + 1);
    *cptr++ = ' ';
    cptr =
        read_config_save_objid(cptr, user->cloneFrom, user->cloneFromLen);
    *cptr++ = ' ';
    cptr = read_config_save_objid(cptr, user->authProtocol,
                                  user->authProtocolLen);
    *cptr++ = ' ';
    cptr =
        read_config_save_octet_string(cptr, user->authKey,
                                      user->authKeyLen);
    *cptr++ = ' ';
    cptr = read_config_save_objid(cptr, user->privProtocol,
                                  user->privProtocolLen);
    *cptr++ = ' ';
    cptr =
        read_config_save_octet_string(cptr, user->privKey,
                                      user->privKeyLen);
    *cptr++ = ' ';
    cptr = read_config_save_octet_string(cptr, user->userPublicString,
                                         (user->userPublicString ==
                                          NULL) ? 0 : strlen((char *)
                                                             user->
                                                             userPublicString)
                                         + 1);
    read_config_store(type, line);
}

/*
 * usm_parse_user(): reads in a line containing a saved user profile
 * and returns a pointer to a newly created struct usmUser. 
 */
struct usmUser *
usm_read_user(char *line)
{
    struct usmUser *user;
    size_t          len;

    user = usm_create_user();
    if (user == NULL)
        return NULL;

    user->userStatus = atoi(line);
    line = skip_token(line);
    user->userStorageType = atoi(line);
    line = skip_token(line);
    line = read_config_read_octet_string(line, &user->engineID,
                                         &user->engineIDLen);

    /*
     * set the lcd entry for this engineID to the minimum boots/time
     * values so that its a known engineid and won't return a report pdu.
     * This is mostly important when receiving v3 traps so that the usm
     * will at least continue processing them. 
     */
    set_enginetime(user->engineID, user->engineIDLen, 1, 0, 0);

    line = read_config_read_octet_string(line, (u_char **) & user->name,
                                         &len);
    line = read_config_read_octet_string(line, (u_char **) & user->secName,
                                         &len);
    SNMP_FREE(user->cloneFrom);
    user->cloneFromLen = 0;

    line =
        read_config_read_objid(line, &user->cloneFrom,
                               &user->cloneFromLen);

    SNMP_FREE(user->authProtocol);
    user->authProtocolLen = 0;

    line = read_config_read_objid(line, &user->authProtocol,
                                  &user->authProtocolLen);
    line = read_config_read_octet_string(line, &user->authKey,
                                         &user->authKeyLen);
    SNMP_FREE(user->privProtocol);
    user->privProtocolLen = 0;

    line = read_config_read_objid(line, &user->privProtocol,
                                  &user->privProtocolLen);
    line = read_config_read_octet_string(line, &user->privKey,
                                         &user->privKeyLen);
    line = read_config_read_octet_string(line, &user->userPublicString,
                                         &len);
    return user;
}

/*
 * snmpd.conf parsing routines 
 */
void
usm_parse_config_usmUser(const char *token, char *line)
{
    struct usmUser *uptr;

    uptr = usm_read_user(line);
    usm_add_user(uptr);
}




/*******************************************************************-o-******
 * usm_set_password
 *
 * Parameters:
 *	*token
 *	*line
 *      
 *
 * format: userSetAuthPass     secname engineIDLen engineID pass
 *     or: userSetPrivPass     secname engineIDLen engineID pass 
 *     or: userSetAuthKey      secname engineIDLen engineID KuLen Ku
 *     or: userSetPrivKey      secname engineIDLen engineID KuLen Ku 
 *     or: userSetAuthLocalKey secname engineIDLen engineID KulLen Kul
 *     or: userSetPrivLocalKey secname engineIDLen engineID KulLen Kul 
 *
 * type is:	1=passphrase; 2=Ku; 3=Kul.
 *
 *
 * ASSUMES  Passwords are null-terminated printable strings.
 */
void
usm_set_password(const char *token, char *line)
{
    char           *cp;
    char            nameBuf[SNMP_MAXBUF];
    u_char         *engineID;
    size_t          engineIDLen;
    struct usmUser *user;

    cp = copy_nword(line, nameBuf, sizeof(nameBuf));
    if (cp == NULL) {
        config_perror("invalid name specifier");
        return;
    }

    DEBUGMSGTL(("usm", "comparing: %s and %s\n", cp, WILDCARDSTRING));
    if (strncmp(cp, WILDCARDSTRING, strlen(WILDCARDSTRING)) == 0) {
        /*
         * match against all engineIDs we know about 
         */
        cp = skip_token(cp);
        for (user = userList; user != NULL; user = user->next) {
            if (strcmp(user->secName, nameBuf) == 0) {
                usm_set_user_password(user, token, cp);
            }
        }
    } else {
        cp = read_config_read_octet_string(cp, &engineID, &engineIDLen);
        if (cp == NULL) {
            config_perror("invalid engineID specifier");
            return;
        }

        user = usm_get_user(engineID, engineIDLen, nameBuf);
        if (user == NULL) {
            config_perror("not a valid user/engineID pair");
            return;
        }
        usm_set_user_password(user, token, cp);
    }
}

/*
 * uses the rest of LINE to configure USER's password of type TOKEN 
 */
void
usm_set_user_password(struct usmUser *user, const char *token, char *line)
{
    char           *cp = line;
    u_char         *engineID = user->engineID;
    size_t          engineIDLen = user->engineIDLen;

    u_char        **key;
    size_t         *keyLen;
    u_char          userKey[SNMP_MAXBUF_SMALL];
    size_t          userKeyLen = SNMP_MAXBUF_SMALL;
    u_char         *userKeyP = userKey;
    int             type, ret;

    /*
     * Retrieve the "old" key and set the key type.
     */
    if (strcmp(token, "userSetAuthPass") == 0) {
        key = &user->authKey;
        keyLen = &user->authKeyLen;
        type = 0;
    } else if (strcmp(token, "userSetPrivPass") == 0) {
        key = &user->privKey;
        keyLen = &user->privKeyLen;
        type = 0;
    } else if (strcmp(token, "userSetAuthKey") == 0) {
        key = &user->authKey;
        keyLen = &user->authKeyLen;
        type = 1;
    } else if (strcmp(token, "userSetPrivKey") == 0) {
        key = &user->privKey;
        keyLen = &user->privKeyLen;
        type = 1;
    } else if (strcmp(token, "userSetAuthLocalKey") == 0) {
        key = &user->authKey;
        keyLen = &user->authKeyLen;
        type = 2;
    } else if (strcmp(token, "userSetPrivLocalKey") == 0) {
        key = &user->privKey;
        keyLen = &user->privKeyLen;
        type = 2;
    } else {
        /*
         * no old key, or token was not recognized 
         */
        return;
    }

    if (*key) {
        /*
         * (destroy and) free the old key 
         */
        memset(*key, 0, *keyLen);
        free(*key);
    }

    if (type == 0) {
        /*
         * convert the password into a key 
         */
        ret = generate_Ku(user->authProtocol, user->authProtocolLen,
                          (u_char *) cp, strlen(cp), userKey, &userKeyLen);

        if (ret != SNMPERR_SUCCESS) {
            config_perror("setting key failed (in sc_genKu())");
            return;
        }
    } else if (type == 1) {
        cp = read_config_read_octet_string(cp, &userKeyP, &userKeyLen);

        if (cp == NULL) {
            config_perror("invalid user key");
            return;
        }
    }

    if (type < 2) {
        *key = (u_char *) malloc(SNMP_MAXBUF_SMALL);
        *keyLen = SNMP_MAXBUF_SMALL;
        ret = generate_kul(user->authProtocol, user->authProtocolLen,
                           engineID, engineIDLen,
                           userKey, userKeyLen, *key, keyLen);
        if (ret != SNMPERR_SUCCESS) {
            config_perror("setting key failed (in generate_kul())");
            return;
        }

        /*
         * (destroy and) free the old key 
         */
        memset(userKey, 0, sizeof(userKey));

    } else {
        /*
         * the key is given, copy it in 
         */
        cp = read_config_read_octet_string(cp, key, keyLen);

        if (cp == NULL) {
            config_perror("invalid localized user key");
            return;
        }
    }
}                               /* end usm_set_password() */
