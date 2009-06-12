/* This is an automatically generated file. Do not edit.               */
/* Repository name: PACKAGES/kernel/VERSION/include/instrument_desc.h  */
/* Install tree   : INSTALL/include/cyg/kernel/instrument_desc.h       */

struct instrument_desc_s {                                
    char *   msg;                                         
    CYG_WORD num;                                         
};                                                        

#ifdef CYGDBG_KERNEL_INSTRUMENT_MSGS_DEFINE_TABLE         
struct instrument_desc_s instrument_desc[] = {
{"ALARM",   0x0900},
{"ALARM_ADD",   1},
{"ALARM_CALL",   3},
{"ALARM_INIT",   4},
{"ALARM_INTERVAL",   6},
{"ALARM_REM",   2},
{"ALARM_TRIGGER",   5},
{"BINSEM",   0x0600},
{"BINSEM_CLAIM",   1},
{"BINSEM_POST",   5},
{"BINSEM_TRY",   4},
{"BINSEM_WAIT",   2},
{"BINSEM_WAKE",   6},
{"BINSEM_WOKE",   3},
{"CLOCK",   0x0800},
{"CLOCK_ISR",   3},
{"CLOCK_TICK_END",   2},
{"CLOCK_TICK_START",   1},
{"CNTSEM",   0x0700},
{"CNTSEM_CLAIM",   1},
{"CNTSEM_POST",   5},
{"CNTSEM_TIMEOUT",   7},
{"CNTSEM_TRY",   4},
{"CNTSEM_WAIT",   2},
{"CNTSEM_WAKE",   6},
{"CNTSEM_WOKE",   3},
{"CONDVAR",   0x0500},
{"CONDVAR_BROADCAST",   5},
{"CONDVAR_SIGNAL",   3},
{"CONDVAR_TIMED_WAIT",   6},
{"CONDVAR_WAIT",   1},
{"CONDVAR_WAKE",   4},
{"CONDVAR_WOKE",   2},
{"INTR",   0x0300},
{"INTR_ACK",   14},
{"INTR_ATTACH",   6},
{"INTR_CALL_DSR",   5},
{"INTR_CHAIN_ISR",   15},
{"INTR_CONFIGURE",   13},
{"INTR_DETACH",   7},
{"INTR_DISABLE",   9},
{"INTR_ENABLE",   10},
{"INTR_END",   2},
{"INTR_GET_CPU",   17},
{"INTR_MASK",   11},
{"INTR_POST_DSR",   4},
{"INTR_RAISE",   1},
{"INTR_RESTORE",   3},
{"INTR_SET_CPU",   16},
{"INTR_SET_VSR",   8},
{"INTR_UNMASK",   12},
{"MBOXT",   0x0a00},
{"MBOXT_GET",   2},
{"MBOXT_GOT",   3},
{"MBOXT_PUT",   7},
{"MBOXT_TIMEOUT",   4},
{"MBOXT_TRY",   6},
{"MBOXT_WAIT",   1},
{"MBOXT_WAKE",   5},
{"MLQ",   0x0c00},
{"MLQ_ADD",   3},
{"MLQ_DEQUEUE",   8},
{"MLQ_ENQUEUE",   7},
{"MLQ_REM",   4},
{"MLQ_REMOVE",   9},
{"MLQ_RESCHEDULE",   2},
{"MLQ_SCHEDULE",   1},
{"MLQ_TIMESLICE",   5},
{"MLQ_YIELD",   6},
{"MUTEX",   0x0400},
{"MUTEX_LOCK",   1},
{"MUTEX_LOCKED",   3},
{"MUTEX_RELEASE",   7},
{"MUTEX_RELEASED",   8},
{"MUTEX_TRY",   4},
{"MUTEX_UNLOCK",   5},
{"MUTEX_WAIT",   2},
{"MUTEX_WAKE",   6},
{"SCHED",   0x0100},
{"SCHED_LOCK",   1},
{"SCHED_RESCHEDULE",   3},
{"SCHED_TIMESLICE",   4},
{"SCHED_UNLOCK",   2},
{"SMP",   0x0b00},
{"SMP_CPU_START",   4},
{"SMP_LOCK_GOT",   6},
{"SMP_LOCK_INC",   1},
{"SMP_LOCK_SET",   3},
{"SMP_LOCK_WAIT",   5},
{"SMP_LOCK_ZERO",   2},
{"SMP_RESCHED_RECV",   9},
{"SMP_RESCHED_SEND",   8},
{"THREAD",   0x0200},
{"THREAD_ALARM",   8},
{"THREAD_ATTACH_STACK",   11},
{"THREAD_CHECK_STACK",   10},
{"THREAD_CREATE",   12},
{"THREAD_DELAY",   7},
{"THREAD_ENTER",   9},
{"THREAD_PRIORITY",   6},
{"THREAD_RESUME",   5},
{"THREAD_SLEEP",   2},
{"THREAD_SUSPEND",   4},
{"THREAD_SWITCH",   1},
{"THREAD_WAKE",   3},
{"USER",   0x0f00},
{"USER_10",   10},
{"USER_1",   1},
{"USER_11",   11},
{"USER_12",   12},
{"USER_13",   13},
{"USER_14",   14},
{"USER_15",   15},
{"USER_16",   16},
{"USER_17",   17},
{"USER_18",   18},
{"USER_19",   19},
{"USER_2",   2},
{"USER_3",   3},
{"USER_4",   4},
{"USER_5",   5},
{"USER_6",   6},
{"USER_7",   7},
{"USER_8",   8},
{"USER_9",   9},
{ 0, 0 }
};
#else /* CYGDBG_KERNEL_INSTRUMENT_MSGS_DEFINE_TABLE */    
extern struct instrument_desc_s instrument_desc[];        
#endif /* CYGDBG_KERNEL_INSTRUMENT_MSGS_DEFINE_TABLE */   

/* EOF instrument_desc.h */
