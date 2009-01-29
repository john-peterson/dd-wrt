
/*
 * $Id: comm_kqueue.cc,v 1.17.2.1 2008/02/25 03:45:24 amosjeffries Exp $
 *
 * DEBUG: section 5     Socket Functions
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

/*
 * This code was originally written by Benno Rice and hacked on quite
 * a bit by Adrian. Adrian then took it to the hybrid-ircd project to use
 * in their new IO subsystem. After a year of modifications and some
 * rather interesting changes (event aggregation) its back in squid.
 * Thanks to the ircd-hybrid guys.
 */

/*
 * XXX Currently not implemented / supported by this module XXX
 *
 * - delay pools
 * - deferred reads
 * - flags.read_pending
 *
 * So, its not entirely useful in a production setup since if a read
 * is meant to be deferred it isn't (we're not even throwing the event
 * away here). Eventually the rest of the code will be rewritten
 * so deferred reads aren't required.
 *  -- adrian
 */

#include "squid.h"
#include "comm_kqueue.h"
#include "CacheManager.h"
#include "Store.h"
#include "fde.h"
#include "SquidTime.h"

#ifdef USE_KQUEUE

#include <sys/event.h>


#define KE_LENGTH        128

/* jlemon goofed up and didn't add EV_SET until fbsd 4.3 */

#ifndef EV_SET
#define EV_SET(kevp, a, b, c, d, e, f) do {     \
        (kevp)->ident = (a);                    \
        (kevp)->filter = (b);                   \
        (kevp)->flags = (c);                    \
        (kevp)->fflags = (d);                   \
        (kevp)->data = (e);                     \
        (kevp)->udata = (f);                    \
} while(0)
#endif

static void kq_update_events(int, short, PF *);
static int kq;

static struct timespec zero_timespec;

static struct kevent *kqlst;        /* kevent buffer */
static int kqmax;                /* max structs to buffer */
static int kqoff;                /* offset into the buffer */
static int max_poll_time = 1000;


/* XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX */
/* Private functions */

void
kq_update_events(int fd, short filter, PF * handler)
{
    PF *cur_handler;
    int kep_flags;

    switch (filter) {

    case EVFILT_READ:
        cur_handler = fd_table[fd].read_handler;
        break;

    case EVFILT_WRITE:
        cur_handler = fd_table[fd].write_handler;
        break;

    default:
        /* XXX bad! -- adrian */
        return;
        break;
    }

    if ((cur_handler == NULL && handler != NULL)
            || (cur_handler != NULL && handler == NULL)) {

        struct kevent *kep;

        kep = kqlst + kqoff;

        if (handler != NULL) {
            kep_flags = (EV_ADD | EV_ONESHOT);
        } else {
            kep_flags = EV_DELETE;
        }

        EV_SET(kep, (uintptr_t) fd, filter, kep_flags, 0, 0, 0);

	/* Check if we've used the last one. If we have then submit them all */
        if (kqoff == kqmax - 1) {
            int ret;

            ret = kevent(kq, kqlst, kqmax, NULL, 0, &zero_timespec);
            /* jdc -- someone needs to do error checking... */

            if (ret == -1) {
                perror("kq_update_events(): kevent()");
                return;
            }

            kqoff = 0;
        } else {
            kqoff++;
        }
    }
}



/* XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX */
/* Public functions */


/*
 * comm_select_init
 *
 * This is a needed exported function which will be called to initialise
 * the network loop code.
 */
void
comm_select_init(void)
{
    kq = kqueue();

    if (kq < 0) {
        fatal("comm_select_init: Couldn't open kqueue fd!\n");
    }

    kqmax = getdtablesize();

    kqlst = (struct kevent *)xmalloc(sizeof(*kqlst) * kqmax);
    zero_timespec.tv_sec = 0;
    zero_timespec.tv_nsec = 0;
}

/*
 * comm_setselect
 *
 * This is a needed exported function which will be called to register
 * and deregister interest in a pending IO state for a given FD.
 */
void
commSetSelect(int fd, unsigned int type, PF * handler,
              void *client_data, time_t timeout)
{
    fde *F = &fd_table[fd];
    assert(fd >= 0);
    assert(F->flags.open);

    if (type & COMM_SELECT_READ) {
        kq_update_events(fd, EVFILT_READ, handler);
        F->read_handler = handler;
        F->read_data = client_data;
    }

    if (type & COMM_SELECT_WRITE) {
        kq_update_events(fd, EVFILT_WRITE, handler);
        F->write_handler = handler;
        F->write_data = client_data;
    }

    if (timeout)
        F->timeout = squid_curtime + timeout;

}

void
commResetSelect(int fd)
{
    fde *F = &fd_table[fd];
    if (F->read_handler) {
	kq_update_events(fd, EVFILT_READ, (PF *)1);
    }
    if (F->write_handler) {
	kq_update_events(fd, EVFILT_WRITE, (PF *)1);
    }
}

/*
 * Check all connections for new connections and input data that is to be
 * processed. Also check for connections with data queued and whether we can
 * write it out.
 */

/*
 * comm_select
 *
 * Called to do the new-style IO, courtesy of of squid (like most of this
 * new IO code). This routine handles the stuff we've hidden in
 * comm_setselect and fd_table[] and calls callbacks for IO ready
 * events.
 */

comm_err_t
comm_select(int msec)
{
    int num, i;

    static struct kevent ke[KE_LENGTH];

    struct timespec poll_time;

    if (msec > max_poll_time)
        msec = max_poll_time;

    poll_time.tv_sec = msec / 1000;

    poll_time.tv_nsec = (msec % 1000) * 1000000;

    for (;;) {
        num = kevent(kq, kqlst, kqoff, ke, KE_LENGTH, &poll_time);
        statCounter.select_loops++;
        kqoff = 0;

        if (num >= 0)
            break;

        if (ignoreErrno(errno))
            break;

        getCurrentTime();

        return COMM_ERROR;

        /* NOTREACHED */
    }

    getCurrentTime();

    if (num == 0)
        return COMM_OK;		/* No error.. */

    for (i = 0; i < num; i++) {
        int fd = (int) ke[i].ident;
        PF *hdl = NULL;
        fde *F = &fd_table[fd];

        if (ke[i].flags & EV_ERROR) {
            errno = ke[i].data;
            /* XXX error == bad! -- adrian */
            continue;        /* XXX! */
        }

        switch (ke[i].filter) {

        case EVFILT_READ:

            if ((hdl = F->read_handler) != NULL) {
                F->read_handler = NULL;
		F->flags.read_pending = 0;
                hdl(fd, F->read_data);
            }

            break;

        case EVFILT_WRITE:

            if ((hdl = F->write_handler) != NULL) {
                F->write_handler = NULL;
                hdl(fd, F->write_data);
            }

            break;

        default:
            /* Bad! -- adrian */
            debugs(5, 1, "comm_select: kevent returned " << ke[i].filter << "!");
            break;
        }
    }

    return COMM_OK;
}

void
comm_quick_poll_required(void)
{
    max_poll_time = 10;
}

void
commKQueueRegisterWithCacheManager(CacheManager & manager)
{
}

#endif /* USE_KQUEUE */
