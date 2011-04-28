#ifndef _INCLUDE_ADS_H_
#define _INCLUDE_ADS_H_
/*
  header for ads (active directory) library routines

  basically this is a wrapper around ldap
*/

enum wb_posix_mapping {
	WB_POSIX_MAP_UNKNOWN    = -1,
	WB_POSIX_MAP_TEMPLATE 	= 0, 
	WB_POSIX_MAP_SFU 	= 1, 
	WB_POSIX_MAP_SFU20 	= 2, 
	WB_POSIX_MAP_RFC2307 	= 3,
	WB_POSIX_MAP_UNIXINFO	= 4
};

/* there are 5 possible types of errors the ads subsystem can produce */
enum ads_error_type {ENUM_ADS_ERROR_KRB5, ENUM_ADS_ERROR_GSS, 
		     ENUM_ADS_ERROR_LDAP, ENUM_ADS_ERROR_SYSTEM, ENUM_ADS_ERROR_NT};

typedef struct {
	enum ads_error_type error_type;
	union err_state{		
		int rc;
		NTSTATUS nt_status;
	} err;
	/* For error_type = ENUM_ADS_ERROR_GSS minor_status describe GSS API error */
	/* Where rc represents major_status of GSS API error */
	int minor_status;
} ADS_STATUS;

struct ads_struct;

struct ads_saslwrap_ops {
	const char *name;
	ADS_STATUS (*wrap)(struct ads_struct *, uint8 *buf, uint32 len);
	ADS_STATUS (*unwrap)(struct ads_struct *);
	void (*disconnect)(struct ads_struct *);
};

enum ads_saslwrap_type {
	ADS_SASLWRAP_TYPE_PLAIN = 1,
	ADS_SASLWRAP_TYPE_SIGN = 2,
	ADS_SASLWRAP_TYPE_SEAL = 4
};

typedef struct ads_struct {
	int is_mine;	/* do I own this structure's memory? */
	
	/* info needed to find the server */
	struct {
		char *realm;
		char *workgroup;
		char *ldap_server;
		int foreign; /* set to 1 if connecting to a foreign
			      * realm */
		bool gc;     /* Is this a global catalog server? */
	} server;

	/* info needed to authenticate */
	struct {
		char *realm;
		char *password;
		char *user_name;
		char *kdc_server;
		unsigned flags;
		int time_offset;
		time_t tgt_expire;
		time_t tgs_expire;
		time_t renewable;
	} auth;

	/* info derived from the servers config */
	struct {
		uint32 flags; /* cldap flags identifying the services. */
		char *realm;
		char *bind_path;
		char *ldap_server_name;
		char *server_site_name;
		char *client_site_name;
		time_t current_time;
		char *schema_path;
		char *config_path;
	} config;

	/* info about the current LDAP connection */
#ifdef HAVE_LDAP
	struct {
		LDAP *ld;
		struct sockaddr_storage ss; /* the ip of the active connection, if any */
		time_t last_attempt; /* last attempt to reconnect */
		int port;

		enum ads_saslwrap_type wrap_type;

#ifdef HAVE_LDAP_SASL_WRAPPING
		Sockbuf_IO_Desc *sbiod; /* lowlevel state for LDAP wrapping */
#endif /* HAVE_LDAP_SASL_WRAPPING */
		TALLOC_CTX *mem_ctx;
		const struct ads_saslwrap_ops *wrap_ops;
		void *wrap_private_data;
		struct {
			uint32 ofs;
			uint32 needed;
			uint32 left;
#define        ADS_SASL_WRAPPING_IN_MAX_WRAPPED        0x0FFFFFFF
			uint32 max_wrapped;
			uint32 min_wrapped;
			uint32 size;
			uint8 *buf;
		} in;
		struct {
			uint32 ofs;
			uint32 left;
#define        ADS_SASL_WRAPPING_OUT_MAX_WRAPPED       0x00A00000
			uint32 max_unwrapped;
			uint32 sig_size;
			uint32 size;
			uint8 *buf;
		} out;
	} ldap;
#endif /* HAVE_LDAP */
} ADS_STRUCT;

/* used to remember the names of the posix attributes in AD */
/* see the rfc2307 & sfu nss backends */

struct posix_schema {
	char *posix_homedir_attr;
	char *posix_shell_attr;
	char *posix_uidnumber_attr;
	char *posix_gidnumber_attr;
	char *posix_gecos_attr;
	char *posix_uid_attr;
};



#ifdef HAVE_ADS
typedef LDAPMod **ADS_MODLIST;
#else
typedef void **ADS_MODLIST;
#endif

