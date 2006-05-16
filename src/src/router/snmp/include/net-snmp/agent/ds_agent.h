#ifndef NETSNMP_DS_AGENT_H
#define NETSNMP_DS_AGENT_H
/*
 * defines agent's default store registrations 
 */

/*
 * booleans 
 */
#define NETSNMP_DS_AGENT_VERBOSE        0       /* 1 if verbose output desired */
#define NETSNMP_DS_AGENT_ROLE           1       /* 0 if master, 1 if client */
#define NETSNMP_DS_AGENT_NO_ROOT_ACCESS 2       /* 1 if we can't get root access */
#define NETSNMP_DS_AGENT_AGENTX_MASTER  3       /* 1 if AgentX desired */
#define NETSNMP_DS_AGENT_QUIT_IMMEDIATELY 4     /* 1 to never start the agent */
#define NETSNMP_DS_AGENT_DISABLE_PERL   5       /* 1 to never start the agent */
#define NETSNMP_DS_AGENT_NO_CONNECTION_WARNINGS 6 /* 1 = !see !connect msgs */
#define NETSNMP_DS_AGENT_LEAVE_PIDFILE  7       /* 1 = leave PID file on exit */
/*
 * strings 
 */
#define NETSNMP_DS_AGENT_PROGNAME 0     /* argv[0] */
#define NETSNMP_DS_AGENT_X_SOCKET 1     /* AF_UNIX or ip:port socket addr */
#define NETSNMP_DS_AGENT_PORTS    2     /* localhost:9161,tcp:localhost:9161... */
#define NETSNMP_DS_AGENT_INTERNAL_SECNAME  3    /* used by disman/mteTriggerTable. */
#define NETSNMP_DS_AGENT_PERL_INIT_FILE    4    /* used by embedded perl */

/*
 * integers 
 */
#define NETSNMP_DS_AGENT_FLAGS    0     /* session.flags */
#define NETSNMP_DS_AGENT_USERID   1
#define NETSNMP_DS_AGENT_GROUPID  2
#define NETSNMP_DS_AGENT_AGENTX_PING_INTERVAL 3 /* ping master every SECONDS */
#define NETSNMP_DS_AGENT_AGENTX_TIMEOUT  4
#define NETSNMP_DS_AGENT_AGENTX_RETRIES  5

#endif
