/*
 * Note: this file originally auto-generated by mib2c using
 *  : generic-table-constants.m2c,v 1.5 2005/07/15 22:41:16 rstory Exp $
 *
 * $Id: ipAddressPrefixTable_constants.h,v 1.2 2005/12/01 15:49:38 rstory Exp $
 */
#ifndef IPADDRESSPREFIXTABLE_CONSTANTS_H
#define IPADDRESSPREFIXTABLE_CONSTANTS_H

#ifdef __cplusplus
extern          "C" {
#endif


    /*
     * column number definitions for table ipAddressPrefixTable 
     */
#define IPADDRESSPREFIXTABLE_OID              1,3,6,1,2,1,4,32

#define COLUMN_IPADDRESSPREFIXIFINDEX         1

#define COLUMN_IPADDRESSPREFIXTYPE         2

#define COLUMN_IPADDRESSPREFIXPREFIX         3

#define COLUMN_IPADDRESSPREFIXLENGTH         4

#define COLUMN_IPADDRESSPREFIXORIGIN         5

#define COLUMN_IPADDRESSPREFIXONLINKFLAG         6

#define COLUMN_IPADDRESSPREFIXAUTONOMOUSFLAG         7

#define COLUMN_IPADDRESSPREFIXADVPREFERREDLIFETIME         8

#define COLUMN_IPADDRESSPREFIXADVVALIDLIFETIME         9


#define IPADDRESSPREFIXTABLE_MIN_COL   COLUMN_IPADDRESSPREFIXORIGIN
#define IPADDRESSPREFIXTABLE_MAX_COL   COLUMN_IPADDRESSPREFIXADVVALIDLIFETIME


    /*
     * NOTES on enums
     * ==============
     *
     * Value Mapping
     * -------------
     * If the values for your data type don't exactly match the
     * possible values defined by the mib, you should map them
     * below. For example, a boolean flag (1/0) is usually represented
     * as a TruthValue in a MIB, which maps to the values (1/2).
     *
     */
/*************************************************************************
 *************************************************************************
 *
 * enum definitions for table ipAddressPrefixTable
 *
 *************************************************************************
 *************************************************************************/

/*************************************************************
 * constants for enums for the MIB node
 * ipAddressPrefixType (InetAddressType / ASN_INTEGER)
 *
 * since a Textual Convention may be referenced more than once in a
 * MIB, protect againt redefinitions of the enum values.
 */
#ifndef INETADDRESSTYPE_ENUMS
#define INETADDRESSTYPE_ENUMS

#define INETADDRESSTYPE_UNKNOWN  0
#define INETADDRESSTYPE_IPV4  1
#define INETADDRESSTYPE_IPV6  2
#define INETADDRESSTYPE_IPV4Z  3
#define INETADDRESSTYPE_IPV6Z  4
#define INETADDRESSTYPE_DNS  16

#endif                          /* INETADDRESSTYPE_ENUMS */


/*************************************************************
 * constants for enums for the MIB node
 * ipAddressPrefixOrigin (IpAddressPrefixOriginTC / ASN_INTEGER)
 *
 * since a Textual Convention may be referenced more than once in a
 * MIB, protect againt redefinitions of the enum values.
 */
#ifndef IPADDRESSPREFIXORIGINTC_ENUMS
#define IPADDRESSPREFIXORIGINTC_ENUMS

#define IPADDRESSPREFIXORIGINTC_OTHER  1
#define IPADDRESSPREFIXORIGINTC_MANUAL  2
#define IPADDRESSPREFIXORIGINTC_WELLKNOWN  3
#define IPADDRESSPREFIXORIGINTC_DHCP  4
#define IPADDRESSPREFIXORIGINTC_ROUTERADV  5

#endif                          /* IPADDRESSPREFIXORIGINTC_ENUMS */


/*************************************************************
 * constants for enums for the MIB node
 * ipAddressPrefixOnLinkFlag (TruthValue / ASN_INTEGER)
 *
 * since a Textual Convention may be referenced more than once in a
 * MIB, protect againt redefinitions of the enum values.
 */
#ifndef TRUTHVALUE_ENUMS
#define TRUTHVALUE_ENUMS

#define TRUTHVALUE_TRUE  1
#define TRUTHVALUE_FALSE  2

#endif                          /* TRUTHVALUE_ENUMS */


/*************************************************************
 * constants for enums for the MIB node
 * ipAddressPrefixAutonomousFlag (TruthValue / ASN_INTEGER)
 *
 * since a Textual Convention may be referenced more than once in a
 * MIB, protect againt redefinitions of the enum values.
 */
#ifndef TRUTHVALUE_ENUMS
#define TRUTHVALUE_ENUMS

#define TRUTHVALUE_TRUE  1
#define TRUTHVALUE_FALSE  2

#endif                          /* TRUTHVALUE_ENUMS */




#ifdef __cplusplus
}
#endif
#endif                          /* IPADDRESSPREFIXTABLE_OIDS_H */
