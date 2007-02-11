/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.48 $ of : mfd-top.m2c,v $
 *
 * $Id: inetCidrRouteTable.h,v 1.13 2006/01/25 16:25:54 dts12 Exp $
 */
#ifndef INETCIDRROUTETABLE_H
#define INETCIDRROUTETABLE_H

#ifdef __cplusplus
extern          "C" {
#endif


/** @addtogroup misc misc: Miscellaneous routines
 *
 * @{
 */
#include <net-snmp/library/asn1.h>
#include <net-snmp/data_access/route.h>

    /*
     * other required module components 
     */
    /* *INDENT-OFF*  */
config_require(ip-forward-mib/data_access/route)
config_require(ip-forward-mib/inetCidrRouteTable/inetCidrRouteTable_interface)
config_require(ip-forward-mib/inetCidrRouteTable/inetCidrRouteTable_data_access)
    /* *INDENT-ON*  */

    /*
     * OID, column number and enum definions for inetCidrRouteTable 
     */
#include "inetCidrRouteTable_constants.h"

    /*
     *********************************************************************
     * function declarations
     */
    void            init_inetCidrRouteTable(void);
    void            shutdown_inetCidrRouteTable(void);

    /*
     *********************************************************************
     * Table declarations
     */
/**********************************************************************
 **********************************************************************
 ***
 *** Table inetCidrRouteTable
 ***
 **********************************************************************
 **********************************************************************/
    /*
     * IP-FORWARD-MIB::inetCidrRouteTable is subid 7 of ipForward.
     * Its status is Current.
     * OID: .1.3.6.1.2.1.4.24.7, length: 9
     */
    /*
     *********************************************************************
     * When you register your mib, you get to provide a generic
     * pointer that will be passed back to you for most of the
     * functions calls.
     *
     * TODO:100:r: Review all context structures
     */
    /*
     * TODO:101:o: |-> Review inetCidrRouteTable registration context.
     */
    typedef netsnmp_data_list inetCidrRouteTable_registration;

/**********************************************************************/
    /*
     * TODO:110:r: |-> Review inetCidrRouteTable data context structure.
     * This structure is used to represent the data for inetCidrRouteTable.
     */
    typedef netsnmp_route_entry inetCidrRouteTable_data;


    /*
     *********************************************************************
     * TODO:115:o: |-> Review inetCidrRouteTable undo context.
     * We're just going to use the same data structure for our
     * undo_context. If you want to do something more efficent,
     * define your typedef here.
     */
    typedef inetCidrRouteTable_data inetCidrRouteTable_undo_data;

    /*
     * TODO:120:r: |-> Review inetCidrRouteTable mib index.
     * This structure is used to represent the index for inetCidrRouteTable.
     */
    typedef struct inetCidrRouteTable_mib_index_s {

        /*
         * inetCidrRouteDestType(1)/InetAddressType/ASN_INTEGER/long(u_long)//l/a/w/E/r/d/h
         */
        u_long          inetCidrRouteDestType;

        /*
         * inetCidrRouteDest(2)/InetAddress/ASN_OCTET_STR/char(char)//L/a/w/e/R/d/h
         */
        char
                     inetCidrRouteDest[NETSNMP_ACCESS_ROUTE_ADDR_BUF_SIZE];
        size_t          inetCidrRouteDest_len;

        /*
         * inetCidrRoutePfxLen(3)/InetAddressPrefixLength/ASN_UNSIGNED/u_long(u_long)//l/a/w/e/R/d/H
         */
        u_long          inetCidrRoutePfxLen;

        /*
         * inetCidrRoutePolicy(4)/OBJECTID/ASN_OBJECT_ID/oid(oid)//L/a/w/e/r/d/h
         */
        oid             inetCidrRoutePolicy[NETSNMP_POLICY_OID_MAX_LEN];
        size_t          inetCidrRoutePolicy_len;

        /*
         * inetCidrRouteNextHopType(5)/InetAddressType/ASN_INTEGER/long(u_long)//l/a/w/E/r/d/h
         */
        u_long          inetCidrRouteNextHopType;

        /*
         * inetCidrRouteNextHop(6)/InetAddress/ASN_OCTET_STR/char(char)//L/a/w/e/R/d/h
         */
        char
                  inetCidrRouteNextHop[NETSNMP_ACCESS_ROUTE_ADDR_BUF_SIZE];
        size_t          inetCidrRouteNextHop_len;


    } inetCidrRouteTable_mib_index;

    /*
     * TODO:121:r: |   |-> Review inetCidrRouteTable max index length.
     * If you KNOW that your indexes will never exceed a certain
     * length, update this macro to that length.
     *
     * BE VERY CAREFUL TO TAKE INTO ACCOUNT THE MAXIMUM
     * POSSIBLE LENGHT FOR EVERY VARIABLE LENGTH INDEX!
     */
#define MAX_inetCidrRouteTable_IDX_LEN (((NETSNMP_ACCESS_ROUTE_ADDR_BUF_SIZE+1)*2)+ 3 + (NETSNMP_POLICY_OID_MAX_LEN + 1))


    /*
     *********************************************************************
     * TODO:130:o: |-> Review inetCidrRouteTable Row request (rowreq) context.
     * When your functions are called, you will be passed a
     * inetCidrRouteTable_rowreq_ctx pointer.
     */
    typedef struct inetCidrRouteTable_rowreq_ctx_s {

    /** this must be first for container compare to work */
        netsnmp_index   oid_idx;
        oid             oid_tmp[MAX_inetCidrRouteTable_IDX_LEN];

        inetCidrRouteTable_mib_index tbl_idx;

        inetCidrRouteTable_data *data;
        inetCidrRouteTable_undo_data *undo;
        unsigned int    column_set_flags;       /* flags for set columns */


        /*
         * flags per row. Currently, the first (lower) 8 bits are reserved
         * for the user. See mfd.h for other flags.
         */
        u_int           rowreq_flags;

        /*
         * TODO:131:o: |   |-> Add useful data to inetCidrRouteTable rowreq context.
         */
        u_char          row_status;
        u_char          row_status_undo;

        /*
         * storage for future expansion
         */
        netsnmp_data_list *inetCidrRouteTable_data_list;

    } inetCidrRouteTable_rowreq_ctx;

    typedef struct inetCidrRouteTable_ref_rowreq_ctx_s {
        inetCidrRouteTable_rowreq_ctx *rowreq_ctx;
    } inetCidrRouteTable_ref_rowreq_ctx;

    /*
     *********************************************************************
     * function prototypes
     */
    int
        inetCidrRouteTable_pre_request(inetCidrRouteTable_registration *
                                       user_context);
    int
        inetCidrRouteTable_post_request(inetCidrRouteTable_registration *
                                        user_context, int rc);

    int
        inetCidrRouteTable_rowreq_ctx_init(inetCidrRouteTable_rowreq_ctx *
                                           rowreq_ctx,
                                           void *user_init_ctx);
    void
        inetCidrRouteTable_rowreq_ctx_cleanup(inetCidrRouteTable_rowreq_ctx
                                              * rowreq_ctx);

    inetCidrRouteTable_data *inetCidrRouteTable_allocate_data(void);
    void            inetCidrRouteTable_release_data(inetCidrRouteTable_data
                                                    * data);

    int
        inetCidrRouteTable_check_dependencies(inetCidrRouteTable_rowreq_ctx
                                              * rowreq_ctx);
    int             inetCidrRouteTable_commit(inetCidrRouteTable_rowreq_ctx
                                              * rowreq_ctx);

         
    inetCidrRouteTable_rowreq_ctx
        * inetCidrRouteTable_row_find_by_mib_index
        (inetCidrRouteTable_mib_index * mib_idx);

    extern oid      inetCidrRouteTable_oid[];
    extern int      inetCidrRouteTable_oid_size;


#include "inetCidrRouteTable_interface.h"
#include "inetCidrRouteTable_data_access.h"
    /*
     *********************************************************************
     * GET function declarations
     */

    /*
     *********************************************************************
     * GET Table declarations
     */
/**********************************************************************
 **********************************************************************
 ***
 *** Table inetCidrRouteTable
 ***
 **********************************************************************
 **********************************************************************/
    /*
     * IP-FORWARD-MIB::inetCidrRouteTable is subid 7 of ipForward.
     * Its status is Current.
     * OID: .1.3.6.1.2.1.4.24.7, length: 9
     */
    /*
     * indexes
     */

    int             inetCidrRouteIfIndex_get(inetCidrRouteTable_rowreq_ctx
                                             * rowreq_ctx, long
                                             *inetCidrRouteIfIndex_val_ptr);
    int             inetCidrRouteType_get(inetCidrRouteTable_rowreq_ctx *
                                          rowreq_ctx,
                                          u_long *
                                          inetCidrRouteType_val_ptr);
    int             inetCidrRouteProto_get(inetCidrRouteTable_rowreq_ctx *
                                           rowreq_ctx,
                                           u_long *
                                           inetCidrRouteProto_val_ptr);
    int             inetCidrRouteAge_get(inetCidrRouteTable_rowreq_ctx *
                                         rowreq_ctx,
                                         u_long *
                                         inetCidrRouteAge_val_ptr);
    int
        inetCidrRouteNextHopAS_get(inetCidrRouteTable_rowreq_ctx *
                                   rowreq_ctx,
                                   u_long *
                                   inetCidrRouteNextHopAS_val_ptr);
    int             inetCidrRouteMetric1_get(inetCidrRouteTable_rowreq_ctx
                                             * rowreq_ctx, long
                                             *inetCidrRouteMetric1_val_ptr);
    int             inetCidrRouteMetric2_get(inetCidrRouteTable_rowreq_ctx
                                             * rowreq_ctx, long
                                             *inetCidrRouteMetric2_val_ptr);
    int             inetCidrRouteMetric3_get(inetCidrRouteTable_rowreq_ctx
                                             * rowreq_ctx, long
                                             *inetCidrRouteMetric3_val_ptr);
    int             inetCidrRouteMetric4_get(inetCidrRouteTable_rowreq_ctx
                                             * rowreq_ctx, long
                                             *inetCidrRouteMetric4_val_ptr);
    int             inetCidrRouteMetric5_get(inetCidrRouteTable_rowreq_ctx
                                             * rowreq_ctx, long
                                             *inetCidrRouteMetric5_val_ptr);
    int             inetCidrRouteStatus_get(inetCidrRouteTable_rowreq_ctx *
                                            rowreq_ctx,
                                            u_long *
                                            inetCidrRouteStatus_val_ptr);


    int
        inetCidrRouteTable_indexes_set_tbl_idx(inetCidrRouteTable_mib_index
                                               * tbl_idx,
                                               u_long
                                               inetCidrRouteDestType_val,
                                               char
                                               *inetCidrRouteDest_val_ptr,
                                               size_t
                                               inetCidrRouteDest_val_ptr_len,
                                               u_long
                                               inetCidrRoutePfxLen_val,
                                               oid *
                                               inetCidrRoutePolicy_val_ptr,
                                               size_t
                                               inetCidrRoutePolicy_val_ptr_len,
                                               u_long
                                               inetCidrRouteNextHopType_val,
                                               char
                                               *inetCidrRouteNextHop_val_ptr,
                                               size_t
                                               inetCidrRouteNextHop_val_ptr_len);
    int
        inetCidrRouteTable_indexes_set(inetCidrRouteTable_rowreq_ctx *
                                       rowreq_ctx,
                                       u_long inetCidrRouteDestType_val,
                                       char *inetCidrRouteDest_val_ptr,
                                       size_t
                                       inetCidrRouteDest_val_ptr_len,
                                       u_long inetCidrRoutePfxLen_val,
                                       oid * inetCidrRoutePolicy_val_ptr,
                                       size_t
                                       inetCidrRoutePolicy_val_ptr_len,
                                       u_long inetCidrRouteNextHopType_val,
                                       char *inetCidrRouteNextHop_val_ptr,
                                       size_t
                                       inetCidrRouteNextHop_val_ptr_len);



    /*
     *********************************************************************
     * SET function declarations
     */

    /*
     *********************************************************************
     * SET Table declarations
     */
/**********************************************************************
 **********************************************************************
 ***
 *** Table inetCidrRouteTable
 ***
 **********************************************************************
 **********************************************************************/
    /*
     * IP-FORWARD-MIB::inetCidrRouteTable is subid 7 of ipForward.
     * Its status is Current.
     * OID: .1.3.6.1.2.1.4.24.7, length: 9
     */


    int
        inetCidrRouteTable_undo_setup(inetCidrRouteTable_rowreq_ctx *
                                      rowreq_ctx);
    int
        inetCidrRouteTable_undo_cleanup(inetCidrRouteTable_rowreq_ctx *
                                        rowreq_ctx);
    int             inetCidrRouteTable_undo(inetCidrRouteTable_rowreq_ctx *
                                            rowreq_ctx);
    int             inetCidrRouteTable_commit(inetCidrRouteTable_rowreq_ctx
                                              * rowreq_ctx);
    int
        inetCidrRouteTable_undo_commit(inetCidrRouteTable_rowreq_ctx *
                                       rowreq_ctx);


    int
        inetCidrRouteIfIndex_check_value(inetCidrRouteTable_rowreq_ctx *
                                         rowreq_ctx,
                                         long inetCidrRouteIfIndex_val);
    int
        inetCidrRouteIfIndex_undo_setup(inetCidrRouteTable_rowreq_ctx *
                                        rowreq_ctx);
    int             inetCidrRouteIfIndex_set(inetCidrRouteTable_rowreq_ctx
                                             * rowreq_ctx, long
                                             inetCidrRouteIfIndex_val);
    int             inetCidrRouteIfIndex_undo(inetCidrRouteTable_rowreq_ctx
                                              * rowreq_ctx);

    int
        inetCidrRouteType_check_value(inetCidrRouteTable_rowreq_ctx *
                                      rowreq_ctx,
                                      u_long inetCidrRouteType_val);
    int
        inetCidrRouteType_undo_setup(inetCidrRouteTable_rowreq_ctx *
                                     rowreq_ctx);
    int             inetCidrRouteType_set(inetCidrRouteTable_rowreq_ctx *
                                          rowreq_ctx,
                                          u_long inetCidrRouteType_val);
    int             inetCidrRouteType_undo(inetCidrRouteTable_rowreq_ctx *
                                           rowreq_ctx);

    int
        inetCidrRouteProto_check_value(inetCidrRouteTable_rowreq_ctx *
                                       rowreq_ctx,
                                       u_long inetCidrRouteProto_val);
    int
        inetCidrRouteProto_undo_setup(inetCidrRouteTable_rowreq_ctx *
                                      rowreq_ctx);
    int             inetCidrRouteProto_set(inetCidrRouteTable_rowreq_ctx *
                                           rowreq_ctx,
                                           u_long inetCidrRouteProto_val);
    int             inetCidrRouteProto_undo(inetCidrRouteTable_rowreq_ctx *
                                            rowreq_ctx);

    int
        inetCidrRouteAge_check_value(inetCidrRouteTable_rowreq_ctx *
                                     rowreq_ctx,
                                     u_long inetCidrRouteAge_val);
    int
        inetCidrRouteAge_undo_setup(inetCidrRouteTable_rowreq_ctx *
                                    rowreq_ctx);
    int             inetCidrRouteAge_set(inetCidrRouteTable_rowreq_ctx *
                                         rowreq_ctx,
                                         u_long inetCidrRouteAge_val);
    int             inetCidrRouteAge_undo(inetCidrRouteTable_rowreq_ctx *
                                          rowreq_ctx);

    int
        inetCidrRouteNextHopAS_check_value(inetCidrRouteTable_rowreq_ctx *
                                           rowreq_ctx,
                                           u_long
                                           inetCidrRouteNextHopAS_val);
    int
        inetCidrRouteNextHopAS_undo_setup(inetCidrRouteTable_rowreq_ctx *
                                          rowreq_ctx);
    int
        inetCidrRouteNextHopAS_set(inetCidrRouteTable_rowreq_ctx *
                                   rowreq_ctx,
                                   u_long inetCidrRouteNextHopAS_val);
    int
        inetCidrRouteNextHopAS_undo(inetCidrRouteTable_rowreq_ctx *
                                    rowreq_ctx);

    int
        inetCidrRouteMetric1_check_value(inetCidrRouteTable_rowreq_ctx *
                                         rowreq_ctx,
                                         long inetCidrRouteMetric1_val);
    int
        inetCidrRouteMetric1_undo_setup(inetCidrRouteTable_rowreq_ctx *
                                        rowreq_ctx);
    int             inetCidrRouteMetric1_set(inetCidrRouteTable_rowreq_ctx
                                             * rowreq_ctx, long
                                             inetCidrRouteMetric1_val);
    int             inetCidrRouteMetric1_undo(inetCidrRouteTable_rowreq_ctx
                                              * rowreq_ctx);

    int
        inetCidrRouteMetric2_check_value(inetCidrRouteTable_rowreq_ctx *
                                         rowreq_ctx,
                                         long inetCidrRouteMetric2_val);
    int
        inetCidrRouteMetric2_undo_setup(inetCidrRouteTable_rowreq_ctx *
                                        rowreq_ctx);
    int             inetCidrRouteMetric2_set(inetCidrRouteTable_rowreq_ctx
                                             * rowreq_ctx, long
                                             inetCidrRouteMetric2_val);
    int             inetCidrRouteMetric2_undo(inetCidrRouteTable_rowreq_ctx
                                              * rowreq_ctx);

    int
        inetCidrRouteMetric3_check_value(inetCidrRouteTable_rowreq_ctx *
                                         rowreq_ctx,
                                         long inetCidrRouteMetric3_val);
    int
        inetCidrRouteMetric3_undo_setup(inetCidrRouteTable_rowreq_ctx *
                                        rowreq_ctx);
    int             inetCidrRouteMetric3_set(inetCidrRouteTable_rowreq_ctx
                                             * rowreq_ctx, long
                                             inetCidrRouteMetric3_val);
    int             inetCidrRouteMetric3_undo(inetCidrRouteTable_rowreq_ctx
                                              * rowreq_ctx);

    int
        inetCidrRouteMetric4_check_value(inetCidrRouteTable_rowreq_ctx *
                                         rowreq_ctx,
                                         long inetCidrRouteMetric4_val);
    int
        inetCidrRouteMetric4_undo_setup(inetCidrRouteTable_rowreq_ctx *
                                        rowreq_ctx);
    int             inetCidrRouteMetric4_set(inetCidrRouteTable_rowreq_ctx
                                             * rowreq_ctx, long
                                             inetCidrRouteMetric4_val);
    int             inetCidrRouteMetric4_undo(inetCidrRouteTable_rowreq_ctx
                                              * rowreq_ctx);

    int
        inetCidrRouteMetric5_check_value(inetCidrRouteTable_rowreq_ctx *
                                         rowreq_ctx,
                                         long inetCidrRouteMetric5_val);
    int
        inetCidrRouteMetric5_undo_setup(inetCidrRouteTable_rowreq_ctx *
                                        rowreq_ctx);
    int             inetCidrRouteMetric5_set(inetCidrRouteTable_rowreq_ctx
                                             * rowreq_ctx, long
                                             inetCidrRouteMetric5_val);
    int             inetCidrRouteMetric5_undo(inetCidrRouteTable_rowreq_ctx
                                              * rowreq_ctx);

    int
        inetCidrRouteStatus_check_value(inetCidrRouteTable_rowreq_ctx *
                                        rowreq_ctx,
                                        u_long inetCidrRouteStatus_val);
    int
        inetCidrRouteStatus_undo_setup(inetCidrRouteTable_rowreq_ctx *
                                       rowreq_ctx);
    int             inetCidrRouteStatus_set(inetCidrRouteTable_rowreq_ctx *
                                            rowreq_ctx,
                                            u_long
                                            inetCidrRouteStatus_val);
    int             inetCidrRouteStatus_undo(inetCidrRouteTable_rowreq_ctx
                                             * rowreq_ctx);


    int
        inetCidrRouteTable_check_dependencies(inetCidrRouteTable_rowreq_ctx
                                              * ctx);


    /*
     * DUMMY markers, ignore
     *
     * TODO:099:x: *************************************************************
     * TODO:199:x: *************************************************************
     * TODO:299:x: *************************************************************
     * TODO:399:x: *************************************************************
     * TODO:499:x: *************************************************************
     */

#ifdef __cplusplus
}
#endif
#endif                          /* INETCIDRROUTETABLE_H */
/**  @} */

