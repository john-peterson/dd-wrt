#include <errno.h>
#include <sys/syscall.h>
#include <sys/sysmips.h>

_syscall4(int, sysmips, const int, cmd, const long, arg1, const int, arg2, const int, arg3);
