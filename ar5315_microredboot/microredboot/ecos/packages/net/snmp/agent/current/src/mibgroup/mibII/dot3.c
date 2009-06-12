//==========================================================================
//
//      snmp/snmpagent/current/src/mibgroup/mibII/dot3.c
//
//
//==========================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
//
// Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
// at http://sources.redhat.com/ecos/ecos-license/
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//####UCDSNMPCOPYRIGHTBEGIN####
//
// -------------------------------------------
//
// Portions of this software may have been derived from the UCD-SNMP
// project,  <http://ucd-snmp.ucdavis.edu/>  from the University of
// California at Davis, which was originally based on the Carnegie Mellon
// University SNMP implementation.  Portions of this software are therefore
// covered by the appropriate copyright disclaimers included herein.
//
// The release used was version 4.1.2 of May 2000.  "ucd-snmp-4.1.2"
// -------------------------------------------
//
//####UCDSNMPCOPYRIGHTEND####
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    hmt
// Contributors: hmt
// Date:         2000-05-30
// Purpose:      Port of UCD-SNMP distribution to eCos.
// Description:  
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================
/********************************************************************
       Copyright 1989, 1991, 1992 by Carnegie Mellon University
 
                          Derivative Work -
Copyright 1996, 1998, 1999, 2000 The Regents of the University of California
 
                         All Rights Reserved
 
Permission to use, copy, modify and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appears in all copies and
that both that copyright notice and this permission notice appear in
supporting documentation, and that the name of CMU and The Regents of
the University of California not be used in advertising or publicity
pertaining to distribution of the software without specific written
permission.
 
CMU AND THE REGENTS OF THE UNIVERSITY OF CALIFORNIA DISCLAIM ALL
WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL CMU OR
THE REGENTS OF THE UNIVERSITY OF CALIFORNIA BE LIABLE FOR ANY SPECIAL,
INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING
FROM THE LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*********************************************************************/
/* This file was generated by mib2c and is intended for use as a mib module
   for the ucd-snmp snmpd agent. */


/* This should always be included first before anything else */
#include <config.h>


/* minimal include directives */
#include "mibincl.h"
#include "mibgroup/util_funcs.h"
#include "mibgroup/mibII/dot3.h"
#include "mibgroup/mibII/ifr_helper.h"

#include <net/if_types.h>
#include <net/if.h>
#ifdef CYGPKG_NET_FREEBSD_STACK
#include <net/if_var.h>
#endif

// Get info about the device
#include <pkgconf/system.h>

// These two two acquire all the statistics.
#include <cyg/io/eth/eth_drv_stats.h>
#include <sys/sockio.h>
 
/* 
 * dot3_variables_oid:
 *   this is the top level oid that we want to register under.  This
 *   is essentially a prefix, with the suffix appearing in the
 *   variable below.
 */

oid dot3_variables_oid[] = { 1,3,6,1,2,1,10,7 };


extern struct ifnet_head ifnet;


/* 
 * variable4 dot3_variables:
 *   this variable defines function callbacks and type return information 
 *   for the dot3 mib section 
 */