/* macros to simplify error returning */
#define ADS_ERROR(rc) ADS_ERROR_LDAP(rc)
#define ADS_ERROR_LDAP(rc) ads_build_error(ENUM_ADS_ERROR_LDAP, rc, 0)
#define ADS_ERROR_SYSTEM(rc) ads_build_error(ENUM_ADS_ERROR_SYSTEM, rc?rc:EINVAL, 0)
#define ADS_ERROR_KRB5(rc) ads_build_error(ENUM_ADS_ERROR_KRB5, rc, 0)
#define ADS_ERROR_GSS(rc, minor) ads_build_error(ENUM_ADS_ERROR_GSS, rc, minor)
#define ADS_ERROR_NT(rc) ads_build_nt_error(ENUM_ADS_ERROR_NT,rc)

#define ADS_ERR_OK(status) ((status.error_type == ENUM_ADS_ERROR_NT) ? NT_STATUS_IS_OK(status.err.nt_status):(status.err.rc == 0))
#define ADS_SUCCESS ADS_ERROR(0)

#define ADS_ERROR_HAVE_NO_MEMORY(x) do { \
        if (!(x)) {\
                return ADS_ERROR(LDAP_NO_MEMORY);\
        }\
} while (0)


/* time between reconnect attempts */
#define ADS_RECONNECT_TIME 5

/* ldap control oids */
#define ADS_PAGE_CTL_OID 	"1.2.840.113556.1.4.319"
#define ADS_NO_REFERRALS_OID 	"1.2.840.113556.1.4.1339"
#define ADS_SERVER_SORT_OID 	"1.2.840.113556.1.4.473"
#define ADS_PERMIT_MODIFY_OID 	"1.2.840.113556.1.4.1413"
#define ADS_ASQ_OID		"1.2.840.113556.1.4.1504"
#define ADS_EXTENDED_DN_OID	"1.2.840.113556.1.4.529"
#define ADS_SD_FLAGS_OID	"1.2.840.113556.1.4.801"

/* ldap attribute oids (Services for Unix 3.0, 3.5) */
#define ADS_ATTR_SFU_UIDNUMBER_OID 	"1.2.840.113556.1.6.18.1.310"
#define ADS_ATTR_SFU_GIDNUMBER_OID 	"1.2.840.113556.1.6.18.1.311"
#define ADS_ATTR_SFU_HOMEDIR_OID 	"1.2.840.113556.1.6.18.1.344"
#define ADS_ATTR_SFU_SHELL_OID 		"1.2.840.113556.1.6.18.1.312"
#define ADS_ATTR_SFU_GECOS_OID 		"1.2.840.113556.1.6.18.1.337"
#define ADS_ATTR_SFU_UID_OID            "1.2.840.113556.1.6.18.1.309"

/* ldap attribute oids (Services for Unix 2.0) */
#define ADS_ATTR_SFU20_UIDNUMBER_OID	"1.2.840.113556.1.4.7000.187.70"
#define ADS_ATTR_SFU20_GIDNUMBER_OID	"1.2.840.113556.1.4.7000.187.71"
#define ADS_ATTR_SFU20_HOMEDIR_OID	"1.2.840.113556.1.4.7000.187.106"
#define ADS_ATTR_SFU20_SHELL_OID	"1.2.840.113556.1.4.7000.187.72"
#define ADS_ATTR_SFU20_GECOS_OID 	"1.2.840.113556.1.4.7000.187.97"
#define ADS_ATTR_SFU20_UID_OID          "1.2.840.113556.1.4.7000.187.102"


/* ldap attribute oids (RFC2307) */
#define ADS_ATTR_RFC2307_UIDNUMBER_OID	"1.3.6.1.1.1.1.0"
#define ADS_ATTR_RFC2307_GIDNUMBER_OID	"1.3.6.1.1.1.1.1"
#define ADS_ATTR_RFC2307_HOMEDIR_OID	"1.3.6.1.1.1.1.3"
#define ADS_ATTR_RFC2307_SHELL_OID	"1.3.6.1.1.1.1.4"
#define ADS_ATTR_RFC2307_GECOS_OID	"1.3.6.1.1.1.1.2"
#define ADS_ATTR_RFC2307_UID_OID        "0.9.2342.19200300.100.1.1"

/* ldap bitwise searches */
#define ADS_LDAP_MATCHING_RULE_BIT_AND	"1.2.840.113556.1.4.803"
#define ADS_LDAP_MATCHING_RULE_BIT_OR	"1.2.840.113556.1.4.804"

