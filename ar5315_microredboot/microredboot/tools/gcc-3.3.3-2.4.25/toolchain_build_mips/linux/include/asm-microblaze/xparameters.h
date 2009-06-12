
/*******************************************************************
*
* CAUTION: This file is automatically generated by libgen.
* Version: Xilinx EDK 3.2.2 EDK_Cm.22
* DO NOT EDIT.
*
* Copyright (c) 2003 Xilinx, Inc.  All rights reserved. 
* 
* Description: Driver parameters
*
*******************************************************************/

#define XPAR_D_LMB_BRAM_IF_CNTLR_BASEADDR 0x00000000
#define XPAR_D_LMB_BRAM_IF_CNTLR_HIGHADDR 0x00003FFF
#define XPAR_I_LMB_BRAM_IF_CNTLR_BASEADDR 0x00000000
#define XPAR_I_LMB_BRAM_IF_CNTLR_HIGHADDR 0x00003FFF
#define XPAR_DDR_CONTROLLER_BASEADDR 0x80000000
#define XPAR_DDR_CONTROLLER_HIGHADDR 0x80FFFFFF
#define XPAR_SYSTEM_MEMCON_BASEADDR 0xFFFF0000
#define XPAR_SYSTEM_MEMCON_HIGHADDR 0xFFFF00FF

/******************************************************************/

#define XPAR_XUARTLITE_NUM_INSTANCES 2
#define XPAR_CONSOLE_UART_BASEADDR 0xFFFF2000
#define XPAR_CONSOLE_UART_HIGHADDR 0xFFFF20FF
#define XPAR_DEBUG_UART_BASEADDR 0xFFFF4000
#define XPAR_DEBUG_UART_HIGHADDR 0xFFFF40FF

/******************************************************************/

#define XPAR_INTC_MAX_NUM_INTR_INPUTS 3
#define XPAR_INTC_SINGLE_BASEADDR 0xFFFF3000
#define XPAR_INTC_SINGLE_HIGHADDR 0xFFFF30FF
#define XPAR_XINTC_NUM_INSTANCES 1
#define XPAR_XINTC_USE_DCR 0
#define XPAR_SYSTEM_INTC_BASEADDR 0xFFFF3000
#define XPAR_SYSTEM_INTC_HIGHADDR 0xFFFF30FF
#define XPAR_SYSTEM_INTC_KIND_OF_INTR 0x0000006E

/******************************************************************/

#define XPAR_SYSTEM_TIMER_INTERRUPT_MASK 0X000001
#define XPAR_CONSOLE_UART_INTERRUPT_MASK 0X000002
#define XPAR_DEBUG_UART_INTERRUPT_MASK 0X000004

/******************************************************************/

#define XPAR_XTMRCTR_NUM_INSTANCES 1
#define XPAR_SYSTEM_TIMER_BASEADDR 0xFFFF1000
#define XPAR_SYSTEM_TIMER_HIGHADDR 0xFFFF10FF

/******************************************************************/

#define XPAR_XGPIO_NUM_INSTANCES 1
#define XPAR_SYSTEM_GPIO_BASEADDR 0xFFFF5000
#define XPAR_SYSTEM_GPIO_HIGHADDR 0xFFFF50FF
#define XPAR_SYSTEM_GPIO_DEVICE_ID 0

#define XPAR_GPIO_0_BASEADDR XPAR_SYSTEM_GPIO_BASEADDR
#define XPAR_GPIO_0_HIGHADDR XPAR_SYSTEM_GPIO_HIGHADDR
#define XPAR_GPIO_0_DEVICE_ID XPAR_SYSTEM_GPIO_DEVICE_ID

/******************************************************************/

#define XPAR_XEMAC_NUM_INSTANCES 1
#define XPAR_EMAC_0_BASEADDR 0xC0000000
#define XPAR_EMAC_0_HIGHADDR 0xC0003FFF
#define XPAR_EMAC_0_DEVICE_ID 0

#define XPAR_OPB_ETHERNET_0_DEVICE_ID XPAR_EMAC_0_DEVICE_ID
#define XPAR_OPB_ETHERNET_0_BASEADDR XPAR_EMAC_0_BASEADDR

#define XPAR_OPB_ETHERNET_0_DMA_PRESENT 1
#define XPAR_OPB_ETHERNET_0_ERR_COUNT_EXIST 1
#define XPAR_OPB_ETHERNET_0_MII_EXIST 1

/******************************************************************/

#define XPAR_XSYSACE_NUM_INSTANCES 1
#define XPAR_SYSACE_0_HIGHADDR 0xCF0001FF
#define XPAR_SYSACE_0_BASEADDR 0xCF000000
#define XPAR_OPB_SYSACE_0_DEVICE_ID 0
#define XPAR_OPB_SYSACE_0_BASEADDR XPAR_SYSACE_0_BASEADDR

/******************************************************************/

#define STDIN_BASEADDRESS 0xFFFF2000
#define STDOUT_BASEADDRESS 0xFFFF2000

extern void XIntc_LowLevelInterruptHandler(void) __attribute__ ((interrupt_handler));
