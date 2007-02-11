#include <Python.h>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#ifdef I_SYS_TIME
#include <sys/time.h>
#endif
#include <netdb.h>
#include <stdlib.h>

#ifdef HAVE_REGEX_H
#include <regex.h>
#endif

#define SUCCESS 1
#define FAILURE 0

#define VARBIND_TAG_F 0
#define VARBIND_IID_F 1
#define VARBIND_VAL_F 2
#define VARBIND_TYPE_F 3

#define TYPE_UNKNOWN 0
#define MAX_TYPE_NAME_LEN 32
#define STR_BUF_SIZE (MAX_TYPE_NAME_LEN * MAX_OID_LEN)
#define ENG_ID_BUF_SIZE 32

#define NO_RETRY_NOSUCH 0

/* these should be part of transform_oids.h ? */
#define USM_AUTH_PROTO_MD5_LEN 10
#define USM_AUTH_PROTO_SHA_LEN 10
#define USM_PRIV_PROTO_DES_LEN 10

#define STRLEN(x) (x ? strlen(x) : 0)

typedef netsnmp_session SnmpSession;
typedef struct tree SnmpMibNode;
static void __recalc_timeout (struct timeval*,struct timeval*,
                                struct timeval*,struct timeval*, int* );
static in_addr_t __parse_address (char*);
static int __is_numeric_oid (char*);
static int __is_leaf (struct tree*);
static int __translate_appl_type (char*);
static int __translate_asn_type (int);
static int __snprint_value (char *, size_t,
                              netsnmp_variable_list*, struct tree *,
                             int, int);
static int __sprint_num_objid (char *, oid *, int);
static int __scan_num_objid (char *, oid *, size_t *);
static int __get_type_str (int, char *);
static int __get_label_iid (char *, char **, char **, int);
static int __oid_cmp (oid *, int, oid *, int);
static int __tp_sprint_num_objid (char*,SnmpMibNode *);
static SnmpMibNode * __get_next_mib_node (SnmpMibNode *);
static struct tree * __oid2tp (oid*, int, struct tree *, int*);
static struct tree * __tag2oid (char *, char *, oid  *, int  *, int *, int);
static int __concat_oid_str (oid *, int *, char *);
static int __add_var_val_str (netsnmp_pdu *, oid *, int, char *,
                                 int, int);
#define USE_NUMERIC_OIDS 0x08
#define NON_LEAF_NAME 0x04
#define USE_LONG_NAMES 0x02
#define FAIL_ON_NULL_IID 0x01
#define NO_FLAGS 0x00


/* Wrapper around fprintf(stderr, ...) for clean and easy debug output. */
static int _debug_level = 0;
#ifdef	DEBUGGING
#define	DBPRT(severity, otherargs)					\
	do {								\
	    if (_debug_level && severity <= _debug_level) {		\
	      (void)printf(otherargs);					\
	    }								\
	} while (/*CONSTCOND*/0)
#else	/* DEBUGGING */
#define	DBPRT(severity, otherargs)	/* Ignore */
#endif	/* DEBUGGING */

#define SAFE_FREE(x) do {if (x != NULL) free(x);} while(/*CONSTCOND*/0)

void
__libraries_init(char *appname)
{
  static int have_inited = 0;

  if (have_inited)
    return;
  have_inited = 1;

  snmp_set_quick_print(1);
  snmp_enable_stderrlog();
  init_snmp(appname);
    
  netsnmp_ds_set_boolean(NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_DONT_BREAKDOWN_OIDS, 1);
  netsnmp_ds_set_int(NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_PRINT_SUFFIX_ONLY, 1);
  netsnmp_ds_set_int(NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_OID_OUTPUT_FORMAT,
		     NETSNMP_OID_OUTPUT_SUFFIX);
}

static void
__recalc_timeout (tvp, ctvp, ltvp, itvp, block)
struct timeval* tvp;
struct timeval* ctvp;
struct timeval* ltvp;
struct timeval* itvp;
int *block;
{
   struct timeval now;

   if (!timerisset(itvp)) return;  /* interval zero means loop forever */
   *block = 0;
   gettimeofday(&now,(struct timezone *)0);

   if (ctvp->tv_sec < 0) { /* first time or callback just fired */
      timersub(&now,ltvp,ctvp);
      timersub(ctvp,itvp,ctvp);
      timersub(itvp,ctvp,ctvp);
      timeradd(ltvp,itvp,ltvp);
   } else {
      timersub(&now,ltvp,ctvp);
      timersub(itvp,ctvp,ctvp);
   }

   /* flag is set for callback but still hasnt fired so set to something
    * small and we will service packets first if there are any ready
    * (also guard against negative timeout - should never happen?)
    */
   if (!timerisset(ctvp) || ctvp->tv_sec < 0 || ctvp->tv_usec < 0) {
      ctvp->tv_sec = 0;
      ctvp->tv_usec = 10;
   }

   /* if snmp timeout > callback timeout or no more requests to process */
   if (timercmp(tvp, ctvp, >) || !timerisset(tvp)) {
      *tvp = *ctvp; /* use the smaller non-zero timeout */
      timerclear(ctvp); /* used as a flag to let callback fire on timeout */
   }
}

static in_addr_t
__parse_address(address)
char *address;
{
    in_addr_t addr;
    struct sockaddr_in saddr;
    struct hostent *hp;

    if ((addr = inet_addr(address)) != -1)
	return addr;
    hp = gethostbyname(address);
    if (hp == NULL){
        return (-1); /* error value */
    } else {
	memcpy(&saddr.sin_addr, hp->h_addr, hp->h_length);
	return saddr.sin_addr.s_addr;
    }

}

static int
__is_numeric_oid (oidstr)
char* oidstr;
{
  if (!oidstr) return 0;
  for (; *oidstr; oidstr++) {
     if (isalpha((int)*oidstr)) return 0;
  }
  return(1);
}

static int
__is_leaf (tp)
struct tree* tp;
{
   char buf[MAX_TYPE_NAME_LEN];
   return (tp && (__get_type_str(tp->type,buf) || 
		  (tp->parent && __get_type_str(tp->parent->type,buf) )));
}

static SnmpMibNode*
__get_next_mib_node (tp)
SnmpMibNode* tp;
{
   /* printf("tp = %lX, parent = %lX, peer = %lX, child = %lX\n",
              tp, tp->parent, tp->next_peer, tp->child_list); */
   if (tp->child_list) return(tp->child_list);
   if (tp->next_peer) return(tp->next_peer);
   if (!tp->parent) return(NULL);
   for (tp = tp->parent; !tp->next_peer; tp = tp->parent) {
      if (!tp->parent) return(NULL);
   }
   return(tp->next_peer);
}

static int
__translate_appl_type(typestr)
char* typestr;
{
	if (typestr == NULL || *typestr == '\0') return TYPE_UNKNOWN;

	if (!strncasecmp(typestr,"INTEGER32",8))
            return(TYPE_INTEGER32);
	if (!strncasecmp(typestr,"INTEGER",3))
            return(TYPE_INTEGER);
	if (!strncasecmp(typestr,"UNSIGNED32",3))
            return(TYPE_UNSIGNED32);
	if (!strcasecmp(typestr,"COUNTER")) /* check all in case counter64 */
            return(TYPE_COUNTER);
	if (!strncasecmp(typestr,"GAUGE",3))
            return(TYPE_GAUGE);
	if (!strncasecmp(typestr,"IPADDR",3))
            return(TYPE_IPADDR);
	if (!strncasecmp(typestr,"OCTETSTR",3))
            return(TYPE_OCTETSTR);
	if (!strncasecmp(typestr,"TICKS",3))
            return(TYPE_TIMETICKS);
	if (!strncasecmp(typestr,"OPAQUE",3))
            return(TYPE_OPAQUE);
	if (!strncasecmp(typestr,"OBJECTID",3))
            return(TYPE_OBJID);
	if (!strncasecmp(typestr,"NETADDR",3))
	    return(TYPE_NETADDR);
	if (!strncasecmp(typestr,"COUNTER64",3))
	    return(TYPE_COUNTER64);
	if (!strncasecmp(typestr,"NULL",3))
	    return(TYPE_NULL);
	if (!strncasecmp(typestr,"BITS",3))
	    return(TYPE_BITSTRING);
	if (!strncasecmp(typestr,"ENDOFMIBVIEW",3))
	    return(SNMP_ENDOFMIBVIEW);
	if (!strncasecmp(typestr,"NOSUCHOBJECT",7))
	    return(SNMP_NOSUCHOBJECT);
	if (!strncasecmp(typestr,"NOSUCHINSTANCE",7))
	    return(SNMP_NOSUCHINSTANCE);
	if (!strncasecmp(typestr,"UINTEGER",3))
	    return(TYPE_UINTEGER); /* historic - should not show up */
                                   /* but it does?                  */
	if (!strncasecmp(typestr, "NOTIF", 3))
		return(TYPE_NOTIFTYPE);
	if (!strncasecmp(typestr, "TRAP", 4))
		return(TYPE_TRAPTYPE);
        return(TYPE_UNKNOWN);
}

static int
__translate_asn_type(type)
int type;
{
   switch (type) {
        case ASN_INTEGER:
            return(TYPE_INTEGER);
	    break;
	case ASN_OCTET_STR:
            return(TYPE_OCTETSTR);
	    break;
	case ASN_OPAQUE:
            return(TYPE_OPAQUE);
	    break;
	case ASN_OBJECT_ID:
            return(TYPE_OBJID);
	    break;
	case ASN_TIMETICKS:
            return(TYPE_TIMETICKS);
	    break;
	case ASN_GAUGE:
            return(TYPE_GAUGE);
	    break;
	case ASN_COUNTER:
            return(TYPE_COUNTER);
	    break;
	case ASN_IPADDRESS:
            return(TYPE_IPADDR);
	    break;
	case ASN_BIT_STR:
            return(TYPE_BITSTRING);
	    break;
	case ASN_NULL:
            return(TYPE_NULL);
	    break;
	/* no translation for these exception type values */
	case SNMP_ENDOFMIBVIEW:
	case SNMP_NOSUCHOBJECT:
	case SNMP_NOSUCHINSTANCE:
	    return(type);
	    break;
	case ASN_UINTEGER:
            return(TYPE_UINTEGER);
	    break;
	case ASN_COUNTER64:
            return(TYPE_COUNTER64);
	    break;
	default:
            return(TYPE_OTHER);
            fprintf(stderr, "translate_asn_type: unhandled asn type (%d)\n",type);
            break;
        }
}

