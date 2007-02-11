/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.iterate_access.conf
 */
#ifndef IPCIDRROUTETABLE_H
#define IPCIDRROUTETABLE_H

/** other required module components */
config_require(mibII/ipCidrRouteTable_access)
config_require(mibII/ipCidrRouteTable_checkfns)
config_add_mib(IP-FORWARD-MIB)
config_add_mib(IANA-RTPROTO-MIB)

/*
 * function declarations 
 */
void            init_ipCidrRouteTable(void);
void            initialize_table_ipCidrRouteTable(void);
Netsnmp_Node_Handler ipCidrRouteTable_handler;

Netsnmp_First_Data_Point ipCidrRouteTable_get_first_data_point;
Netsnmp_Next_Data_Point ipCidrRouteTable_get_next_data_point;

/*
 * column number definitions for table ipCidrRouteTable 
 */
#include "ipCidrRouteTable_columns.h"

/*
 * enum definions 
 */
#include "ipCidrRouteTable_enums.h"

#endif /** IPCIDRROUTETABLE_H */
