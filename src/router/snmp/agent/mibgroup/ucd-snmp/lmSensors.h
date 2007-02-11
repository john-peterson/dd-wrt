/*
 * This file was generated by mib2c and is intended for use as a mib module
 * for the ucd-snmp snmpd agent. 
 */

#ifndef _MIBGROUP_LMSENSORS_H
#define _MIBGROUP_LMSENSORS_H

config_require(util_funcs)
config_add_mib(LM-SENSORS-MIB)

     void            init_lmSensors(void);
     void            shutdown_lmSensors(void);
     FindVarMethod   var_lmSensorsTable;

#endif                          /* _MIBGROUP_LMSENSORS_H */