struct variable4 dot3_variables[] = {
/*  magic number        , variable type , ro/rw , callback fn  , L, oidsuffix */
#define   DOT3STATSINDEX        3
  { DOT3STATSINDEX      , ASN_INTEGER   , RONLY , var_dot3StatsTable, 3, { 2,1,1 } },
#define   DOT3STATSALIGNMENTERRORS  4
  { DOT3STATSALIGNMENTERRORS, ASN_COUNTER   , RONLY , var_dot3StatsTable, 3, { 2,1,2 } },
#define   DOT3STATSFCSERRORS    5
  { DOT3STATSFCSERRORS  , ASN_COUNTER   , RONLY , var_dot3StatsTable, 3, { 2,1,3 } },
#define   DOT3STATSSINGLECOLLISIONFRAMES  6
  { DOT3STATSSINGLECOLLISIONFRAMES, ASN_COUNTER   , RONLY , var_dot3StatsTable, 3, { 2,1,4 } },
#define   DOT3STATSMULTIPLECOLLISIONFRAMES  7
  { DOT3STATSMULTIPLECOLLISIONFRAMES, ASN_COUNTER   , RONLY , var_dot3StatsTable, 3, { 2,1,5 } },
#define   DOT3STATSSQETESTERRORS  8
  { DOT3STATSSQETESTERRORS, ASN_COUNTER   , RONLY , var_dot3StatsTable, 3, { 2,1,6 } },
#define   DOT3STATSDEFERREDTRANSMISSIONS  9
  { DOT3STATSDEFERREDTRANSMISSIONS, ASN_COUNTER   , RONLY , var_dot3StatsTable, 3, { 2,1,7 } },
#define   DOT3STATSLATECOLLISIONS  10
  { DOT3STATSLATECOLLISIONS, ASN_COUNTER   , RONLY , var_dot3StatsTable, 3, { 2,1,8 } },
#define   DOT3STATSEXCESSIVECOLLISIONS  11
  { DOT3STATSEXCESSIVECOLLISIONS, ASN_COUNTER   , RONLY , var_dot3StatsTable, 3, { 2,1,9 } },
#define   DOT3STATSINTERNALMACTRANSMITERRORS  12
  { DOT3STATSINTERNALMACTRANSMITERRORS, ASN_COUNTER   , RONLY , var_dot3StatsTable, 3, { 2,1,10 } },
#define   DOT3STATSCARRIERSENSEERRORS  13
  { DOT3STATSCARRIERSENSEERRORS, ASN_COUNTER   , RONLY , var_dot3StatsTable, 3, { 2,1,11 } },
#define   DOT3STATSFRAMETOOLONGS  14
  { DOT3STATSFRAMETOOLONGS, ASN_COUNTER   , RONLY , var_dot3StatsTable, 3, { 2,1,13 } },
#define   DOT3STATSINTERNALMACRECEIVEERRORS  15
  { DOT3STATSINTERNALMACRECEIVEERRORS, ASN_COUNTER   , RONLY , var_dot3StatsTable, 3, { 2,1,16 } },
#define   DOT3STATSETHERCHIPSET  16
  { DOT3STATSETHERCHIPSET, ASN_OBJECT_ID , RONLY , var_dot3StatsTable, 3, { 2,1,17 } },
#define   DOT3STATSSYMBOLERRORS  17
  { DOT3STATSSYMBOLERRORS, ASN_COUNTER   , RONLY , var_dot3StatsTable, 3, { 2,1,18 } },
#define   DOT3STATSDUPLEXSTATUS  18
  { DOT3STATSDUPLEXSTATUS, ASN_INTEGER   , RONLY , var_dot3StatsTable, 3, { 2,1,19 } },

// We do not have this histogram available:
//#define   DOT3COLLFREQUENCIES   22
//  { DOT3COLLFREQUENCIES , ASN_COUNTER   , RONLY , var_dot3CollTable, 3, { 5,1,3 } },

//UNSUPPORTED: currently we do not get these statistics out
//#define   DOT3CONTROLFUNCTIONSSUPPORTED  25
//  { DOT3CONTROLFUNCTIONSSUPPORTED, ASN_BIT_STR , RONLY , var_dot3ControlTable, 3, { 9,1,1 } },
//#define   DOT3CONTROLINUNKNOWNOPCODES  26
//  { DOT3CONTROLINUNKNOWNOPCODES, ASN_COUNTER   , RONLY , var_dot3ControlTable, 3, { 9,1,2 } },


//UNSUPPORTED: currently we do not get these statistics out
//#define   DOT3PAUSEADMINMODE    29
//  { DOT3PAUSEADMINMODE  , ASN_INTEGER   , RWRITE, var_dot3PauseTable, 3, { 10,1,1 } },
//#define   DOT3PAUSEOPERMODE     30
//  { DOT3PAUSEOPERMODE   , ASN_INTEGER   , RONLY , var_dot3PauseTable, 3, { 10,1,2 } },
//#define   DOT3INPAUSEFRAMES     31
//  { DOT3INPAUSEFRAMES   , ASN_COUNTER   , RONLY , var_dot3PauseTable, 3, { 10,1,3 } },
//#define   DOT3OUTPAUSEFRAMES    32
//  { DOT3OUTPAUSEFRAMES  , ASN_COUNTER   , RONLY , var_dot3PauseTable, 3, { 10,1,4 } },

};
/*    (L = length of the oidsuffix) */


/*
 * init_dot3():
 *   Initialization routine.  This is called when the agent starts up.
 *   At a minimum, registration of your variables should take place here.
 */
void init_dot3(void)
{
    /* register ourselves with the agent to handle our mib tree */
    REGISTER_MIB("dot3", dot3_variables, variable4,
                 dot3_variables_oid);

    /* place any other initialization junk you need here */
}


