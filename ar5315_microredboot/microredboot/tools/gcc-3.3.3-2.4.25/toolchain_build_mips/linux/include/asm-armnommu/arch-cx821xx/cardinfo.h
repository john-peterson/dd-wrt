/****************************************************************************
 *
 *  Name:			CARDINFO.H
 *
 *  Description:	Card status informations header file
 *
 *  Copyright (c) 1999 Conexant
 *
 ****************************************************************************
 *
 *  $Author: gerg $
 *  $Revision: 1.1 $
 *  $Modtime: 4/10/02 2:16p $
 *
 ***************************************************************************/
#ifndef CARDINFO_H
#define CARDINFO_H

#define FIRMWARE_VERSION_STRING "$_$"		// TBD

/*
** ADSL Line State and ADSL Requested Line State uses the following enum.
** Note that the cardinfo structure does not include this info now, it
** may in the future.
*/
typedef enum
{
   ADSL_DISABLE,
   ADSL_HW_INIT,
   ADSL_SHOWTIME_UP,
   ADSL_SHOWTIME_DOWN,
   ADSL_REQUEST_SHOWTIME_UP,
   ADSL_REQUEST_SHOWTIME_DOWN
} eADLSState;

typedef struct cardParamStruct
{
	DWORD dwIndex;
	DWORD dwValue;
} CARDPARAM, *PCARDPARAM;

#define CARD_PARAM_MAX					(7)

typedef struct cardDataStruct
{
	DWORD     dwCount;
	CARDPARAM Param[CARD_PARAM_MAX];
} CARDDATA, *PCARDDATA;

/****************************************************************************
*  Type definitions
****************************************************************************/

/* This declaration moved here from DevIoCardAL.h, */ 
/* because it is shared with host driver.          */ 
//typedef enum BD_ADSL_MODEM_STATUS_E
//{
//	BD_MODEM_START					= 0,
//	BD_MODEM_DOWN					= 0,
//	BD_MODEM_ACTIVATION,			/* 1 */
//	BD_MODEM_TRANSCEIVER_TRAINING,
//	BD_MODEM_CHANNEL_ANALYSIS,
//	BD_MODEM_EXCHANGE,
//	BD_MODEM_ACTIVATED,				/* 5 */
//	BD_MODEM_WAITING_INIT,
//	BD_MODEM_INITIALIZING,
//	BD_MODEM_UNKNOWN,
//	BD_MODEM_END					/* 9 */
//} BD_ADSL_MODEM_STATUS_T;

/* This declaration moved here from DevIo.h, */ 
/* because it is shared with host driver.    */ 
#define UNKNOWN_4BIT_VALUE		0x0000000F
#define UNKNOWN_8BIT_VALUE		0x000000FF
#define UNKNOWN_16BIT_VALUE		0x0000FFFF
#define UNKNOWN_32BIT_VALUE		0xFFFFFFFF

#if 0
//#ifndef _HWIO_H_
//
//	HW_IO_GET_PERFORMANCE		Extracted from Tigris  Hwio.h  8/11/00
//
typedef enum HW_IO_MODEM_STATUS_CODES_E
{
	HW_IO_MODEM_STATUS_START					= 0,

	HW_IO_MODEM_STATUS_DOWN						= 0,
	HW_IO_MODEM_STATUS_ACTIVATION,					// 1
	HW_IO_MODEM_STATUS_TRANSCEIVER_TRAINING,
	HW_IO_MODEM_STATUS_CHANNEL_ANALYSIS,
	HW_IO_MODEM_STATUS_EXCHANGE,
	HW_IO_MODEM_STATUS_ACTIVATED,					// 5
	HW_IO_MODEM_STATUS_WAITING_INIT,
	HW_IO_MODEM_STATUS_INITIALIZING,
	HW_IO_MODEM_STATUS_UNKNOWN,

	HW_IO_MODEM_STATUS_END							// 9

} HW_IO_MODEM_STATUS_CODES_T;
#endif

