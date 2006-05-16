/*
 * usmStats.h
 *
 * This file was generated by mib2c and is intended for use as a mib module
 * for the ucd-snmp snmpd agent.
 */

#ifndef _MIBGROUP_USMSTATS_H
#define _MIBGROUP_USMSTATS_H

/*
 * we use header_generic and checkmib from the util_funcs module 
 */

config_require(util_funcs)
config_add_mib(SNMP-USER-BASED-SM-MIB)

    /*
     * Magic number definitions: 
     */
#define   USMSTATSUNSUPPORTEDSECLEVELS  0
#define   USMSTATSNOTINTIMEWINDOWS      1
#define   USMSTATSUNKNOWNUSERNAMES      2
#define   USMSTATSUNKNOWNENGINEIDS      3
#define   USMSTATSWRONGDIGESTS          4
#define   USMSTATSDECRYPTIONERRORS      5
    /*
     * function definitions 
     */
     extern void     init_usmStats(void);
     extern FindVarMethod var_usmStats;


#endif                          /* _MIBGROUP_USMSTATS_H */