/* UserFlags for userAccountControl */
#define UF_SCRIPT	 			0x00000001
#define UF_ACCOUNTDISABLE			0x00000002
#define UF_UNUSED_1	 			0x00000004
#define UF_HOMEDIR_REQUIRED			0x00000008

#define UF_LOCKOUT	 			0x00000010
#define UF_PASSWD_NOTREQD 			0x00000020
#define UF_PASSWD_CANT_CHANGE 			0x00000040
#define UF_ENCRYPTED_TEXT_PASSWORD_ALLOWED	0x00000080

#define UF_TEMP_DUPLICATE_ACCOUNT       	0x00000100
#define UF_NORMAL_ACCOUNT               	0x00000200
#define UF_UNUSED_2	 			0x00000400
#define UF_INTERDOMAIN_TRUST_ACCOUNT    	0x00000800

#define UF_WORKSTATION_TRUST_ACCOUNT    	0x00001000
#define UF_SERVER_TRUST_ACCOUNT         	0x00002000
#define UF_UNUSED_3	 			0x00004000
#define UF_UNUSED_4	 			0x00008000

#define UF_DONT_EXPIRE_PASSWD			0x00010000
#define UF_MNS_LOGON_ACCOUNT			0x00020000
#define UF_SMARTCARD_REQUIRED			0x00040000
#define UF_TRUSTED_FOR_DELEGATION		0x00080000

#define UF_NOT_DELEGATED			0x00100000
#define UF_USE_DES_KEY_ONLY			0x00200000
#define UF_DONT_REQUIRE_PREAUTH			0x00400000
#define UF_PASSWORD_EXPIRED			0x00800000

#define UF_TRUSTED_TO_AUTHENTICATE_FOR_DELEGATION 0x01000000
#define UF_NO_AUTH_DATA_REQUIRED		0x02000000
#define UF_UNUSED_8				0x04000000
#define UF_UNUSED_9				0x08000000

#define UF_UNUSED_10				0x10000000
#define UF_UNUSED_11				0x20000000
#define UF_UNUSED_12				0x40000000
#define UF_UNUSED_13				0x80000000

#define UF_MACHINE_ACCOUNT_MASK (\
		UF_INTERDOMAIN_TRUST_ACCOUNT |\
		UF_WORKSTATION_TRUST_ACCOUNT |\
		UF_SERVER_TRUST_ACCOUNT \
		)

#define UF_ACCOUNT_TYPE_MASK (\
		UF_TEMP_DUPLICATE_ACCOUNT |\
		UF_NORMAL_ACCOUNT |\
		UF_INTERDOMAIN_TRUST_ACCOUNT |\
		UF_WORKSTATION_TRUST_ACCOUNT |\
		UF_SERVER_TRUST_ACCOUNT \
                )

#define UF_SETTABLE_BITS (\
		UF_SCRIPT |\
		UF_ACCOUNTDISABLE |\
		UF_HOMEDIR_REQUIRED  |\
		UF_LOCKOUT |\
		UF_PASSWD_NOTREQD |\
		UF_PASSWD_CANT_CHANGE |\
		UF_ACCOUNT_TYPE_MASK | \
		UF_DONT_EXPIRE_PASSWD | \
		UF_MNS_LOGON_ACCOUNT |\
		UF_ENCRYPTED_TEXT_PASSWORD_ALLOWED |\
		UF_SMARTCARD_REQUIRED |\
		UF_TRUSTED_FOR_DELEGATION |\
		UF_NOT_DELEGATED |\
		UF_USE_DES_KEY_ONLY  |\
		UF_DONT_REQUIRE_PREAUTH \
		)

/* sAMAccountType */
#define ATYPE_NORMAL_ACCOUNT			0x30000000 /* 805306368 */
#define ATYPE_WORKSTATION_TRUST			0x30000001 /* 805306369 */
#define ATYPE_INTERDOMAIN_TRUST			0x30000002 /* 805306370 */ 
#define ATYPE_SECURITY_GLOBAL_GROUP		0x10000000 /* 268435456 */
#define ATYPE_DISTRIBUTION_GLOBAL_GROUP		0x10000001 /* 268435457 */
#define ATYPE_DISTRIBUTION_UNIVERSAL_GROUP 	ATYPE_DISTRIBUTION_GLOBAL_GROUP
#define ATYPE_SECURITY_LOCAL_GROUP		0x20000000 /* 536870912 */
#define ATYPE_DISTRIBUTION_LOCAL_GROUP		0x20000001 /* 536870913 */