//* Persistent Data Elements
//  From PC to P52 
typedef enum LC_CFG_PARM_NAMES_E 
{
 	LC_CFG_START = 0,
 	LC_CFG_DEBUG_FLAG = LC_CFG_START,
	LC_ADSLDATAPATHSELECT,
	LC_ADSLPSDTESTMODE,
	LC_LINEAUTOACTIVATION,
	LC_LINEPERSISTENTACT, 
	LC_CONTROLLERLOGENABLE,
	LC_CAPABILITIES_G922,
	LC_CAPABILITIES_G922ANNEX,
	LC_VENDOR_NEAR_ID,
	LC_MAC_ADDRESS,
	LC_AUTO_SENSE_HANDSHAKE,
	LC_AUTO_SENSE_LOOP_MIN,
	LC_AUTO_SENSE_LOOP_MAX,
	LC_AUTO_SENSE_INITIAL_TIMEOUT,
	LC_AUTO_SENSE_LOOP_TIMEOUTS,
	LC_AUTO_SENSE_LOCK_TIMEOUT,
	LC_AUTO_SENSE_RESTART_PAUSE,
	LC_AUTO_SENSE_SEARCH_PAUSE,	
	LC_AUTO_SENSE_WIRING_SELECTION,
	LC_AUTO_SENSE_WIRING_NUM_COMBOS,
	LC_AUTO_SENSE_WIRING_COMBO_GPIO,
	LC_AUTO_SENSE_WIRING_RELAY_DELAY,
	LC_AUTO_SENSE_WIRING_RELAY_ENRG,
	LC_AUTO_SENSE_WIRING_ORDER,
	LC_ADSL_HEAD_END,
	LC_ADSL_HEAD_END_ENVIRONMENT,
	LC_PHYSICAL_DRIVER_TYPE,
	LC_SOFTWARE_SWITCH_HOOK_STATE,
	LC_USE_SOFTWARE_SWITCH_HOOK_STATE,
    LC_ADSL_CRC_PER_SES,    
    LC_ADSL_SES_TO_RETRAIN,
	LC_ADSL_TELCO_ID,
    LC_CFG_MAX
}LC_CFG_PARM_NAMES_T;

#define EXE_HWIO_COMMAND	0xFFFFFFFF;

//  From P52 to PC 
typedef enum CC_CFG_STATUS_E 
{
 	CC_CFG_START = 0,
 	CC_CFG_DOWNRATE = CC_CFG_START,
	CC_CFG_UPRATE,
	CC_CFG_LINKSTATUS,
	CC_CFG_LINESTATUS,
	CC_CFG_MACADDRHIGH, 
	CC_CFG_MACADDRLOW,

	// New enum for layered CP
	CC_CFG_UP_SNR_MARGIN_CUR,
	CC_CFG_DOWN_SNR_MARGIN_CUR,
	CC_CFG_UP_ATTN,
	CC_CFG_DOWN_ATTN,
	CC_CFG_TX_POWER,
	CC_CFG_UP_BITS_PER_FRAME,
	CC_CFG_DOWN_BITS_PER_FRAME,
	CC_CFG_STARTUP_ATTEMPTS,
	CC_CFG_UP_CRC_ERRORS,
	CC_CFG_DOWN_CRC_ERRORS,
	CC_CFG_UP_FEC_ERRORS,
	CC_CFG_DOWN_FEC_ERRORS,
	CC_CFG_UP_HEC_ERRORS,
	CC_CFG_DOWN_HEC_ERRORS,

	CC_CFG_HWIO_LINE_STARTABLE, 
	CC_CFG_HWIO_MODULATION,
	CC_CFG_ADSL_HEAD_END,
	CC_CFG_ADSL_HEAD_END_ENVIRONMENT,
	
/*	Old enum for non-layered CP
	CC_CFG_CAPOCCUPATIONDNSTR,
	CC_CFG_NOISEMARGINDNSTR,
	CC_CFG_OUTPUTPOWERDNSTR,
	CC_CFG_ATTENDNSTR,
	CC_CFG_CAPOCCUPATIONUPSTR,
	CC_CFG_NOISEMARGINUPSTR,
	CC_CFG_OUTPUTPOWERUPSTR,
	CC_CFG_ATTENUPSTR,
*/
	CC_CFG_MAX
} CC_CFG_STATUS_T;

#endif