/*
 * var_dot3StatsTable():
 *   Handle this table separately from the scalar value case.
 *   The workings of this are basically the same as for var_dot3StatsTable above.
 */
unsigned char *
var_dot3StatsTable(struct variable *vp,
            oid     *name,
            size_t  *length,
            int     exact,
            size_t  *var_len,
            WriteMethod **write_method)
{
    static long long_ret;
    static oid nullobjid[] = { 0,0 };
    static oid etherobjid[ SNMP_CHIPSET_LEN ];
 
    register struct ifnet *ifp;
    int interface_count = 0;
    int supports_dot3 = 0, i;
    struct ether_drv_stats x;
    
    interface_count = cyg_snmp_num_interfaces();

    if ( header_simple_table( vp,name,length,exact,var_len,write_method,
                              interface_count)
         == MATCH_FAILED )
        return NULL;

    ifp = cyg_snmp_get_if(name[ (*length)-1 ]);

    if ( ! ifp )
        return NULL;

    /* If its an ethernet call the IOCTL and see if the device supports
       dot3. If it does it will return all the values needed */
    if ( IFT_ETHER == ifp->if_type ) {
        bzero( &x, sizeof( x ) );      
        if ( NULL != ifp->if_ioctl ) {
            if (!(*ifp->if_ioctl)(ifp, SIOCGIFSTATSUD, (caddr_t)&x)) {
                supports_dot3 = x.supports_dot3;
            }
        }
    }
    
    if ( (IFT_LOOP == ifp->if_type ) ||
         (IFT_PROPVIRTUAL == ifp->if_type ) ||
         (!supports_dot3)) {
        switch(vp->magic) {
        case DOT3STATSINDEX:
            long_ret = name[(*length)-1];
            return (unsigned char *) &long_ret;
            
        case DOT3STATSETHERCHIPSET:
            *var_len = sizeof(nullobjid);
            return (unsigned char *) nullobjid;
            
        case DOT3STATSDUPLEXSTATUS:
            long_ret = 1;
            return (unsigned char *) &long_ret;
            
        default:
            long_ret = 0; // a dummy value for most of them
            return (unsigned char *) &long_ret;
        }
    }
    
    /* If we got here, the device does support dot3 */
    switch(vp->magic) {
    case DOT3STATSINDEX:
        long_ret = name[(*length)-1];
        return (unsigned char *) &long_ret;
      
    case DOT3STATSALIGNMENTERRORS:
        long_ret = x.rx_align_errors;
        return (unsigned char *) &long_ret;
        
    case DOT3STATSFCSERRORS:
        long_ret = x.rx_crc_errors;
        return (unsigned char *) &long_ret;
        
    case DOT3STATSSINGLECOLLISIONFRAMES:
        long_ret = x.tx_single_collisions;
        return (unsigned char *) &long_ret;
        
    case DOT3STATSMULTIPLECOLLISIONFRAMES:
        long_ret = x.tx_mult_collisions;
        return (unsigned char *) &long_ret;
        
    case DOT3STATSSQETESTERRORS:
        long_ret = x.tx_sqetesterrors;
        return (unsigned char *) &long_ret;
        
    case DOT3STATSDEFERREDTRANSMISSIONS:
        long_ret = x.tx_deferred;
        return (unsigned char *) &long_ret;
        
    case DOT3STATSLATECOLLISIONS:
        long_ret = x.tx_late_collisions;
        return (unsigned char *) &long_ret;
        
    case DOT3STATSEXCESSIVECOLLISIONS:
        long_ret = x.tx_max_collisions;
        return (unsigned char *) &long_ret;
        
    case DOT3STATSINTERNALMACTRANSMITERRORS:
        long_ret = x.tx_underrun;
        return (unsigned char *) &long_ret;
        
    case DOT3STATSCARRIERSENSEERRORS:
        long_ret = x.tx_carrier_loss;
        return (unsigned char *) &long_ret;
      
    case DOT3STATSFRAMETOOLONGS:
        long_ret = x.rx_too_long_frames;
        return (unsigned char *) &long_ret;
      
    case DOT3STATSINTERNALMACRECEIVEERRORS:
        long_ret = x.rx_overrun_errors +
            x.rx_resource_errors;
        return (unsigned char *) &long_ret;
      
    case DOT3STATSSYMBOLERRORS:
        long_ret = x.rx_symbol_errors;
        return (unsigned char *) &long_ret;
        
    case DOT3STATSETHERCHIPSET:
        i = 0;
        while ( i < sizeof(etherobjid) ) {
            if ( 0 == (etherobjid[i] = x.snmp_chipset[i]) )
                break;
            i++;
        }
        if (i)
        {
            *var_len = i;
            return (unsigned char *) etherobjid;
        }
        else
        {
            *var_len = sizeof(nullobjid);
            return (unsigned char *) nullobjid;
        }
        
    case DOT3STATSDUPLEXSTATUS:
        long_ret = x.duplex;
        if ( 1 > long_ret || 3 < long_ret )
            long_ret = 1;
        return (unsigned char *) &long_ret;
      
    default:
        ERROR_MSG("");
        return NULL;
    }
}


