/*
 * Note: this file originally auto-generated by mib2c using
 *        : : mib2c.check_values_local.conf,v 5.1 2003/05/30 23:53:15 hardaker Exp $
 * 
 */
#ifndef NETSNMPHOSTSTABLE_CHECKFNS_H
#define NETSNMPHOSTSTABLE_CHECKFNS_H

/*
 * these functions are designed to check incoming values for 
 * columns in the netSnmpHostsTable table for legality with respect to 
 * datatype and value according to local conventions.  You should modify
 * them as appropriate.  They will be called from parent check_value
 * functions that are auto-generated using mib2c and the parent functions
 * should NOT be modified.
 */

#ifdef __cplusplus
extern "C" {
#endif

int             check_(int type, long *val, size_t val_len, long *old_val,
                       size_t old_val_len);
int             check_(int type, char *val, size_t val_len, char *old_val,
                       size_t old_val_len);
int             check_(int type, long *val, size_t val_len, long *old_val,
                       size_t old_val_len);
int             check_(int type, long *val, size_t val_len, long *old_val,
                       size_t old_val_len);


#ifdef __cplusplus
}
#endif

#endif                          /* NETSNMPHOSTSTABLE_CHECKFNS_H */
