#ifndef ALL_HANDLERS_H
#define ALL_HANDLERS_H

#ifdef __cplusplus
extern          "C" {
#endif

#include <net-snmp/agent/instance.h>
#include <net-snmp/agent/multiplexer.h>
#include <net-snmp/agent/null.h>
#include <net-snmp/agent/debug_handler.h>
#include <net-snmp/agent/old_api.h>
#include <net-snmp/agent/read_only.h>
#include <net-snmp/agent/serialize.h>
#include <net-snmp/agent/bulk_to_next.h>
#include <net-snmp/agent/mode_end_call.h>
/*
 * #include <net-snmp/agent/set_helper.h> 
 */
#include <net-snmp/agent/table.h>
#include <net-snmp/agent/table_data.h>
#include <net-snmp/agent/table_dataset.h>
#include <net-snmp/agent/table_iterator.h>
/*
 * #include <net-snmp/agent/table_array.h> 
 */

void            netsnmp_init_helpers(void);

#ifdef __cplusplus
}
#endif
#endif                          /* ALL_HANDLERS_H */
