/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.check_values.conf,v 1.1 2003/02/06 06:07:12 hardaker Exp $
 */

/*
 * standard headers 
 */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include "ipCidrRouteTable_checkfns.h"
#include "ipCidrRouteTable_enums.h"

/** Decides if an incoming value for the ipCidrRouteIfIndex mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
int
check_ipCidrRouteIfIndex(int type, long *val, size_t val_len)
{

    /** Check to see that we were called legally */
    if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
    if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;


    /** looks ok */
    return SNMP_ERR_NOERROR;
}

/** Decides if an incoming value for the ipCidrRouteType mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
int
check_ipCidrRouteType(int type, long *val, size_t val_len)
{

    /** Check to see that we were called legally */
    if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
    if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;

    /** Check the enums.  Legal values continue. */
    switch (*val) {
    case IPCIDRROUTETYPE_OTHER:
    case IPCIDRROUTETYPE_REJECT:
    case IPCIDRROUTETYPE_LOCAL:
    case IPCIDRROUTETYPE_REMOTE:
        break;

    /** not a legal enum value.  return an error */
    default:
        return SNMP_ERR_INCONSISTENTVALUE;
    }

    /** looks ok */
    return SNMP_ERR_NOERROR;
}

/** Decides if an incoming value for the ipCidrRouteInfo mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
int
check_ipCidrRouteInfo(int type, oid * val, size_t val_len)
{

    /** Check to see that we were called legally */
    if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
    if (type != ASN_OBJECT_ID)
        return SNMP_ERR_WRONGTYPE;

    /** looks ok */
    return SNMP_ERR_NOERROR;
}

/** Decides if an incoming value for the ipCidrRouteNextHopAS mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
int
check_ipCidrRouteNextHopAS(int type, long *val, size_t val_len)
{

    /** Check to see that we were called legally */
    if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
    if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;


    /** looks ok */
    return SNMP_ERR_NOERROR;
}

/** Decides if an incoming value for the ipCidrRouteMetric1 mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
int
check_ipCidrRouteMetric1(int type, long *val, size_t val_len)
{

    /** Check to see that we were called legally */
    if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
    if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;


    /** looks ok */
    return SNMP_ERR_NOERROR;
}

/** Decides if an incoming value for the ipCidrRouteMetric2 mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
int
check_ipCidrRouteMetric2(int type, long *val, size_t val_len)
{

    /** Check to see that we were called legally */
    if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
    if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;


    /** looks ok */
    return SNMP_ERR_NOERROR;
}

/** Decides if an incoming value for the ipCidrRouteMetric3 mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
int
check_ipCidrRouteMetric3(int type, long *val, size_t val_len)
{

    /** Check to see that we were called legally */
    if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
    if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;


    /** looks ok */
    return SNMP_ERR_NOERROR;
}

/** Decides if an incoming value for the ipCidrRouteMetric4 mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
int
check_ipCidrRouteMetric4(int type, long *val, size_t val_len)
{

    /** Check to see that we were called legally */
    if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
    if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;


    /** looks ok */
    return SNMP_ERR_NOERROR;
}

/** Decides if an incoming value for the ipCidrRouteMetric5 mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
int
check_ipCidrRouteMetric5(int type, long *val, size_t val_len)
{

    /** Check to see that we were called legally */
    if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
    if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;


    /** looks ok */
    return SNMP_ERR_NOERROR;
}

/** Decides if an incoming value for the ipCidrRouteStatus mib node is legal.
 *  @param type    The incoming data type.
 *  @param val     The value to be checked.
 *  @param val_len The length of data stored in val (in bytes).
 *  @return 0 if the incoming value is legal, an SNMP error code otherwise.
 */
int
check_ipCidrRouteStatus(int type, long *val, size_t val_len)
{

    /** Check to see that we were called legally */
    if (!val)
        return SNMP_ERR_GENERR;

    /** Check the incoming type for correctness */
    if (type != ASN_INTEGER)
        return SNMP_ERR_WRONGTYPE;

    /** Check the enums.  Legal values continue. */
    switch (*val) {
    case IPCIDRROUTESTATUS_ACTIVE:
    case IPCIDRROUTESTATUS_NOTINSERVICE:
    case IPCIDRROUTESTATUS_NOTREADY:
    case IPCIDRROUTESTATUS_CREATEANDGO:
    case IPCIDRROUTESTATUS_CREATEANDWAIT:
    case IPCIDRROUTESTATUS_DESTROY:
        break;

    /** not a legal enum value.  return an error */
    default:
        return SNMP_ERR_INCONSISTENTVALUE;
    }

    /** looks ok */
    return SNMP_ERR_NOERROR;
}
