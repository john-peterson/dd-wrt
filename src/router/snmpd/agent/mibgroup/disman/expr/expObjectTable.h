/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.table_data.conf,v 1.11 2005/09/16 11:48:12 dts12 Exp $
 */
#ifndef EXPOBJECTTABLE_H
#define EXPOBJECTTABLE_H

/*
 * function declarations 
 */
void            init_expObjectTable(void);
Netsnmp_Node_Handler expObjectTable_handler;

/*
 * column number definitions for table expObjectTable 
 */
#define COLUMN_EXPOBJECTINDEX			1
#define COLUMN_EXPOBJECTID			2
#define COLUMN_EXPOBJECTIDWILDCARD		3
#define COLUMN_EXPOBJECTSAMPLETYPE		4
#define COLUMN_EXPOBJECTDELTADISCONTINUITYID	5
#define COLUMN_EXPOBJECTDISCONTINUITYIDWILDCARD	6
#define COLUMN_EXPOBJECTDISCONTINUITYIDTYPE	7
#define COLUMN_EXPOBJECTCONDITIONAL		8
#define COLUMN_EXPOBJECTCONDITIONALWILDCARD	9
#define COLUMN_EXPOBJECTENTRYSTATUS		10
#endif                          /* EXPOBJECTTABLE_H */