#define USE_BASIC 0
#define USE_ENUMS 1
#define USE_SPRINT_VALUE 2
static int
__snprint_value (buf, buf_len, var, tp, type, flag)
char * buf;
size_t buf_len;
netsnmp_variable_list * var;
struct tree * tp;
int type;
int flag;
{
   int len = 0;
   u_char* ip;
   struct enum_list *ep;


   buf[0] = '\0';
   if (flag == USE_SPRINT_VALUE) {
	snprint_value(buf, buf_len, var->name, var->name_length, var);
	len = STRLEN(buf);
   } else {
     switch (var->type) {
        case ASN_INTEGER:
           if (flag == USE_ENUMS) {
              for(ep = tp->enums; ep; ep = ep->next) {
                 if (ep->value == *var->val.integer) {
                    strcpy(buf, ep->label);
                    len = STRLEN(buf);
                    break;
                 }
              }
           }
           if (!len) {
              sprintf(buf,"%ld", *var->val.integer);
              len = STRLEN(buf);
           }
           break;

        case ASN_GAUGE:
        case ASN_COUNTER:
        case ASN_TIMETICKS:
        case ASN_UINTEGER:
           sprintf(buf,"%lu", (unsigned long) *var->val.integer);
           len = STRLEN(buf);
           break;

        case ASN_OCTET_STR:
        case ASN_OPAQUE:
           memcpy(buf, (char*)var->val.string, var->val_len);
           len = var->val_len;
           break;

        case ASN_IPADDRESS:
          ip = (u_char*)var->val.string;
          sprintf(buf, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
          len = STRLEN(buf);
          break;

        case ASN_NULL:
           break;

        case ASN_OBJECT_ID:
          __sprint_num_objid(buf, (oid *)(var->val.objid),
                             var->val_len/sizeof(oid));
          len = STRLEN(buf);
          break;

	case SNMP_ENDOFMIBVIEW:
          sprintf(buf,"%s", "ENDOFMIBVIEW");
	  break;
	case SNMP_NOSUCHOBJECT:
	  sprintf(buf,"%s", "NOSUCHOBJECT");
	  break;
	case SNMP_NOSUCHINSTANCE:
	  sprintf(buf,"%s", "NOSUCHINSTANCE");
	  break;

        case ASN_COUNTER64:
#ifdef OPAQUE_SPECIAL_TYPES
        case ASN_OPAQUE_COUNTER64:
        case ASN_OPAQUE_U64:
#endif
          printU64(buf,(struct counter64 *)var->val.counter64);
          len = STRLEN(buf);
          break;

#ifdef OPAQUE_SPECIAL_TYPES
        case ASN_OPAQUE_I64:
          printI64(buf,(struct counter64 *)var->val.counter64);
          len = STRLEN(buf);
          break;
#endif

        case ASN_BIT_STR:
            snprint_bitstring(buf, sizeof(buf), var, NULL, NULL, NULL);
            len = STRLEN(buf);
            break;
#ifdef OPAQUE_SPECIAL_TYPES
        case ASN_OPAQUE_FLOAT:
	  if (var->val.floatVal)
	    sprintf(buf,"%f", *var->val.floatVal);
         break;
         
        case ASN_OPAQUE_DOUBLE:
	  if (var->val.doubleVal)
	    sprintf(buf,"%f", *var->val.doubleVal);
         break;
#endif
         
        case ASN_NSAP:
        default:
	  fprintf(stderr,"snprint_value: asn type not handled %d\n",var->type);
     }
   }
   return(len);
}

static int
__sprint_num_objid (buf, objid, len)
char *buf;
oid *objid;
int len;
{
   int i;
   buf[0] = '\0';
   for (i=0; i < len; i++) {
	sprintf(buf,".%lu",*objid++);
	buf += STRLEN(buf);
   }
   return SUCCESS;
}

static int
__tp_sprint_num_objid (buf, tp)
char *buf;
SnmpMibNode *tp;
{
   oid newname[MAX_OID_LEN], *op;
   /* code taken from get_node in snmp_client.c */
   for (op = newname + MAX_OID_LEN - 1; op >= newname; op--) {
      *op = tp->subid;
      tp = tp->parent;
      if (tp == NULL) break;
   }
   return __sprint_num_objid(buf, op, newname + MAX_OID_LEN - op);
}

static int
__scan_num_objid (buf, objid, len)
char *buf;
oid *objid;
size_t *len;
{
   char *cp;
   *len = 0;
   if (*buf == '.') buf++;
   cp = buf;
   while (*buf) {
      if (*buf++ == '.') {
         sscanf(cp, "%lu", objid++);
         /* *objid++ = atoi(cp); */
         (*len)++;
         cp = buf;
      } else {
         if (isalpha((int)*buf)) {
	    return FAILURE;
         }
      }
   }
   sscanf(cp, "%lu", objid++);
   /* *objid++ = atoi(cp); */
   (*len)++;
   return SUCCESS;
}

static int
__get_type_str (type, str)
int type;
char * str;
{
   switch (type) {
	case TYPE_OBJID:
       		strcpy(str, "OBJECTID");
	        break;
	case TYPE_OCTETSTR:
       		strcpy(str, "OCTETSTR");
	        break;
	case TYPE_INTEGER:
       		strcpy(str, "INTEGER");
	        break;
	case TYPE_INTEGER32:
       		strcpy(str, "INTEGER32");
	        break;
	case TYPE_UNSIGNED32:
       		strcpy(str, "UNSIGNED32");
	        break;
	case TYPE_NETADDR:
       		strcpy(str, "NETADDR");
	        break;
	case TYPE_IPADDR:
       		strcpy(str, "IPADDR");
	        break;
	case TYPE_COUNTER:
       		strcpy(str, "COUNTER");
	        break;
	case TYPE_GAUGE:
       		strcpy(str, "GAUGE");
	        break;
	case TYPE_TIMETICKS:
       		strcpy(str, "TICKS");
	        break;
	case TYPE_OPAQUE:
       		strcpy(str, "OPAQUE");
	        break;
	case TYPE_COUNTER64:
       		strcpy(str, "COUNTER64");
	        break;
	case TYPE_NULL:
                strcpy(str, "NULL");
                break;
	case SNMP_ENDOFMIBVIEW:
                strcpy(str, "ENDOFMIBVIEW");
                break;
	case SNMP_NOSUCHOBJECT:
                strcpy(str, "NOSUCHOBJECT");
                break;
	case SNMP_NOSUCHINSTANCE:
                strcpy(str, "NOSUCHINSTANCE");
                break;
	case TYPE_UINTEGER:
                strcpy(str, "UINTEGER"); /* historic - should not show up */
                                          /* but it does?                  */
                break;
	case TYPE_NOTIFTYPE:
		strcpy(str, "NOTIF");
		break;
	case TYPE_BITSTRING:
		strcpy(str, "BITS");
		break;
	case TYPE_TRAPTYPE:
		strcpy(str, "TRAP");
		break;
	case TYPE_OTHER: /* not sure if this is a valid leaf type?? */
	case TYPE_NSAPADDRESS:
        default: /* unsupported types for now */
           strcpy(str, "");
	   if (_debug_level) printf("__get_type_str:FAILURE(%d)\n", type);

           return(FAILURE);
   }
   return SUCCESS;
}

/* does a destructive disection of <label1>...<labeln>.<iid> returning
   <labeln> and <iid> in seperate strings (note: will destructively
   alter input string, 'name') */
static int
__get_label_iid (name, last_label, iid, flag)
char * name;
char ** last_label;
char ** iid;
int flag;
{
   char *lcp;
   char *icp;
   int len = STRLEN(name);
   int found_label = 0;

   *last_label = *iid = NULL;

   if (len == 0) return(FAILURE);

   /* Handle case where numeric oid's have been requested.  The input 'name'
   ** in this case should be a numeric OID -- return failure if not.
   */
   if ((flag & USE_NUMERIC_OIDS)) {
      if (!__is_numeric_oid(name))
       return(FAILURE);

      /* Walk backward through the string, looking for first two '.' chars */
      lcp = &(name[len]);
      icp = NULL;
      while (lcp > name) {
       if (*lcp == '.') {

          /* If this is the first occurence of '.', note it in icp.
          ** Otherwise, this must be the second occurrence, so break
          ** out of the loop.
          */
          if (icp == NULL)
             icp = lcp;
          else
             break;
       }
       lcp --;
      }

      /* Make sure we found at least a label and index. */
      if (!icp)
         return(FAILURE);

      /* Push forward past leading '.' chars and separate the strings. */
      lcp ++;
      *icp ++ = '\0';

      *last_label = (flag & USE_LONG_NAMES) ? name : lcp;
      *iid        = icp;

      return(SUCCESS);
   }

   lcp = icp = &(name[len]);

   while (lcp > name) {
      if (*lcp == '.') {
	if (found_label) {
	   lcp++;
           break;
        } else {
           icp = lcp;
        }
      }
      if (!found_label && isalpha((int)*lcp)) found_label = 1;
      lcp--;
   }

   if (!found_label || (!isdigit((int)*(icp+1)) && (flag & FAIL_ON_NULL_IID)))
      return(FAILURE);

   if (flag & NON_LEAF_NAME) { /* dont know where to start instance id */
     /* put the whole thing in label */
     icp = &(name[len]);
     flag |= USE_LONG_NAMES;
     /* special hack in case no mib loaded - object identifiers will
      * start with .iso.<num>.<num>...., in which case it is preferable
      * to make the label entirely numeric (i.e., convert "iso" => "1")
      */
      if (*lcp == '.' && lcp == name) {
         if (!strncmp(".ccitt.",lcp,7)) {
            name += 2;
            *name = '.';
            *(name+1) = '0';
         } else if (!strncmp(".iso.",lcp,5)) {
            name += 2;
            *name = '.';
            *(name+1) = '1';
         } else if (!strncmp(".joint-iso-ccitt.",lcp,17)) {
            name += 2;
            *name = '.';
            *(name+1) = '2';
         }
      }
   } else if (*icp) {
      *(icp++) = '\0';
   }
   *last_label = (flag & USE_LONG_NAMES ? name : lcp);

   *iid = icp;

   return(SUCCESS);
}


static int
__oid_cmp(oida_arr, oida_arr_len, oidb_arr, oidb_arr_len)
oid *oida_arr;
int oida_arr_len;
oid *oidb_arr;
int oidb_arr_len;
{
   for (;oida_arr_len && oidb_arr_len;
	oida_arr++, oida_arr_len--, oidb_arr++, oidb_arr_len--) {
	if (*oida_arr == *oidb_arr) continue;
	return(*oida_arr > *oidb_arr ? 1 : -1);
   }
   if (oida_arr_len == oidb_arr_len) return(0);
   return(oida_arr_len > oidb_arr_len ? 1 : -1);
}


/* Convert a tag (string) to an OID array              */
/* Tag can be either a symbolic name, or an OID string */
static struct tree *
__tag2oid(tag, iid, oid_arr, oid_arr_len, type, best_guess)
char * tag;
char * iid;
oid  * oid_arr;
int  * oid_arr_len;
int  * type;
int    best_guess;
{
   struct tree *tp = NULL;
   struct tree *rtp = NULL;
   oid newname[MAX_OID_LEN], *op;
   size_t newname_len = 0;

   char str_buf[STR_BUF_SIZE];
   str_buf[0] = '\0';

   if (type) *type = TYPE_UNKNOWN;
   if (oid_arr_len) *oid_arr_len = 0;
   if (!tag) goto done;

   /*********************************************************/
   /* best_guess = 0 - same as no switches (read_objid)     */
   /*                  if multiple parts, or uses find_node */
   /*                  if a single leaf                     */
   /* best_guess = 1 - same as -Ib (get_wild_node)          */
   /* best_guess = 2 - same as -IR (get_node)               */
   /*********************************************************/

   /* numeric scalar                (1,2) */
   /* single symbolic               (1,2) */
   /* single regex                  (1)   */
   /* partial full symbolic         (2)   */
   /* full symbolic                 (2)   */
   /* module::single symbolic       (2)   */
   /* module::partial full symbolic (2)   */
   if (best_guess == 1 || best_guess == 2) { 
     if (!__scan_num_objid(tag, newname, &newname_len)) { /* make sure it's not a numeric tag */
       newname_len = MAX_OID_LEN;
       if (best_guess == 2) {		/* Random search -IR */
         if (get_node(tag, newname, &newname_len)) {
	   rtp = tp = get_tree(newname, newname_len, get_tree_head());
         }
       }
       else if (best_guess == 1) {	/* Regex search -Ib */
	 clear_tree_flags(get_tree_head()); 
         if (get_wild_node(tag, newname, &newname_len)) {
	   rtp = tp = get_tree(newname, newname_len, get_tree_head());
         }
       }
     }
     else {
       rtp = tp = get_tree(newname, newname_len, get_tree_head());
     }
     if (type) *type = (tp ? tp->type : TYPE_UNKNOWN);
     if ((oid_arr == NULL) || (oid_arr_len == NULL)) return rtp;
     memcpy(oid_arr,(char*)newname,newname_len*sizeof(oid));
     *oid_arr_len = newname_len;
   }
   
   /* if best_guess is off and multi part tag or module::tag */
   /* numeric scalar                                         */
   /* module::single symbolic                                */
   /* module::partial full symbolic                          */
   /* FULL symbolic OID                                      */
   else if (strchr(tag,'.') || strchr(tag,':')) { 
     if (!__scan_num_objid(tag, newname, &newname_len)) { /* make sure it's not a numeric tag */
	newname_len = MAX_OID_LEN;
	if (read_objid(tag, newname, &newname_len)) {	/* long name */
	  rtp = tp = get_tree(newname, newname_len, get_tree_head());
	}
      }
      else {
	rtp = tp = get_tree(newname, newname_len, get_tree_head());
      }
      if (type) *type = (tp ? tp->type : TYPE_UNKNOWN);
      if ((oid_arr == NULL) || (oid_arr_len == NULL)) return rtp;
      memcpy(oid_arr,(char*)newname,newname_len*sizeof(oid));
      *oid_arr_len = newname_len;
   }
   
   /* else best_guess is off and it is a single leaf */
   /* single symbolic                                */
   else { 
      rtp = tp = find_node(tag, get_tree_head());
      if (tp) {
         if (type) *type = tp->type;
         if ((oid_arr == NULL) || (oid_arr_len == NULL)) return rtp;
         /* code taken from get_node in snmp_client.c */
         for(op = newname + MAX_OID_LEN - 1; op >= newname; op--){
           *op = tp->subid;
	   tp = tp->parent;
	   if (tp == NULL)
	      break;
         }
         *oid_arr_len = newname + MAX_OID_LEN - op;
         memcpy(oid_arr, op, *oid_arr_len * sizeof(oid));
      } else {
         return(rtp);   /* HACK: otherwise, concat_oid_str confuses things */
      }
   }
 done:
   if (iid && *iid) __concat_oid_str(oid_arr, oid_arr_len, iid);
   return(rtp);
}
/* searches down the mib tree for the given oid
   returns the last found tp and its index in lastind
 */
static struct tree *
__oid2tp (oidp, len, subtree, lastind)
oid* oidp;
int len;
struct tree * subtree;
int* lastind;
{
    struct tree    *return_tree = NULL;


    for (; subtree; subtree = subtree->next_peer) {
	if (*oidp == subtree->subid){
	    goto found;
	}
    }
    *lastind=0;
    return NULL;

found:
    if (len > 1){
       return_tree =
          __oid2tp(oidp + 1, len - 1, subtree->child_list, lastind);
       (*lastind)++;
    } else {
       *lastind=1;
    }
    if (return_tree)
	return return_tree;
    else
	return subtree;
}

/* function: __concat_oid_str
 *
 * This function converts a dotted-decimal string, soid_str, to an array
 * of oid types and concatenates them on doid_arr begining at the index
 * specified by doid_arr_len.
 *
 * returns : SUCCESS, FAILURE
 */
static int
__concat_oid_str(doid_arr, doid_arr_len, soid_str)
oid *doid_arr;
int *doid_arr_len;
char * soid_str;
{
   char soid_buf[STR_BUF_SIZE];
   char *cp;
   char *st;

   if (!soid_str || !*soid_str) return SUCCESS;/* successfully added nothing */
   if (*soid_str == '.') soid_str++;
   strcpy(soid_buf, soid_str);
   cp = strtok_r(soid_buf,".",&st);
   while (cp) {
     sscanf(cp, "%lu", &(doid_arr[(*doid_arr_len)++]));
     /* doid_arr[(*doid_arr_len)++] =  atoi(cp); */
     cp = strtok_r(NULL,".",&st);
   }
   return(SUCCESS);
}

/*
 * add a varbind to PDU
 */
static int
__add_var_val_str(pdu, name, name_length, val, len, type)
    netsnmp_pdu *pdu;
    oid *name;
    int name_length;
    char * val;
    int len;
    int type;
{
    netsnmp_variable_list *vars;
    oid oidbuf[MAX_OID_LEN];
    int ret = SUCCESS;

    if (pdu->variables == NULL){
	pdu->variables = vars =
           (netsnmp_variable_list *)calloc(1,sizeof(netsnmp_variable_list));
    } else {
	for(vars = pdu->variables;
            vars->next_variable;
            vars = vars->next_variable)
	    /*EXIT*/;
	vars->next_variable =
           (netsnmp_variable_list *)calloc(1,sizeof(netsnmp_variable_list));
	vars = vars->next_variable;
    }

    vars->next_variable = NULL;
    vars->name = (oid *)malloc(name_length * sizeof(oid));
    memcpy((char *)vars->name, (char *)name, name_length * sizeof(oid));
    vars->name_length = name_length;
    switch (type) {
      case TYPE_INTEGER:
      case TYPE_INTEGER32:
        vars->type = ASN_INTEGER;
        vars->val.integer = (long *)malloc(sizeof(long));
        if (val)
            *(vars->val.integer) = strtol(val,NULL,0);
        else {
            ret = FAILURE;
            *(vars->val.integer) = 0;
        }
        vars->val_len = sizeof(long);
        break;

      case TYPE_GAUGE:
      case TYPE_UNSIGNED32:
        vars->type = ASN_GAUGE;
        goto UINT;
      case TYPE_COUNTER:
        vars->type = ASN_COUNTER;
        goto UINT;
      case TYPE_TIMETICKS:
        vars->type = ASN_TIMETICKS;
        goto UINT;
      case TYPE_UINTEGER:
        vars->type = ASN_UINTEGER;
UINT:
        vars->val.integer = (long *)malloc(sizeof(long));
        if (val)
            sscanf(val,"%lu",vars->val.integer);
        else {
            ret = FAILURE;
            *(vars->val.integer) = 0;
        }
        vars->val_len = sizeof(long);
        break;

      case TYPE_OCTETSTR:
	vars->type = ASN_OCTET_STR;
	goto OCT;

      case TYPE_BITSTRING:
	vars->type = ASN_OCTET_STR;
	goto OCT;

      case TYPE_OPAQUE:
        vars->type = ASN_OCTET_STR;
OCT:
        vars->val.string = (u_char *)malloc(len);
        vars->val_len = len;
        if (val && len)
            memcpy((char *)vars->val.string, val, len);
        else {
            ret = FAILURE;
            vars->val.string = (u_char*)strdup("");
            vars->val_len = 0;
        }
        break;

      case TYPE_IPADDR:
        vars->type = ASN_IPADDRESS;
        vars->val.integer = (long *)malloc(sizeof(long));
        if (val)
            *(vars->val.integer) = inet_addr(val);
        else {
            ret = FAILURE;
            *(vars->val.integer) = 0;
        }
        vars->val_len = sizeof(long);
        break;

      case TYPE_OBJID:
        vars->type = ASN_OBJECT_ID;
	vars->val_len = MAX_OID_LEN;
        /* if (read_objid(val, oidbuf, &(vars->val_len))) { */
	/* tp = __tag2oid(val,NULL,oidbuf,&(vars->val_len),NULL,0); */
        if (!val || !snmp_parse_oid(val, oidbuf, &vars->val_len)) {
            vars->val.objid = NULL;
	    ret = FAILURE;
        } else {
            vars->val_len *= sizeof(oid);
            vars->val.objid = (oid *)malloc(vars->val_len);
            memcpy((char *)vars->val.objid, (char *)oidbuf, vars->val_len);
        }
        break;

      default:
        vars->type = ASN_NULL;
	vars->val_len = 0;
	vars->val.string = NULL;
	ret = FAILURE;
    }

     return ret;
}

/* takes ss and pdu as input and updates the 'response' argument */
/* the input 'pdu' argument will be freed */
static int
__send_sync_pdu(ss, pdu, response, retry_nosuch,
	        err_str, err_num, err_ind)
netsnmp_session *ss;
netsnmp_pdu *pdu;
netsnmp_pdu **response;
int retry_nosuch;
char *err_str;
int *err_num;
int *err_ind;
{
   int status;
   long command = pdu->command;
   *response = NULL;
retry:

   status = snmp_synch_response(ss, pdu, response);

   if ((*response == NULL) && (status == STAT_SUCCESS)) status = STAT_ERROR;

   switch (status) {
      case STAT_SUCCESS:
	 switch ((*response)->errstat) {
	    case SNMP_ERR_NOERROR:
	       break;

            case SNMP_ERR_NOSUCHNAME:
               if (retry_nosuch && (pdu = snmp_fix_pdu(*response, command))) {
                  if (*response) snmp_free_pdu(*response);
                  goto retry;
               }

            /* Pv1, SNMPsec, Pv2p, v2c, v2u, v2*, and SNMPv3 PDUs */
            case SNMP_ERR_TOOBIG:
            case SNMP_ERR_BADVALUE:
            case SNMP_ERR_READONLY:
            case SNMP_ERR_GENERR:
            /* in SNMPv2p, SNMPv2c, SNMPv2u, SNMPv2*, and SNMPv3 PDUs */
            case SNMP_ERR_NOACCESS:
            case SNMP_ERR_WRONGTYPE:
            case SNMP_ERR_WRONGLENGTH:
            case SNMP_ERR_WRONGENCODING:
            case SNMP_ERR_WRONGVALUE:
            case SNMP_ERR_NOCREATION:
            case SNMP_ERR_INCONSISTENTVALUE:
            case SNMP_ERR_RESOURCEUNAVAILABLE:
            case SNMP_ERR_COMMITFAILED:
            case SNMP_ERR_UNDOFAILED:
            case SNMP_ERR_AUTHORIZATIONERROR:
            case SNMP_ERR_NOTWRITABLE:
            /* in SNMPv2c, SNMPv2u, SNMPv2*, and SNMPv3 PDUs */
            case SNMP_ERR_INCONSISTENTNAME:
            default:
               strcat(err_str,(char*)snmp_errstring((*response)->errstat));
               *err_num = (int)(*response)->errstat;
	       *err_ind = (*response)->errindex;
               status = (*response)->errstat;
               break;
	 }
         break;

      case STAT_TIMEOUT:
      case STAT_ERROR:
          strcat(err_str, (char*)snmp_api_errstring(ss->s_snmp_errno));
          *err_num = ss->s_snmp_errno;
         break;

      default:
         strcat(err_str, "send_sync_pdu: unknown status");
         *err_num = ss->s_snmp_errno;
         break;
   }
   if (_debug_level && *err_num) printf("XXX sync PDU: %s\n", err_str);
   return(status);
}

static PyObject * 
py_netsnmp_construct_varbind(void)
{
  PyObject *module;
  PyObject *dict;
  PyObject *callable;

  module = PyImport_ImportModule("netsnmp");
  dict = PyModule_GetDict(module);

  callable = PyDict_GetItemString(dict, "Varbind");

  return PyObject_CallFunction(callable, "");
}

static char *
py_netsnmp_attr_string(PyObject *obj, char * attr_name)
{
  char *val = NULL;

  if (obj && attr_name && PyObject_HasAttrString(obj, attr_name)) {
    PyObject *attr = PyObject_GetAttrString(obj, attr_name);
    if (attr) {
      val = PyString_AsString(attr);
      Py_DECREF(attr);
    }
  }

  return val;
}

static long
py_netsnmp_attr_long(PyObject *obj, char * attr_name)
{
  long val = -1;

  if (obj && attr_name  && PyObject_HasAttrString(obj, attr_name)) {
    PyObject *attr = PyObject_GetAttrString(obj, attr_name);
    if (attr) {
      val = PyInt_AsLong(attr);
      Py_DECREF(attr);
    }
  }

  return val;
}

static int
py_netsnmp_verbose(void)
{
  int verbose = 0;
  PyObject *pkg = PyImport_ImportModule("netsnmp");
  if (pkg) {
    verbose = py_netsnmp_attr_long(pkg, "verbose");
    Py_DECREF(pkg);
  }

  return verbose;
}

static int
py_netsnmp_attr_set_string(PyObject *obj, char *attr_name, 
			   char *val, size_t len)
{
  int ret = -1;
  if (obj && attr_name) {
    PyObject* val_obj =  (val ? 
			  Py_BuildValue("s#", val, len) : 
			  Py_BuildValue(""));
    ret = PyObject_SetAttrString(obj, attr_name, val_obj);
    Py_DECREF(val_obj);
  }
  return ret;
}

static PyObject *
netsnmp_create_session(PyObject *self, PyObject *args)
{
  int version;
  char *community;
  char *peer;
  int  lport;
  int  retries;
  int  timeout;
  SnmpSession session = {0};
  SnmpSession *ss = NULL;
  int verbose = py_netsnmp_verbose();

  if (!PyArg_ParseTuple(args, "issiii", &version,
			&community, &peer, &lport, 
			&retries, &timeout))
    return NULL;

  __libraries_init("python");

  session.version = -1;
#ifndef DISABLE_SNMPV1
  if (version == 1) {
    session.version = SNMP_VERSION_1;
  }
#endif
#ifndef DISABLE_SNMPV2C
  if (version == 2) {
    session.version = SNMP_VERSION_2c;
  }
#endif
  if (version == 3) {
    session.version = SNMP_VERSION_3;
  }
  if (session.version == -1) {
    if (verbose)
      printf("error:snmp_new_session:Unsupported SNMP version (%d)\n", version);
    goto end;
  }

  session.community_len = STRLEN((char *)community);
  session.community = (u_char *)community;
  session.peername = peer;
  session.local_port = lport;
  session.retries = retries; /* 5 */
  session.timeout = timeout; /* 1000000L */
  session.authenticator = NULL;

  ss = snmp_open(&session);

  if (ss == NULL) {
    if (verbose) 
      printf("error:snmp_new_session: Couldn't open SNMP session");
  }
 end:
  return Py_BuildValue("i", (int)ss);
}

static PyObject *
netsnmp_create_session_v3(PyObject *self, PyObject *args)
{
  int version;
  char *peer;
  int  lport;
  int  retries;
  int  timeout;
  char *  sec_name;
  int     sec_level;
  char *  sec_eng_id;
  char *  context_eng_id;
  char *  context;
  char *  auth_proto;
  char *  auth_pass;
  char *  priv_proto;
  char *  priv_pass;
  int     eng_boots;
  int     eng_time;
  SnmpSession session = {0};
  SnmpSession *ss = NULL;
  int verbose = py_netsnmp_verbose();

  if (!PyArg_ParseTuple(args, "isiiisisssssssii", &version,
			&peer, &lport, &retries, &timeout,
			&sec_name, &sec_level, &sec_eng_id, 
			&context_eng_id, &context, 
			&auth_proto, &auth_pass, 
			&priv_proto, &priv_pass,
			&eng_boots, &eng_time))
    return NULL;

  __libraries_init("python");

  if (version == 3) {
    session.version = SNMP_VERSION_3;
  } else {
    if (verbose)
      printf("error:snmp_new_v3_session:Unsupported SNMP version (%d)\n", version);
    goto end;
  }

  session.peername = peer;
  session.retries = retries; /* 5 */
  session.timeout = timeout; /* 1000000L */
  session.authenticator = NULL;
  session.contextNameLen = STRLEN(context);
  session.contextName = context;
  session.securityNameLen = STRLEN(sec_name);
  session.securityName = sec_name;
  session.securityLevel = sec_level;
  session.securityModel = USM_SEC_MODEL_NUMBER;
  session.securityEngineIDLen =
    hex_to_binary2((unsigned char*)sec_eng_id, STRLEN(sec_eng_id),
		   (char **) &session.securityEngineID);
  session.contextEngineIDLen =
    hex_to_binary2((unsigned char*)sec_eng_id, STRLEN(sec_eng_id),
		   (char **) &session.contextEngineID);
  session.engineBoots = eng_boots;
  session.engineTime = eng_time;

#ifndef DISABLE_MD5
  if (!strcmp(auth_proto, "MD5")) {
    session.securityAuthProto = 
      snmp_duplicate_objid(usmHMACMD5AuthProtocol,
			   USM_AUTH_PROTO_MD5_LEN);
    session.securityAuthProtoLen = USM_AUTH_PROTO_MD5_LEN;
  } else
#endif
    if (!strcmp(auth_proto, "SHA")) {
      session.securityAuthProto = 
	snmp_duplicate_objid(usmHMACSHA1AuthProtocol,
			     USM_AUTH_PROTO_SHA_LEN);
      session.securityAuthProtoLen = USM_AUTH_PROTO_SHA_LEN;
    } else if (!strcmp(auth_proto, "DEFAULT")) {
      session.securityAuthProto = 
	get_default_authtype(&session.securityAuthProtoLen);
    } else {
      if (verbose)
	printf("error:snmp_new_v3_session:Unsupported authentication protocol(%s)\n", auth_proto);
      goto end;
    }
  if (session.securityLevel >= SNMP_SEC_LEVEL_AUTHNOPRIV) {
    if (STRLEN(auth_pass) > 0) {
      session.securityAuthKeyLen = USM_AUTH_KU_LEN;
      if (generate_Ku(session.securityAuthProto,
		      session.securityAuthProtoLen,
		      (u_char *)auth_pass, STRLEN(auth_pass),
		      session.securityAuthKey,
		      &session.securityAuthKeyLen) != SNMPERR_SUCCESS) {
	if (verbose)
	  printf("error:snmp_new_v3_session:Error generating Ku from authentication password.\n");
	goto end;
      }
    }
  }
#ifndef DISABLE_DES
  if (!strcmp(priv_proto, "DES")) {
    session.securityPrivProto =
      snmp_duplicate_objid(usmDESPrivProtocol,
			   USM_PRIV_PROTO_DES_LEN);
    session.securityPrivProtoLen = USM_PRIV_PROTO_DES_LEN;
  } else
#endif
    if (!strncmp(priv_proto, "AES", 3)) {
      session.securityPrivProto =
	snmp_duplicate_objid(usmAESPrivProtocol,
			     USM_PRIV_PROTO_AES_LEN);
      session.securityPrivProtoLen = USM_PRIV_PROTO_AES_LEN;
    } else if (!strcmp(priv_proto, "DEFAULT")) {
      session.securityPrivProto = 
	get_default_privtype(&session.securityPrivProtoLen);
    } else {
      if (verbose)
	printf("error:snmp_new_v3_session:Unsupported privacy protocol(%s)\n", priv_proto);
      goto end;
    }

  if (session.securityLevel >= SNMP_SEC_LEVEL_AUTHPRIV) {
    session.securityPrivKeyLen = USM_PRIV_KU_LEN;
    if (generate_Ku(session.securityAuthProto,
		    session.securityAuthProtoLen,
		    (u_char *)priv_pass, STRLEN(priv_pass),
		    session.securityPrivKey,
		    &session.securityPrivKeyLen) != SNMPERR_SUCCESS) {
      if (verbose)
	printf("error:v3_session: couldn't gen Ku from priv pass phrase.\n");
      goto end;
    }
  }
  
  ss = snmp_open(&session);

 end:
  if (ss == NULL) {
    if (verbose) 
      printf("error:v3_session: couldn't open SNMP session(%s).\n",
	     snmp_api_errstring(snmp_errno));
  }
  free (session.securityEngineID);
  free (session.contextEngineID);

  return Py_BuildValue("i", (int)ss);
}

static PyObject *
netsnmp_delete_session(PyObject *self, PyObject *args)
{
  PyObject *session;
  SnmpSession *ss = NULL;

  if (!PyArg_ParseTuple(args, "O", &session)) {
    return NULL;
  }

  ss = (SnmpSession *)py_netsnmp_attr_long(session, "sess_ptr");

  snmp_close(ss);
  return (Py_BuildValue(""));
}


static PyObject *
netsnmp_get(PyObject *self, PyObject *args)
{
  PyObject *session;
  PyObject *varlist;
  PyObject *varbind;
  PyObject *val_tuple = NULL;
  int varlist_len = 0;
  int varlist_ind;
  netsnmp_session *ss;
  netsnmp_pdu *pdu, *response;
  netsnmp_variable_list *vars;
  struct tree *tp;
  int len;
  oid *oid_arr;
  int oid_arr_len = MAX_OID_LEN;
  int type;
  char type_str[MAX_TYPE_NAME_LEN];
  int status;
  u_char str_buf[STR_BUF_SIZE], *str_bufp = str_buf;
  size_t str_buf_len = sizeof(str_buf);
  size_t out_len = 0;
  int buf_over = 0;
  char *tag;
  char *iid;
  int getlabel_flag = NO_FLAGS;
  int sprintval_flag = USE_BASIC;
  int verbose = py_netsnmp_verbose();
  int old_format;
  int best_guess;
  int retry_nosuch;
  int err_ind;
  int err_num;
  char err_str[STR_BUF_SIZE];
	   
  oid_arr = calloc(MAX_OID_LEN, sizeof(oid));

  if (oid_arr && args) {

    if (!PyArg_ParseTuple(args, "OO", &session, &varlist)) {
      goto done;
    }

    ss = (SnmpSession *)py_netsnmp_attr_long(session, "sess_ptr");

    strcpy(err_str, py_netsnmp_attr_string(session, "ErrorStr"));
    err_num = py_netsnmp_attr_long(session, "ErrorNum");
    err_ind = py_netsnmp_attr_long(session, "ErrorInd");

    if (py_netsnmp_attr_long(session, "UseLongNames"))
      getlabel_flag |= USE_LONG_NAMES;
    if (py_netsnmp_attr_long(session, "UseNumeric"))
      getlabel_flag |= USE_NUMERIC_OIDS;
    if (py_netsnmp_attr_long(session, "UseEnums"))
      sprintval_flag = USE_ENUMS;
    if (py_netsnmp_attr_long(session, "UseSprintValue"))
      sprintval_flag = USE_SPRINT_VALUE;	
    best_guess = py_netsnmp_attr_long(session, "BestGuess");
    retry_nosuch = py_netsnmp_attr_long(session, "RetryNoSuch");
      
    pdu = snmp_pdu_create(SNMP_MSG_GET);

    if (varlist) {
      PyObject *varlist_iter = PyObject_GetIter(varlist);

      while (varlist_iter && (varbind = PyIter_Next(varlist_iter))) {
	tag = py_netsnmp_attr_string(varbind, "tag");
	iid = py_netsnmp_attr_string(varbind, "iid");


	tp = __tag2oid(tag, iid, oid_arr, &oid_arr_len, 
		       NULL, best_guess);

	if (oid_arr_len) {
	  snmp_add_null_var(pdu, oid_arr, oid_arr_len);
	  varlist_len++;
	} else {
	  if (verbose)
	    printf("error: get: unknown object ID (%s)",
		   (tag ? tag : "<null>"));
	  snmp_free_pdu(pdu);
	  goto done;
	}
	/* release reference when done */
	Py_DECREF(varbind);
      }

      Py_DECREF(varlist_iter);

      if (PyErr_Occurred()) {
	/* propagate error */
	if (verbose)
	  printf("error: get: unknown python error");
	snmp_free_pdu(pdu);
	goto done;
      }
    }

    status = __send_sync_pdu(ss, pdu, &response, retry_nosuch, 
			     err_str, &err_num, &err_ind);

    /*
    ** Set up for numeric or full OID's, if necessary.  Save the old
    ** output format so that it can be restored when we finish -- this
    ** is a library-wide global, and has to be set/restored for each
    ** session.
    */
    old_format = netsnmp_ds_get_int(NETSNMP_DS_LIBRARY_ID,
				    NETSNMP_DS_LIB_OID_OUTPUT_FORMAT);

    if (py_netsnmp_attr_long(session, "UseLongNames")) {
      getlabel_flag |= USE_LONG_NAMES;

      netsnmp_ds_set_int(NETSNMP_DS_LIBRARY_ID,
			 NETSNMP_DS_LIB_OID_OUTPUT_FORMAT,
			 NETSNMP_OID_OUTPUT_FULL);
    }
    /* Setting UseNumeric forces UseLongNames on so check for UseNumeric
       after UseLongNames (above) to make sure the final outcome of 
       NETSNMP_DS_LIB_OID_OUTPUT_FORMAT is NETSNMP_OID_OUTPUT_NUMERIC */
    if (py_netsnmp_attr_long(session, "UseNumeric")) {
      getlabel_flag |= USE_LONG_NAMES;
      getlabel_flag |= USE_NUMERIC_OIDS;

      netsnmp_ds_set_int(NETSNMP_DS_LIBRARY_ID,
			 NETSNMP_DS_LIB_OID_OUTPUT_FORMAT,
			 NETSNMP_OID_OUTPUT_NUMERIC);
    }

    val_tuple = PyTuple_New(varlist_len);
    // initialize return tuple
    for (varlist_ind = 0; varlist_ind < varlist_len; varlist_ind++) {
      PyTuple_SetItem(val_tuple, varlist_ind, Py_BuildValue(""));
    }

    for(vars = (response ? response->variables : NULL), varlist_ind = 0;
	vars && (varlist_ind < varlist_len);
	vars = vars->next_variable, varlist_ind++) {

      varbind = PySequence_GetItem(varlist, varlist_ind);

      if (PyObject_HasAttrString(varbind, "tag")) {
	*str_buf = '.';
	*(str_buf+1) = '\0';
	out_len = 0;
	tp = netsnmp_sprint_realloc_objid_tree(&str_bufp, &str_buf_len,
					       &out_len, 0, &buf_over,
					       vars->name,vars->name_length);
	if (_debug_level) 
	  printf("netsnmp_get:str_bufp:%s:%d:%d\n",
		 str_bufp,str_buf_len,out_len);

	str_buf[sizeof(str_buf)-1] = '\0';

	if (__is_leaf(tp)) {
	  type = (tp->type ? tp->type : tp->parent->type);
	  getlabel_flag &= ~NON_LEAF_NAME;
	  if (_debug_level) printf("netsnmp_get:is_leaf:%d\n",type);
	} else {
	  getlabel_flag |= NON_LEAF_NAME;
	  type = __translate_asn_type(vars->type);
	  if (_debug_level) printf("netsnmp_get:!is_leaf:%d\n",tp->type);
	}
	
	if (_debug_level) printf("netsnmp_get:str_buf:%s\n",str_buf);

	__get_label_iid(str_buf, &tag, &iid, getlabel_flag);

	py_netsnmp_attr_set_string(varbind, "tag", tag, STRLEN(tag));
	py_netsnmp_attr_set_string(varbind, "iid", iid, STRLEN(iid));

	__get_type_str(type, type_str);

	py_netsnmp_attr_set_string(varbind, "type", type_str, 
				   STRLEN(type_str));

	len = __snprint_value(str_buf,sizeof(str_buf),
			    vars,tp,type,sprintval_flag);
	str_buf[len] = '\0';
	py_netsnmp_attr_set_string(varbind, "val", str_buf, len);

	/* save in return tuple as well */
	PyTuple_SetItem(val_tuple, varlist_ind, 
			(len ? Py_BuildValue("s#", str_buf, len) :
			 Py_BuildValue("")));

	Py_DECREF(varbind);
      } else {
	printf("netsnmp_get: bad varbind (%d)\n", varlist_ind);
      }	
    }

    /* Reset the library's behavior for numeric/symbolic OID's. */
    netsnmp_ds_set_int(NETSNMP_DS_LIBRARY_ID,
		       NETSNMP_DS_LIB_OID_OUTPUT_FORMAT,
		       old_format);

    if (response) snmp_free_pdu(response);
  }

 done:
  SAFE_FREE(oid_arr);
  return (val_tuple ? val_tuple : Py_BuildValue(""));
}

static PyObject *
netsnmp_getnext(PyObject *self, PyObject *args)
{
  PyObject *session;
  PyObject *varlist;
  PyObject *varbind;
  PyObject *val_tuple = NULL;
  int varlist_len = 0;
  int varlist_ind;
  netsnmp_session *ss;
  netsnmp_pdu *pdu, *response;
  netsnmp_variable_list *vars;
  struct tree *tp;
  int len;
  oid *oid_arr;
  int oid_arr_len = MAX_OID_LEN;
  int type;
  char type_str[MAX_TYPE_NAME_LEN];
  int status;
  u_char str_buf[STR_BUF_SIZE], *str_bufp = str_buf;
  size_t str_buf_len = sizeof(str_buf);
  size_t out_len = 0;
  int buf_over = 0;
  char *tag;
  char *iid;
  int getlabel_flag = NO_FLAGS;
  int sprintval_flag = USE_BASIC;
  int verbose = py_netsnmp_verbose();
  int old_format;
  int best_guess;
  int retry_nosuch;
  int err_ind;
  int err_num;
  char err_str[STR_BUF_SIZE];
	   
  oid_arr = calloc(MAX_OID_LEN, sizeof(oid));

  if (oid_arr && args) {

    if (!PyArg_ParseTuple(args, "OO", &session, &varlist)) {
      goto done;
    }

    ss = (SnmpSession *)py_netsnmp_attr_long(session, "sess_ptr");

    strcpy(err_str, py_netsnmp_attr_string(session, "ErrorStr"));
    err_num = py_netsnmp_attr_long(session, "ErrorNum");
    err_ind = py_netsnmp_attr_long(session, "ErrorInd");

    if (py_netsnmp_attr_long(session, "UseLongNames"))
      getlabel_flag |= USE_LONG_NAMES;
    if (py_netsnmp_attr_long(session, "UseNumeric"))
      getlabel_flag |= USE_NUMERIC_OIDS;
    if (py_netsnmp_attr_long(session, "UseEnums"))
      sprintval_flag = USE_ENUMS;
    if (py_netsnmp_attr_long(session, "UseSprintValue"))
      sprintval_flag = USE_SPRINT_VALUE;	
    best_guess = py_netsnmp_attr_long(session, "BestGuess");
    retry_nosuch = py_netsnmp_attr_long(session, "RetryNoSuch");
      
    pdu = snmp_pdu_create(SNMP_MSG_GETNEXT);

    if (varlist) {
      PyObject *varlist_iter = PyObject_GetIter(varlist);

      while (varlist_iter && (varbind = PyIter_Next(varlist_iter))) {
	tag = py_netsnmp_attr_string(varbind, "tag");
	iid = py_netsnmp_attr_string(varbind, "iid");

	tp = __tag2oid(tag, iid, oid_arr, &oid_arr_len, 
		       NULL, best_guess);

	if (_debug_level) 
	  printf("netsnmp_getnext: filling request: %s:%s:%d:%d\n", 
		 tag, iid, oid_arr_len,best_guess);

	if (oid_arr_len) {
	  snmp_add_null_var(pdu, oid_arr, oid_arr_len);
	  varlist_len++;
	} else {
	  if (verbose)
	    printf("error: get: unknown object ID (%s)",
		   (tag ? tag : "<null>"));
	  snmp_free_pdu(pdu);
	  goto done;
	}
	/* release reference when done */
	Py_DECREF(varbind);
      }

      Py_DECREF(varlist_iter);

      if (PyErr_Occurred()) {
	/* propagate error */
	if (verbose)
	  printf("error: get: unknown python error");
	snmp_free_pdu(pdu);
	goto done;
      }
    }

    status = __send_sync_pdu(ss, pdu, &response, retry_nosuch, 
			     err_str, &err_num, &err_ind);

    /*
    ** Set up for numeric or full OID's, if necessary.  Save the old
    ** output format so that it can be restored when we finish -- this
    ** is a library-wide global, and has to be set/restored for each
    ** session.
    */
    old_format = netsnmp_ds_get_int(NETSNMP_DS_LIBRARY_ID,
				    NETSNMP_DS_LIB_OID_OUTPUT_FORMAT);

    if (py_netsnmp_attr_long(session, "UseLongNames")) {
      getlabel_flag |= USE_LONG_NAMES;

      netsnmp_ds_set_int(NETSNMP_DS_LIBRARY_ID,
			 NETSNMP_DS_LIB_OID_OUTPUT_FORMAT,
			 NETSNMP_OID_OUTPUT_FULL);
    }
    /* Setting UseNumeric forces UseLongNames on so check for UseNumeric
       after UseLongNames (above) to make sure the final outcome of 
       NETSNMP_DS_LIB_OID_OUTPUT_FORMAT is NETSNMP_OID_OUTPUT_NUMERIC */
    if (py_netsnmp_attr_long(session, "UseNumeric")) {
      getlabel_flag |= USE_LONG_NAMES;
      getlabel_flag |= USE_NUMERIC_OIDS;

      netsnmp_ds_set_int(NETSNMP_DS_LIBRARY_ID,
			 NETSNMP_DS_LIB_OID_OUTPUT_FORMAT,
			 NETSNMP_OID_OUTPUT_NUMERIC);
    }

    val_tuple = PyTuple_New(varlist_len);
    // initialize return tuple
    val_tuple = PyTuple_New(varlist_len);
    for (varlist_ind = 0; varlist_ind < varlist_len; varlist_ind++) {
      PyTuple_SetItem(val_tuple, varlist_ind, Py_BuildValue(""));
    }

    for(vars = (response ? response->variables : NULL), varlist_ind = 0;
	vars && (varlist_ind < varlist_len);
	vars = vars->next_variable, varlist_ind++) {

      varbind = PySequence_GetItem(varlist, varlist_ind);

      if (PyObject_HasAttrString(varbind, "tag")) {
	*str_buf = '.';
	*(str_buf+1) = '\0';
	out_len = 0;
	tp = netsnmp_sprint_realloc_objid_tree(&str_bufp, &str_buf_len,
					       &out_len, 0, &buf_over,
					       vars->name,vars->name_length);
	str_buf[sizeof(str_buf)-1] = '\0';

	if (__is_leaf(tp)) {
	  type = (tp->type ? tp->type : tp->parent->type);
	  getlabel_flag &= ~NON_LEAF_NAME;
	} else {
	  getlabel_flag |= NON_LEAF_NAME;
	  type = __translate_asn_type(vars->type);
	}

	__get_label_iid(str_buf, &tag, &iid, getlabel_flag);

	if (_debug_level) 
	  printf("netsnmp_getnext: filling response: %s:%s\n", tag, iid);

	py_netsnmp_attr_set_string(varbind, "tag", tag, STRLEN(tag));
	py_netsnmp_attr_set_string(varbind, "iid", iid, STRLEN(iid));

	__get_type_str(type, type_str);

	py_netsnmp_attr_set_string(varbind, "type", type_str, 
				   STRLEN(type_str));

	len = __snprint_value(str_buf,sizeof(str_buf),
			    vars,tp,type,sprintval_flag);
	str_buf[len] = '\0';

	py_netsnmp_attr_set_string(varbind, "val", str_buf, len);

	/* save in return tuple as well */
	PyTuple_SetItem(val_tuple, varlist_ind, 
			(len ? Py_BuildValue("s#", str_buf, len) :
			 Py_BuildValue("")));

	Py_DECREF(varbind);
      } else {
	printf("netsnmp_getnext: bad varbind (%d)\n", varlist_ind);
      }
    }

    /* Reset the library's behavior for numeric/symbolic OID's. */
    netsnmp_ds_set_int(NETSNMP_DS_LIBRARY_ID,
		       NETSNMP_DS_LIB_OID_OUTPUT_FORMAT,
		       old_format);

    if (response) snmp_free_pdu(response);
  }

 done:
  SAFE_FREE(oid_arr);
  return (val_tuple ? val_tuple : Py_BuildValue(""));
}

static PyObject *
netsnmp_walk(PyObject *self, PyObject *args)
{
  PyObject *session;
  PyObject *varlist;
  PyObject *varbind;
  PyObject *val_tuple = NULL;
  PyObject *varbinds;
  int varlist_len = 0;
  int varlist_ind;
  netsnmp_session *ss;
  netsnmp_pdu *pdu, *response;
  netsnmp_variable_list *vars;
  struct tree *tp;
  int len;
  oid *oid_arr;
  int oid_arr_len = MAX_OID_LEN;
  int type;
  char type_str[MAX_TYPE_NAME_LEN];
  int status;
  u_char str_buf[STR_BUF_SIZE], *str_bufp = str_buf;
  size_t str_buf_len = sizeof(str_buf);
  size_t out_len = 0;
  int buf_over = 0;
  char *tag;
  char *iid;
  int getlabel_flag = NO_FLAGS;
  int sprintval_flag = USE_BASIC;
  int verbose = py_netsnmp_verbose();
  int old_format;
  int best_guess;
  int retry_nosuch;
  int err_ind;
  int err_num;
  char err_str[STR_BUF_SIZE];
  int notdone = 1;
  int result_count = 0;
	   
  oid_arr = calloc(MAX_OID_LEN, sizeof(oid));

  if (oid_arr && args) {

    if (!PyArg_ParseTuple(args, "OO", &session, &varlist)) {
      goto done;
    }

    if (!varlist) {
      goto done;
    }

    if ((varbinds = PyObject_GetAttrString(varlist, "varbinds")) == NULL) {
      goto done;
    }
    ss = (SnmpSession *)py_netsnmp_attr_long(session, "sess_ptr");

    strcpy(err_str, py_netsnmp_attr_string(session, "ErrorStr"));
    err_num = py_netsnmp_attr_long(session, "ErrorNum");
    err_ind = py_netsnmp_attr_long(session, "ErrorInd");

    if (py_netsnmp_attr_long(session, "UseLongNames"))
      getlabel_flag |= USE_LONG_NAMES;
    if (py_netsnmp_attr_long(session, "UseNumeric"))
      getlabel_flag |= USE_NUMERIC_OIDS;
    if (py_netsnmp_attr_long(session, "UseEnums"))
      sprintval_flag = USE_ENUMS;
    if (py_netsnmp_attr_long(session, "UseSprintValue"))
      sprintval_flag = USE_SPRINT_VALUE;	
    best_guess = py_netsnmp_attr_long(session, "BestGuess");
    retry_nosuch = py_netsnmp_attr_long(session, "RetryNoSuch");
        
    PyObject *varlist_iter = PyObject_GetIter(varlist);

    pdu = snmp_pdu_create(SNMP_MSG_GETNEXT);
    
    /* get the initial starting oids*/
    while (varlist_iter && (varbind = PyIter_Next(varlist_iter))) {
      tag = py_netsnmp_attr_string(varbind, "tag");
      iid = py_netsnmp_attr_string(varbind, "iid");

      tp = __tag2oid(tag, iid, oid_arr, &oid_arr_len, 
      	       NULL, best_guess);

      if (_debug_level) 
	printf("netsnmp_walk: filling request: %s:%s:%d:%d\n", 
	       tag, iid, oid_arr_len,best_guess);

      if (oid_arr_len) {
        snmp_add_null_var(pdu, oid_arr, oid_arr_len);
        varlist_len++;
      } else {
        if (verbose)
          printf("error: walk: unknown object ID (%s)",
      	   (tag ? tag : "<null>"));
        snmp_free_pdu(pdu);
        goto done;
      }
      /* release reference when done */
      Py_DECREF(varbind);
    }

    Py_DECREF(varlist_iter);

    if (PyErr_Occurred()) {
      /* propagate error */
      if (verbose)
        printf("error: walk: unknown python error (varlist)");
      snmp_free_pdu(pdu);
      goto done;
    }

    /* pre-allocate the return tuples */
    val_tuple = PyTuple_New(0);

    if (!val_tuple) {
      /* propagate error */
      if (verbose)
        printf("error: walk: couldn't allocate a new value tuple");
      snmp_free_pdu(pdu);
      goto done;
    }        
    
    /*
    ** Set up for numeric or full OID's, if necessary.  Save the old
    ** output format so that it can be restored when we finish -- this
    ** is a library-wide global, and has to be set/restored for each
    ** session.
    */
    old_format = netsnmp_ds_get_int(NETSNMP_DS_LIBRARY_ID,
                                    NETSNMP_DS_LIB_OID_OUTPUT_FORMAT);

    if (py_netsnmp_attr_long(session, "UseLongNames")) {
      getlabel_flag |= USE_LONG_NAMES;

      netsnmp_ds_set_int(NETSNMP_DS_LIBRARY_ID,
                         NETSNMP_DS_LIB_OID_OUTPUT_FORMAT,
                         NETSNMP_OID_OUTPUT_FULL);
    }

    /* Setting UseNumeric forces UseLongNames on so check for UseNumeric
       after UseLongNames (above) to make sure the final outcome of 
       NETSNMP_DS_LIB_OID_OUTPUT_FORMAT is NETSNMP_OID_OUTPUT_NUMERIC */
    if (py_netsnmp_attr_long(session, "UseNumeric")) {
      getlabel_flag |= USE_LONG_NAMES;
      getlabel_flag |= USE_NUMERIC_OIDS;

      netsnmp_ds_set_int(NETSNMP_DS_LIBRARY_ID,
                         NETSNMP_DS_LIB_OID_OUTPUT_FORMAT,
                         NETSNMP_OID_OUTPUT_NUMERIC);
    }

    /* delete the existing varbinds that we'll replace */
    PySequence_DelSlice(varbinds, 0, PySequence_Length(varbinds));

    if (PyErr_Occurred()) {
      /* propagate error */
      if (verbose)
        printf("error: walk: deleting old varbinds failed\n");
      snmp_free_pdu(pdu);
      goto done;
    }

    while(notdone) {

      status = __send_sync_pdu(ss, pdu, &response, retry_nosuch, 
                               err_str, &err_num, &err_ind);
      
      if (!response || !response->variables ||
          (response->variables->name_length < oid_arr_len) ||
          (memcmp(oid_arr, response->variables->name,
                  oid_arr_len * sizeof(oid))) ||
          status != STAT_SUCCESS ||
          response->errstat != SNMP_ERR_NOERROR) {
          notdone = 0;
      } else {
        for(vars = (response ? response->variables : NULL), varlist_ind = 0;
    	vars && (varlist_ind < varlist_len);
    	vars = vars->next_variable, varlist_ind++) {

          if ((vars->type == SNMP_ENDOFMIBVIEW) ||
              (vars->type == SNMP_NOSUCHOBJECT) ||
              (vars->type == SNMP_NOSUCHINSTANCE)) {
              notdone = 0;
              break;
          }

          varbind = py_netsnmp_construct_varbind();

          if (PyObject_HasAttrString(varbind, "tag")) {
    	  str_buf[0] = '.';
    	  str_buf[1] = '\0';
    	  out_len = 0;
    	  tp = netsnmp_sprint_realloc_objid_tree(&str_bufp, &str_buf_len,
                                                   &out_len, 0, &buf_over,
                                                   vars->name,vars->name_length);
    	  str_buf[sizeof(str_buf)-1] = '\0';

    	  if (__is_leaf(tp)) {
	    type = (tp->type ? tp->type : tp->parent->type);
    	    getlabel_flag &= ~NON_LEAF_NAME;
    	  } else {
    	    getlabel_flag |= NON_LEAF_NAME;
    	    type = __translate_asn_type(vars->type);
    	  }

    	  __get_label_iid(str_buf, &tag, &iid, getlabel_flag);

    	  if (_debug_level) printf("netsnmp_walk: filling response: %s:%s\n", tag, iid);

	  py_netsnmp_attr_set_string(varbind, "tag", tag, STRLEN(tag));
	  py_netsnmp_attr_set_string(varbind, "iid", iid, STRLEN(iid));

    	  __get_type_str(type, type_str);

	  py_netsnmp_attr_set_string(varbind, "type", type_str, 
				     STRLEN(type_str));

    	  len = __snprint_value(str_buf,sizeof(str_buf),
				vars,tp,type,sprintval_flag);
    	  str_buf[len] = '\0';

	  py_netsnmp_attr_set_string(varbind, "val", str_buf, len);
            
	  /* push the varbind onto the return varbinds */
	  PyList_Append(varbinds, varbind);

    	  /* save in return tuple as well */
	  /* save in return tuple as well - steals ref */
	  _PyTuple_Resize(&val_tuple, result_count+1);
	  PyTuple_SetItem(val_tuple, result_count++, 
			  (len ? Py_BuildValue("s#", str_buf, len) :
			   Py_BuildValue("")));
            
    	  Py_DECREF(varbind);

          } else {
	    /* Return None for this variable. */
            _PyTuple_Resize(&val_tuple, result_count+1);
	    PyTuple_SetItem(val_tuple, result_count++, Py_BuildValue(""));
	    printf("netsnmp_walk: bad varbind (%d)\n", varlist_ind);
          }	
        }
        /* reuse the response as the next pdu to send */
        pdu = snmp_pdu_create(SNMP_MSG_GETNEXT);
        snmp_add_null_var(pdu, response->variables->name,
                          response->variables->name_length);
      }
      if (response)
	snmp_free_pdu(response);
    }

    /* Reset the library's behavior for numeric/symbolic OID's. */
    netsnmp_ds_set_int(NETSNMP_DS_LIBRARY_ID,
		       NETSNMP_DS_LIB_OID_OUTPUT_FORMAT,
		       old_format);


    if (PyErr_Occurred()) {
      /* propagate error */
      if (verbose)
	printf("error: walk response processing: unknown python error");
      Py_DECREF(val_tuple);
    } 
  }

 done:
  SAFE_FREE(oid_arr);
  return (val_tuple ? val_tuple : Py_BuildValue(""));
}


static PyObject *
netsnmp_getbulk(PyObject *self, PyObject *args)
{
  int nonrepeaters;
  int maxrepetitions;
  PyObject *session;
  PyObject *varlist;
  PyObject *varbinds;
  PyObject *varbind;
  PyObject *val_tuple = NULL;
  int varbind_ind;
  netsnmp_session *ss;
  netsnmp_pdu *pdu, *response;
  netsnmp_variable_list *vars;
  struct tree *tp;
  int len;
  oid *oid_arr;
  int oid_arr_len = MAX_OID_LEN;
  int type;
  char type_str[MAX_TYPE_NAME_LEN];
  int status;
  u_char str_buf[STR_BUF_SIZE], *str_bufp = str_buf;
  size_t str_buf_len = sizeof(str_buf);
  size_t out_len = 0;
  int buf_over = 0;
  char *tag;
  char *iid;
  int getlabel_flag = NO_FLAGS;
  int sprintval_flag = USE_BASIC;
  int verbose = py_netsnmp_verbose();
  int old_format;
  int best_guess;
  int retry_nosuch;
  int err_ind;
  int err_num;
  char err_str[STR_BUF_SIZE];
	   
  oid_arr = calloc(MAX_OID_LEN, sizeof(oid));

  if (oid_arr && args) {

    if (!PyArg_ParseTuple(args, "OiiO", &session, &nonrepeaters, 
			  &maxrepetitions, &varlist)) {
      goto done;
    }

    if (varlist && (varbinds = PyObject_GetAttrString(varlist, "varbinds"))) {
      
      ss = (SnmpSession *)py_netsnmp_attr_long(session, "sess_ptr");

      strcpy(err_str, py_netsnmp_attr_string(session, "ErrorStr"));
      err_num = py_netsnmp_attr_long(session, "ErrorNum");
      err_ind = py_netsnmp_attr_long(session, "ErrorInd");

      if (py_netsnmp_attr_long(session, "UseLongNames"))
	getlabel_flag |= USE_LONG_NAMES;
      if (py_netsnmp_attr_long(session, "UseNumeric"))
	getlabel_flag |= USE_NUMERIC_OIDS;
      if (py_netsnmp_attr_long(session, "UseEnums"))
	sprintval_flag = USE_ENUMS;
      if (py_netsnmp_attr_long(session, "UseSprintValue"))
	sprintval_flag = USE_SPRINT_VALUE;	
      best_guess = py_netsnmp_attr_long(session, "BestGuess");
      retry_nosuch = py_netsnmp_attr_long(session, "RetryNoSuch");
      
      pdu = snmp_pdu_create(SNMP_MSG_GETBULK);

      pdu->errstat = nonrepeaters;
      pdu->errindex = maxrepetitions;

      PyObject *varbinds_iter = PyObject_GetIter(varbinds);

      while (varbinds_iter && (varbind = PyIter_Next(varbinds_iter))) {
	tag = py_netsnmp_attr_string(varbind, "tag");
	iid = py_netsnmp_attr_string(varbind, "iid");

	tp = __tag2oid(tag, iid, oid_arr, &oid_arr_len, 
		       NULL, best_guess);

	if (oid_arr_len) {
	  snmp_add_null_var(pdu, oid_arr, oid_arr_len);
	} else {
	  if (verbose)
	    printf("error: get: unknown object ID (%s)",
		   (tag ? tag : "<null>"));
	  snmp_free_pdu(pdu);
	  goto done;
	}
	/* release reference when done */
	Py_DECREF(varbind);
      }

      Py_DECREF(varbinds_iter);

      if (PyErr_Occurred()) {
	/* propagate error */
	if (verbose)
	  printf("error: get: unknown python error");
	snmp_free_pdu(pdu);
	goto done;
      }

      status = __send_sync_pdu(ss, pdu, &response, retry_nosuch, 
			       err_str, &err_num, &err_ind);

      /*
      ** Set up for numeric or full OID's, if necessary.  Save the old
      ** output format so that it can be restored when we finish -- this
      ** is a library-wide global, and has to be set/restored for each
      ** session.
      */
      old_format = netsnmp_ds_get_int(NETSNMP_DS_LIBRARY_ID,
				      NETSNMP_DS_LIB_OID_OUTPUT_FORMAT);

      if (py_netsnmp_attr_long(session, "UseLongNames")) {
	getlabel_flag |= USE_LONG_NAMES;

	netsnmp_ds_set_int(NETSNMP_DS_LIBRARY_ID,
			   NETSNMP_DS_LIB_OID_OUTPUT_FORMAT,
			   NETSNMP_OID_OUTPUT_FULL);
      }
      /* Setting UseNumeric forces UseLongNames on so check for UseNumeric
	 after UseLongNames (above) to make sure the final outcome of 
	 NETSNMP_DS_LIB_OID_OUTPUT_FORMAT is NETSNMP_OID_OUTPUT_NUMERIC */
      if (py_netsnmp_attr_long(session, "UseNumeric")) {
	getlabel_flag |= USE_LONG_NAMES;
	getlabel_flag |= USE_NUMERIC_OIDS;

	netsnmp_ds_set_int(NETSNMP_DS_LIBRARY_ID,
			   NETSNMP_DS_LIB_OID_OUTPUT_FORMAT,
			   NETSNMP_OID_OUTPUT_NUMERIC);
      }

      /* create tuple in which to return results */
      val_tuple = PyTuple_New(0);

      if(response && response->variables) {
	/* clear varlist to receive response varbinds*/
	PySequence_DelSlice(varbinds, 0, PySequence_Length(varbinds));

        if (PyErr_Occurred()) {
            /* propagate error */
            if (verbose)
                printf("error: bulk: deleting old varbinds failed\n");
            snmp_free_pdu(pdu);
            goto done;
        }

	for(vars = response->variables, varbind_ind=0; 
	    vars; 
	    vars = vars->next_variable, varbind_ind++) {

	  varbind = py_netsnmp_construct_varbind();

	  if (PyObject_HasAttrString(varbind, "tag")) {
	    *str_buf = '.';
	    *(str_buf+1) = '\0';
	    out_len = 0;
	    buf_over = 0;
	    str_bufp = str_buf;
	    tp = netsnmp_sprint_realloc_objid_tree(&str_bufp, &str_buf_len,
						   &out_len, 0, &buf_over,
						   vars->name,vars->name_length);
	    str_buf[sizeof(str_buf)-1] = '\0';
	    if (__is_leaf(tp)) {
	      type = (tp->type ? tp->type : tp->parent->type);
	      getlabel_flag &= ~NON_LEAF_NAME;
	    } else {
	      getlabel_flag |= NON_LEAF_NAME;
	      type = __translate_asn_type(vars->type);
	    }

	    __get_label_iid(str_buf, &tag, &iid, getlabel_flag);

	    py_netsnmp_attr_set_string(varbind, "tag", tag, STRLEN(tag));
	    py_netsnmp_attr_set_string(varbind, "iid", iid, STRLEN(iid));

	    __get_type_str(type, type_str);

	    py_netsnmp_attr_set_string(varbind, "type", type_str, 
				       STRLEN(type_str));

	    len = __snprint_value(str_buf,sizeof(str_buf),
				  vars,tp,type,sprintval_flag);
	    str_buf[len] = '\0';

	    py_netsnmp_attr_set_string(varbind, "val", str_buf, len);

	    /* push varbind onto varbinds */
	    PyList_Append(varbinds, varbind);

	    /* save in return tuple as well - steals ref */
	    _PyTuple_Resize(&val_tuple, varbind_ind+1);
	    PyTuple_SetItem(val_tuple, varbind_ind, 
			       Py_BuildValue("s#", str_buf, len));

	    Py_DECREF(varbind);

	  } else {
	    PyObject *none = Py_BuildValue(""); /* new ref */
	    /* not sure why making vabind failed - should not happen*/
	    PyList_Append(varbinds, none); /* increments ref */
	    /* Return None for this variable. */
	    PyTuple_SetItem(val_tuple, varbind_ind, none); /* steals ref */
	  }	
	}
      }

      /* Reset the library's behavior for numeric/symbolic OID's. */
      netsnmp_ds_set_int(NETSNMP_DS_LIBRARY_ID,
			 NETSNMP_DS_LIB_OID_OUTPUT_FORMAT,
			 old_format);

      if (response) snmp_free_pdu(response);

      Py_DECREF(varbinds);

    }

    if (PyErr_Occurred()) {
      /* propagate error */
      if (verbose)
	printf("error: getbulk response processing: unknown python error");
      Py_DECREF(val_tuple);
      val_tuple = NULL;
    }
  }

 done:
  SAFE_FREE(oid_arr);
  return (val_tuple ? val_tuple : Py_BuildValue(""));
}

static PyObject *
netsnmp_set(PyObject *self, PyObject *args)
{
  PyObject *session;
  PyObject *varlist;
  PyObject *varbind;
  PyObject *ret = NULL;
  netsnmp_session *ss;
  netsnmp_pdu *pdu, *response;
  struct tree *tp;
  char *tag;
  char *iid;
  char *val;
  char *type_str;
  int len;
  oid *oid_arr;
  int oid_arr_len = MAX_OID_LEN;
  int type;
  u_char tmp_val_str[STR_BUF_SIZE];
  int use_enums;
  struct enum_list *ep;
  int verbose = py_netsnmp_verbose();
  int best_guess;
  int status;
  int err_ind;
  int err_num;
  char err_str[STR_BUF_SIZE];
	   
  oid_arr = calloc(MAX_OID_LEN, sizeof(oid));

  if (oid_arr && args) {

    if (!PyArg_ParseTuple(args, "OO", &session, &varlist)) {
      goto done;
    }

    ss = (SnmpSession *)py_netsnmp_attr_long(session, "sess_ptr");

    // PyObject_SetAttrString();
    strcpy(err_str, py_netsnmp_attr_string(session, "ErrorStr"));
    err_num = py_netsnmp_attr_long(session, "ErrorNum");
    err_ind = py_netsnmp_attr_long(session, "ErrorInd");

    use_enums = py_netsnmp_attr_long(session, "UseEnums");
	
    best_guess = py_netsnmp_attr_long(session, "BestGuess");
      
    pdu = snmp_pdu_create(SNMP_MSG_SET);

    if (varlist) {
      PyObject *varlist_iter = PyObject_GetIter(varlist);

      while (varlist_iter && (varbind = PyIter_Next(varlist_iter))) {
	tag = py_netsnmp_attr_string(varbind, "tag");
	iid = py_netsnmp_attr_string(varbind, "iid");

	tp = __tag2oid(tag, iid, oid_arr, &oid_arr_len, 
		       &type, best_guess);

	if (oid_arr_len==0) {
	  if (verbose)
	    printf("error: set: unknown object ID (%s)",
		 (tag?tag:"<null>"));
	  snmp_free_pdu(pdu);
	  goto done;
	}

	if (type == TYPE_UNKNOWN) {
	  type_str = py_netsnmp_attr_string(varbind, "type");
	  type = __translate_appl_type(type_str);
	  if (type == TYPE_UNKNOWN) {
	    if (verbose)
	      printf("error: set: no type found for object");
	    snmp_free_pdu(pdu);
	    goto done;
	  }
	}

	val = py_netsnmp_attr_string(varbind, "val");
	strcpy(tmp_val_str, val);
	if (type==TYPE_INTEGER && use_enums && tp && tp->enums) {
	  for(ep = tp->enums; ep; ep = ep->next) {
	    if (val && !strcmp(ep->label, val)) {
	      strcpy(tmp_val_str, ep->value);
	      break;
	    }
	  }
	}
	len = STRLEN(tmp_val_str);
	status = __add_var_val_str(pdu, oid_arr, oid_arr_len,
				tmp_val_str, len, type);

	if (verbose && status == FAILURE)
	  printf("error: set: adding variable/value to PDU");

	/* release reference when done */
	Py_DECREF(varbind);
      }

      Py_DECREF(varlist_iter);

      if (PyErr_Occurred()) {
	/* propagate error */
	if (verbose)
	  printf("error: set: unknown python error");
	snmp_free_pdu(pdu);
	goto done;
      }
    }

    status = __send_sync_pdu(ss, pdu, &response, NO_RETRY_NOSUCH, 
			     err_str, &err_num, &err_ind);

    if (response) snmp_free_pdu(response);

    if (status == STAT_SUCCESS)
      ret = Py_BuildValue("i",1); /* success, return True */
    else
      ret = Py_BuildValue("i",0); /* success, return False */
  } 
 done:
  SAFE_FREE(oid_arr);
  return (ret ? ret : Py_BuildValue(""));
}


static PyMethodDef ClientMethods[] = {
  {"session",  netsnmp_create_session, METH_VARARGS,
   "create a netsnmp session."},
  {"session_v3",  netsnmp_create_session_v3, METH_VARARGS,
   "create a netsnmp session."},
  {"delete_session",  netsnmp_delete_session, METH_VARARGS,
   "create a netsnmp session."},
  {"get",  netsnmp_get, METH_VARARGS,
   "perform an SNMP GET operation."},
  {"getnext",  netsnmp_getnext, METH_VARARGS,
   "perform an SNMP GETNEXT operation."},
  {"getbulk",  netsnmp_getbulk, METH_VARARGS,
   "perform an SNMP GETBULK operation."},
  {"set",  netsnmp_set, METH_VARARGS,
   "perform an SNMP SET operation."},
  {"walk",  netsnmp_walk, METH_VARARGS,
   "perform an SNMP WALK operation."},
  {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initclient_intf(void)
{
    (void) Py_InitModule("client_intf", ClientMethods);
}





