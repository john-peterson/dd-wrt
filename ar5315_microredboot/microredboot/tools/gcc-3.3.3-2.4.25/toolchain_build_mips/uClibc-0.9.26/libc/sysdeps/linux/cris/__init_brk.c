/* From libc-5.3.12 */

#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include "sysdep.h"

void * ___brk_addr = 0;

int
__init_brk (void)
{
    if (___brk_addr == 0) {
	    /* 
		 * Notice that we don't need to save/restore the GOT
	     * register since that is not call clobbered by the syscall.
	     */
	    asm ("clear.d $r10\n\t"
		 "movu.w " STR(__NR_brk) ",$r9\n\t"
		 "break 13\n\t"
		 "move.d $r10, %0"
		 : "=r" (___brk_addr)
		 :
		 : "r9", "r10");
	    
	    if (___brk_addr == 0) {
		    __set_errno(ENOMEM);
		    return -1;
	    }
    }
    return 0;
}
