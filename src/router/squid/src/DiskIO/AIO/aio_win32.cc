
/*
 * $Id: aio_win32.cc,v 1.3 2007/04/28 22:26:40 hno Exp $
 *
 * DEBUG: section 81    aio_xxx() POSIX emulation on Windows
 * AUTHOR: Guido Serassio <serassio@squid-cache.org>
 *
 * SQUID Web Proxy Cache          http://www.squid-cache.org/
 * ----------------------------------------------------------
 *
 *  Squid is the result of efforts by numerous individuals from
 *  the Internet community; see the CONTRIBUTORS file for full
 *  details.   Many organizations have provided support for Squid's
 *  development; see the SPONSORS file for full details.  Squid is
 *  Copyrighted (C) 2001 by the Regents of the University of
 *  California; see the COPYRIGHT file for full details.  Squid
 *  incorporates software developed and/or copyrighted by other
 *  sources; see the CREDITS file for full details.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
 *
 */

#include "squid.h"
#include "comm.h"
#include "aio_win32.h"

#ifdef _SQUID_WIN32_
VOID CALLBACK IoCompletionRoutine(DWORD dwErrorCode,
                                  DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{

    struct aiocb *aiocbp = (struct aiocb *) lpOverlapped->hEvent;

    aiocbp->aio_sigevent.sigev_notify = dwErrorCode;
    aiocbp->aio_sigevent.sigev_signo = dwNumberOfBytesTransfered;
    debugs(81, 7, "AIO operation complete: errorcode=" << dwErrorCode << " nbytes=" << dwNumberOfBytesTransfered);
    xfree(lpOverlapped);
}


int aio_read(struct aiocb *aiocbp)
{
    LPOVERLAPPED Overlapped;
    BOOL IoOperationStatus;

    /* Allocate an overlapped structure. */
    Overlapped = (LPOVERLAPPED) xcalloc(1, sizeof(OVERLAPPED));

    if (!Overlapped)
    {
        errno = ENOMEM;
        return -1;
    }

#if _FILE_OFFSET_BITS==64
#ifdef __GNUC__
    Overlapped->Offset = (DWORD) (aiocbp->aio_offset % 0x100000000LL);

    Overlapped->OffsetHigh = (DWORD) (aiocbp->aio_offset / 0x100000000LL);

#else

    Overlapped->Offset = (DWORD) (aiocbp->aio_offset % 0x100000000);

    Overlapped->OffsetHigh = (DWORD) (aiocbp->aio_offset / 0x100000000);

#endif
#else

    Overlapped->Offset = aiocbp->aio_offset;

    Overlapped->OffsetHigh = 0;

#endif

    Overlapped->hEvent = aiocbp;

    aiocbp->aio_sigevent.sigev_notify = EINPROGRESS;

    aiocbp->aio_sigevent.sigev_signo = -1;

    IoOperationStatus = ReadFileEx((HANDLE)_get_osfhandle(aiocbp->aio_fildes),
                                   aiocbp->aio_buf,
                                   aiocbp->aio_nbytes,
                                   Overlapped,
                                   IoCompletionRoutine);

    /* Test to see if the I/O was queued successfully. */
    if (!IoOperationStatus)
    {
        errno = GetLastError();
        debugs(81,1, "aio_read: GetLastError=" << errno  );
        return -1;
    }

    /* The I/O queued successfully. Go back into the
       alertable wait for I/O completion or for 
       more I/O requests. */
    return 0;
}


int aio_read64(struct aiocb64 *aiocbp)
{
    LPOVERLAPPED Overlapped;
    BOOL IoOperationStatus;

    /* Allocate an overlapped structure. */
    Overlapped = (LPOVERLAPPED) xcalloc(1, sizeof(OVERLAPPED));

    if (!Overlapped)
    {
        errno = ENOMEM;
        return -1;
    }

#ifdef __GNUC__
    Overlapped->Offset = (DWORD) (aiocbp->aio_offset % 0x100000000LL);

    Overlapped->OffsetHigh = (DWORD) (aiocbp->aio_offset / 0x100000000LL);

#else

    Overlapped->Offset = (DWORD) (aiocbp->aio_offset % 0x100000000);

    Overlapped->OffsetHigh = (DWORD) (aiocbp->aio_offset / 0x100000000);

#endif

    Overlapped->hEvent = aiocbp;

    aiocbp->aio_sigevent.sigev_notify = EINPROGRESS;

    aiocbp->aio_sigevent.sigev_signo = -1;

    IoOperationStatus = ReadFileEx((HANDLE)_get_osfhandle(aiocbp->aio_fildes),
                                   aiocbp->aio_buf,
                                   aiocbp->aio_nbytes,
                                   Overlapped,
                                   IoCompletionRoutine);

    /* Test to see if the I/O was queued successfully. */
    if (!IoOperationStatus)
    {
        errno = GetLastError();
        debugs(81, 1, "aio_read: GetLastError=" << errno  );
        return -1;
    }

    /* The I/O queued successfully. Go back into the
       alertable wait for I/O completion or for 
       more I/O requests. */
    return 0;
}


int aio_write(struct aiocb *aiocbp)
{
    LPOVERLAPPED Overlapped;
    BOOL IoOperationStatus;

    /* Allocate an overlapped structure. */
    Overlapped = (LPOVERLAPPED) xcalloc(1, sizeof(OVERLAPPED));

    if (!Overlapped)
    {
        errno = ENOMEM;
        return -1;
    }

#if _FILE_OFFSET_BITS==64
#ifdef __GNUC__
    Overlapped->Offset = (DWORD) (aiocbp->aio_offset % 0x100000000LL);

    Overlapped->OffsetHigh = (DWORD) (aiocbp->aio_offset / 0x100000000LL);

#else

    Overlapped->Offset = (DWORD) (aiocbp->aio_offset % 0x100000000);

    Overlapped->OffsetHigh = (DWORD) (aiocbp->aio_offset / 0x100000000);

#endif
#else

    Overlapped->Offset = aiocbp->aio_offset;

    Overlapped->OffsetHigh = 0;

#endif

    Overlapped->hEvent = aiocbp;

    aiocbp->aio_sigevent.sigev_notify = EINPROGRESS;

    aiocbp->aio_sigevent.sigev_signo = -1;

    IoOperationStatus = WriteFileEx((HANDLE)_get_osfhandle(aiocbp->aio_fildes),
                                    aiocbp->aio_buf,
                                    aiocbp->aio_nbytes,
                                    Overlapped,
                                    IoCompletionRoutine);

    /* Test to see if the I/O was queued successfully. */
    if (!IoOperationStatus)
    {
        errno = GetLastError();
        debugs(81, 1, "aio_write: GetLastError=" << errno  );
        return -1;
    }

    /* The I/O queued successfully. Go back into the
       alertable wait for I/O completion or for 
       more I/O requests. */
    return 0;
}


int aio_write64(struct aiocb64 *aiocbp)
{
    LPOVERLAPPED Overlapped;
    BOOL IoOperationStatus;

    /* Allocate an overlapped structure. */
    Overlapped = (LPOVERLAPPED) xcalloc(1, sizeof(OVERLAPPED));

    if (!Overlapped)
    {
        errno = ENOMEM;
        return -1;
    }

#ifdef __GNUC__
    Overlapped->Offset = (DWORD) (aiocbp->aio_offset % 0x100000000LL);

    Overlapped->OffsetHigh = (DWORD) (aiocbp->aio_offset / 0x100000000LL);

#else

    Overlapped->Offset = (DWORD) (aiocbp->aio_offset % 0x100000000);

    Overlapped->OffsetHigh = (DWORD) (aiocbp->aio_offset / 0x100000000);

#endif

    Overlapped->hEvent = aiocbp;

    aiocbp->aio_sigevent.sigev_notify = EINPROGRESS;

    aiocbp->aio_sigevent.sigev_signo = -1;

    IoOperationStatus = WriteFileEx((HANDLE)_get_osfhandle(aiocbp->aio_fildes),
                                    aiocbp->aio_buf,
                                    aiocbp->aio_nbytes,
                                    Overlapped,
                                    IoCompletionRoutine);

    /* Test to see if the I/O was queued successfully. */
    if (!IoOperationStatus)
    {
        errno = GetLastError();
        debugs(81, 1, "aio_write: GetLastError=" << errno  );
        return -1;
    }

    /* The I/O queued successfully. Go back into the
       alertable wait for I/O completion or for 
       more I/O requests. */
    return 0;
}


int aio_error(const struct aiocb * aiocbp)
{
    return aiocbp->aio_sigevent.sigev_notify;
}


int aio_error64(const struct aiocb64 * aiocbp)
{
    return aiocbp->aio_sigevent.sigev_notify;
}


int aio_open(const char *path, int mode)
{
    HANDLE hndl;
    DWORD dwCreationDisposition;
    DWORD dwDesiredAccess;
    int fd;

    if (mode & O_WRONLY)
        mode |= O_APPEND;

    mode |= O_BINARY;

    errno = 0;

    if (mode & O_WRONLY)
        dwDesiredAccess = GENERIC_WRITE;
    else
        dwDesiredAccess = (mode & O_RDONLY) ? GENERIC_READ : GENERIC_READ | GENERIC_WRITE;

    if (mode & O_TRUNC)
        dwCreationDisposition = CREATE_ALWAYS;
    else
        dwCreationDisposition = (mode & O_CREAT) ? OPEN_ALWAYS : OPEN_EXISTING;

    if ((hndl = CreateFile(path,	                /* file name               */
                           dwDesiredAccess,	        /* access mode             */
                           0,			            /* share mode              */
                           NULL,			        /* SD                      */
                           dwCreationDisposition,	/* how to create           */
                           FILE_FLAG_OVERLAPPED,	/* file attributes         */
                           NULL			            /* handle to template file */
                          )) != INVALID_HANDLE_VALUE) {
        statCounter.syscalls.disk.opens++;
        fd = _open_osfhandle((long) hndl, 0);
        commSetCloseOnExec(fd);
        fd_open(fd, FD_FILE, path);
    } else {
        errno = GetLastError();
        fd = DISK_ERROR;
    }

    return fd;
}


void aio_close(int fd)
{
    CloseHandle((HANDLE)_get_osfhandle(fd));
    fd_close(fd);
    statCounter.syscalls.disk.closes++;
}


ssize_t aio_return(struct aiocb * aiocbp)
{
    return aiocbp->aio_sigevent.sigev_signo;
}


ssize_t aio_return64(struct aiocb64 * aiocbp)

{
    return aiocbp->aio_sigevent.sigev_signo;
}
#endif /* _SQUID_WIN32_ */
