#ifndef __ARCH_FRIO_SOCKIOS__
#define __ARCH_FRIO_SOCKIOS__

/* Socket-level I/O control calls. */
#define FIOSETOWN 	0x8901
#define SIOCSPGRP	0x8902
#define FIOGETOWN	0x8903
#define SIOCGPGRP	0x8904
#define SIOCATMARK	0x8905
#define SIOCGSTAMP	0x8906		/* Get stamp */

#endif /* __ARCH_FRIO_SOCKIOS__ */