#if 0 // UNSUPPORTED: we do not have the information for these tables.
/*
 * var_dot3ControlTable():
 *   Handle this table separately from the scalar value case.
 *   The workings of this are basically the same as for var_dot3ControlTable above.
 */
unsigned char *
var_dot3ControlTable(struct variable *vp,
            oid     *name,
            size_t  *length,
            int     exact,
            size_t  *var_len,
            WriteMethod **write_method)
{
    /* variables we may use later */
    static long long_ret;
    static unsigned char string[SPRINT_MAX_LEN];
    static oid objid[MAX_OID_LEN];
    static struct counter64 c64;
    if (header_simple_table(vp,name,length,exact,var_len,write_method, TABLE_SIZE)
        == MATCH_FAILED )
        return NULL;

    switch(vp->magic) {
    case DOT3CONTROLFUNCTIONSSUPPORTED:
        long_ret = 1;
        return &long_ret;

    case DOT3CONTROLINUNKNOWNOPCODES:
        long_ret = 0;
        return (unsigned char *) &long_ret;

    default:
      ERROR_MSG("");
    }
    return NULL;
}

/*
 * var_dot3PauseTable():
 *   Handle this table separately from the scalar value case.
 *   The workings of this are basically the same as for var_dot3PauseTable above.
 */
unsigned char *
var_dot3PauseTable(struct variable *vp,
            oid     *name,
            size_t  *length,
            int     exact,
            size_t  *var_len,
            WriteMethod **write_method)
{
    /* variables we may use later */
    static long long_ret;
    static unsigned char string[SPRINT_MAX_LEN];
    static oid objid[MAX_OID_LEN];
    static struct counter64 c64;

    if (header_simple_table(vp,name,length,exact,var_len,write_method, TABLE_SIZE)
        == MATCH_FAILED )
        return NULL;
    
    switch(vp->magic) {
    case DOT3PAUSEADMINMODE:
        *write_method = write_dot3PauseAdminMode;
        long_ret = 0;
        return (unsigned char *) &long_ret;

    case DOT3PAUSEOPERMODE:
        long_ret = 0;
        return (unsigned char *) &long_ret;

    case DOT3INPAUSEFRAMES:
        long_ret = 0;
        return (unsigned char *) &long_ret;

    case DOT3OUTPAUSEFRAMES:
        long_ret = 0;
        return (unsigned char *) &long_ret;

    default:
        ERROR_MSG("");
    }
    return NULL;
}


int
write_dot3PauseAdminMode(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    static long *long_ret;
    int size;

    switch ( action ) {
    case RESERVE1:
        if (var_val_type != ASN_INTEGER){
            fprintf(stderr, "write to dot3PauseAdminMode not ASN_INTEGER\n");
            return SNMP_ERR_WRONGTYPE;
        }
        if (var_val_len > sizeof(long_ret)){
            fprintf(stderr,"write to dot3PauseAdminMode: bad length\n");
            return SNMP_ERR_WRONGLENGTH;
        }
        break;
        
    case RESERVE2:
        size = var_val_len;
        long_ret = (long *) var_val;
        break;
        
    case FREE:
        /* Release any resources that have been allocated */
        break;
        
    case ACTION:
        /* The variable has been stored in long_ret for
           you to use, and you have just been asked to do something with
           it.  Note that anything done here must be reversable in the UNDO case */
        break;
        
    case UNDO:
        /* Back out any changes made in the ACTION case */
        break;
        
    case COMMIT:
        /* Things are working well, so it's now safe to make the change
           permanently.  Make sure that anything done here can't fail! */
        break;
    }
    return SNMP_ERR_NOERROR;
}
#endif // UNSUPPORTED

// EOF dot3.c