#define ATYPE_ACCOUNT		ATYPE_NORMAL_ACCOUNT 		/* 0x30000000 805306368 */
#define ATYPE_GLOBAL_GROUP	ATYPE_SECURITY_GLOBAL_GROUP 	/* 0x10000000 268435456 */
#define ATYPE_LOCAL_GROUP	ATYPE_SECURITY_LOCAL_GROUP 	/* 0x20000000 536870912 */

/* groupType */
#define GROUP_TYPE_BUILTIN_LOCAL_GROUP		0x00000001
#define GROUP_TYPE_ACCOUNT_GROUP		0x00000002
#define GROUP_TYPE_RESOURCE_GROUP		0x00000004
#define GROUP_TYPE_UNIVERSAL_GROUP		0x00000008
#define GROUP_TYPE_APP_BASIC_GROUP		0x00000010
#define GROUP_TYPE_APP_QUERY_GROUP		0x00000020
#define GROUP_TYPE_SECURITY_ENABLED		0x80000000

#define GTYPE_SECURITY_BUILTIN_LOCAL_GROUP ( 	/* 0x80000005 -2147483643 */ \
		GROUP_TYPE_BUILTIN_LOCAL_GROUP| \
		GROUP_TYPE_RESOURCE_GROUP| \
		GROUP_TYPE_SECURITY_ENABLED \
		)
#define GTYPE_SECURITY_DOMAIN_LOCAL_GROUP ( 	/* 0x80000004 -2147483644 */ \
		GROUP_TYPE_RESOURCE_GROUP| \
		GROUP_TYPE_SECURITY_ENABLED \
		)
#define GTYPE_SECURITY_GLOBAL_GROUP ( 		/* 0x80000002 -2147483646 */ \
		GROUP_TYPE_ACCOUNT_GROUP| \
		GROUP_TYPE_SECURITY_ENABLED \
		)
#define GTYPE_SECURITY_UNIVERSAL_GROUP (	/* 0x80000008 -2147483656 */ \
		GROUP_TYPE_UNIVERSAL_GROUP| \
		GROUP_TYPE_SECURITY_ENABLED \
		)

#define GTYPE_DISTRIBUTION_GLOBAL_GROUP		0x00000002	/* 2 */
#define GTYPE_DISTRIBUTION_DOMAIN_LOCAL_GROUP	0x00000004	/* 4 */
#define GTYPE_DISTRIBUTION_UNIVERSAL_GROUP	0x00000008	/* 8 */

#define ADS_PINGS          0x0000FFFF  /* Ping response */
#define ADS_DNS_CONTROLLER 0x20000000  /* DomainControllerName is a DNS name*/
#define ADS_DNS_DOMAIN     0x40000000  /* DomainName is a DNS name */
#define ADS_DNS_FOREST     0x80000000  /* DnsForestName is a DNS name */

/* ads auth control flags */
#define ADS_AUTH_DISABLE_KERBEROS 0x0001
#define ADS_AUTH_NO_BIND          0x0002
#define ADS_AUTH_ANON_BIND        0x0004
#define ADS_AUTH_SIMPLE_BIND      0x0008
#define ADS_AUTH_ALLOW_NTLMSSP    0x0010
#define ADS_AUTH_SASL_SIGN        0x0020
#define ADS_AUTH_SASL_SEAL        0x0040
#define ADS_AUTH_SASL_FORCE       0x0080
#define ADS_AUTH_USER_CREDS       0x0100

/* Kerberos environment variable names */
#define KRB5_ENV_CCNAME "KRB5CCNAME"

#define WELL_KNOWN_GUID_COMPUTERS	"AA312825768811D1ADED00C04FD8D5CD" 
#define WELL_KNOWN_GUID_USERS		"A9D1CA15768811D1ADED00C04FD8D5CD"

enum ads_extended_dn_flags {
	ADS_EXTENDED_DN_HEX_STRING	= 0,
	ADS_EXTENDED_DN_STRING		= 1 /* not supported on win2k */
};

/* this is probably not very well suited to pass other controls generically but
 * is good enough for the extended dn control where it is only used for atm */

typedef struct {
	const char *control;
	int val;
	int critical;
} ads_control;

#define ADS_EXTENDED_RIGHT_APPLY_GROUP_POLICY "edacfd8f-ffb3-11d1-b41d-00a0c968f939"

#define ADS_IGNORE_PRINCIPAL "not_defined_in_RFC4178@please_ignore"

/* Settings for the domainFunctionality attribute in the rootDSE */

#define DS_DOMAIN_FUNCTION_2000		0
#define DS_DOMAIN_FUCNTION_2003_MIXED	1
#define DS_DOMAIN_FUNCTION_2003		2
#define DS_DOMAIN_FUNCTION_2008		3

#endif	/* _INCLUDE_ADS_H_ */
