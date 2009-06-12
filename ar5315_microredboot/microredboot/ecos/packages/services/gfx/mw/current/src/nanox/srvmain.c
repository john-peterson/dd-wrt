/*
 * Copyright (c) 1999, 2000, 2001 Greg Haerr <greg@censoft.com>
 * Copyright (c) 1991 David I. Bell
 * Permission is granted to use, distribute, or modify this source,
 * provided that this copyright notice remains intact.
 *
 * Main module of graphics server.
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#ifdef __PACIFIC__
#include <unixio.h>
#else
#include <errno.h>
#include <sys/types.h>
#endif

#ifdef DOS_TURBOC
#include <io.h>
#endif

#define MWINCLUDECOLORS
#include "serv.h"
#if UNIX | DOS_DJGPP
#include <unistd.h>
#if _MINIX
#include <sys/times.h>
#else
#include <sys/time.h>
#endif
#endif

#if DOS_DJGPP
typedef unsigned long DWORD;	/* FIXME why is this here?*/
#endif

#if ELKS
#include <linuxmt/posix_types.h>
#include <linuxmt/time.h>
#endif

#ifdef __ECOS
# include <pkgconf/system.h>
# include <pkgconf/microwindows.h>
# ifdef CYGPKG_NET
#  define NONETWORK 0
# else
#  error Not yet supported.
#  define NONETWORK 1
# endif
#endif

/*
 * External definitions defined here.
 */
GR_WINDOW_ID	cachewindowid;		/* cached window id */
GR_WINDOW_ID    cachepixmapid;         /* cached pixmap id */
GR_GC_ID	cachegcid;		/* cached graphics context id */
GR_WINDOW	*cachewp;		/* cached window pointer */
GR_GC		*cachegcp;		/* cached graphics context */
GR_PIXMAP       *cachepp;               /* cached pixmap */
GR_PIXMAP       *listpp;                /* List of all pixmaps */
GR_WINDOW	*listwp;		/* list of all windows */
GR_WINDOW	*rootwp;		/* root window pointer */
GR_GC		*listgcp;		/* list of all gc */
GR_REGION	*listregionp;		/* list of all regions */
GR_FONT		*listfontp;		/* list of all fonts */
GR_IMAGE	*listimagep;		/* list of all images */
GR_CURSOR	*listcursorp;		/* list of all cursors */
GR_CURSOR	*stdcursor;		/* root window cursor */
GR_GC		*curgcp;		/* currently enabled gc */
GR_WINDOW	*clipwp;		/* window clipping is set for */
GR_WINDOW	*focuswp;		/* focus window for keyboard */
GR_WINDOW	*mousewp;		/* window mouse is currently in */
GR_WINDOW	*grabbuttonwp;		/* window grabbed by button */
GR_CURSOR	*curcursor;		/* currently enabled cursor */
GR_COORD	cursorx;		/* current x position of cursor */
GR_COORD	cursory;		/* current y position of cursor */
int		curbuttons;		/* current state of buttons */
GR_CLIENT	*curclient;		/* client currently executing for */
GR_EVENT_LIST	*eventfree;		/* list of free events */
GR_BOOL		focusfixed;		/* TRUE if focus is fixed on a window */
PMWFONT		stdfont;		/* default font*/
int		escape_quits = 1;	/* terminate when pressing ESC */
char		*progname;		/* Name of this program.. */

int		current_fd;		/* the fd of the client talking to */
int		connectcount = 0;	/* number of connections to server */
GR_CLIENT	*root_client;		/* root entry of the client table */
GR_CLIENT	*current_client;	/* the client we are currently talking*/
char		*current_shm_cmds;
int		current_shm_cmds_size;
int		keyb_fd;		/* the keyboard file descriptor */
int		mouse_fd;		/* the mouse file descriptor */
char		*curfunc;		/* the name of the current server func*/
GR_TIMEOUT	screensaver_delay;	/* time before screensaver activates */
GR_BOOL		screensaver_active;	/* time before screensaver activates */
GR_SELECTIONOWNER selection_owner;	/* the selection owner and typelist */
GR_TIMEOUT	startTicks;		/* ms time server started*/
int		autoportrait = FALSE;	/* auto portrait mode switching*/

GR_TIMER_ID     cache_timer_id;         /* cached timer ID */
GR_TIMER        *cache_timer;           /* cached timer */
GR_TIMER        *list_timer;            /* list of all timers */

static int	persistent_mode = 0;
static int	portraitmode = MWPORTRAIT_NONE;

#if !NONETWORK
int		un_sock;		/* the server socket descriptor */

static void
usage(void)
{
	fprintf(stderr,"Usage: %s [-e] [-p] [-N] [-c <fontconfig-file> ...]\n",
		progname);
	exit(1);
}

/*
 * This is the main server loop which initialises the server, services
 * the clients, and shuts the server down when there are no more clients.
 */
#ifdef __ECOS
int
nanox_main(int argc, char *argv[])
#else
int
main(int argc, char *argv[])
#endif
{
	int t;
	int read_configfile(char *file);

	progname = argv[0];

	t = 1;
	while ( t < argc ) {
		if ( !strcmp("-e",argv[t])) {
			escape_quits = 0;
			t++;
			continue;
		}
		if ( !strcmp("-p",argv[t]) ) {
			persistent_mode = 1;
			t++;
			continue;
		}
		if ( !strcmp("-A",argv[t]) ) {
			autoportrait = TRUE;
			t++;
			continue;
		}
		if ( !strcmp("-N",argv[t]) ) {
			portraitmode = MWPORTRAIT_NONE;
			t++;
			continue;
		}
		if ( !strcmp("-L",argv[t]) ) {
			portraitmode = MWPORTRAIT_LEFT;
			t++;
			continue;
		}
		if ( !strcmp("-R",argv[t]) ) {
			portraitmode = MWPORTRAIT_RIGHT;
			t++;
			continue;
		}
		if ( !strcmp("-D",argv[t]) ) {
			portraitmode = MWPORTRAIT_DOWN;
			t++;
			continue;
		}
#if FONTMAPPER
		if ( !strcmp("-c",argv[t]) ) {
			if ( t+1 >= argc )
				usage();
			read_configfile(argv[t+1]);
			t += 2;
			continue;
		}
#endif
		usage();
	}

	/* Attempt to initialise the server*/
	if(GsInitialize() < 0)
		exit(1);

	while(1)
		GsSelect(0L);
	return 0;
}
#endif

void
GsAcceptClientFd(int i)
{
	GR_CLIENT *client, *cl;

	if(!(client = malloc(sizeof(GR_CLIENT)))) {
		close(i);
		return;
	}

	client->id = i;
	client->eventhead = NULL;
	client->eventtail = NULL;
	/*client->errorevent.type = GR_EVENT_TYPE_NONE;*/
	client->next = NULL;
	client->prev = NULL;
	client->waiting_for_event = FALSE;
	client->shm_cmds = 0;

	if(connectcount++ == 0)
		root_client = client;
	else {
		cl = root_client;
			while(cl->next)
				cl = cl->next;
		client->prev = cl;
		cl->next = client;
	}
}

/*
 * Open a connection from a new client to the server.
 * Returns -1 on failure.
 */
int
GrOpen(void)
{
#if NONETWORK
	escape_quits = 1;

	/* Client calls this routine once.  We 
	 * init everything here
	 */
	if (connectcount <= 0) {
		if(GsInitialize() < 0)
			return -1;
		GsAcceptClientFd(999);
		curclient = root_client;
	}
#endif
        return 1;
}

/*
 * Close the current connection to the server.
 */
void
GrClose(void)
{
	GsClose(current_fd);
}

/*
 * Drop a specific server connection.
 */
void
GsClose(int fd)
{
	GsDropClient(fd);
	if(!persistent_mode && connectcount == 0)
		GsTerminate();
}

#if NONETWORK
/* client/server GsDropClient is in srvnet.c*/
void
GsDropClient(int fd)
{
	--connectcount;
}
#endif

#if UNIX | DOS_DJGPP
#if NONETWORK && defined(HAVESELECT)
/*
 * Register the specified file descriptor to return an event
 * when input is ready.
 */

static int regfdmax = -1;
static fd_set regfdset;

void
GrRegisterInput(int fd)
{
	FD_SET(fd, &regfdset);
	if (fd > regfdmax) regfdmax = fd + 1;
}

void
GrUnregisterInput(int fd)
{
	int i, max;

	/* unregister all inputs if the FD is -1 */
	if (fd == -1) {
		FD_ZERO(&regfdset);
		regfdmax = -1;
		return;
	}

	FD_CLR(fd, &regfdset);
	/* recalculate the max file descriptor */
	for (i = 0, max = regfdmax, regfdmax = -1; i < max; i++)
		if (FD_ISSET(i, &regfdset))
			regfdmax = i + 1;
}

#endif /* NONETWORK && HAVESELECT */
#endif /* UNIX | DOS_DJGPP*/

/*
 * This function suspends execution of the program for the specified
 * number of milliseconds.
 */
void
GrDelay(GR_TIMEOUT msecs)
{
#if UNIX
	struct timeval timeval;

	timeval.tv_sec = msecs / 1000;
	timeval.tv_usec = msecs % 1000;
	select(0, NULL, NULL, NULL, &timeval);
#endif
}

#if NONETWORK
void
GrFlush(void)
{
}

void
GrMainLoop(GR_FNCALLBACKEVENT fncb)
{
	GR_EVENT event;

	for(;;) {
		GrGetNextEvent(&event);
		fncb(&event);
	}
}

void
GrReqShmCmds(long shmsize)
{
	/* no action required, no client/server*/
}
#endif


#if VXWORKS

#define POLLTIME	100   /* polling sleep interval (in msec) */
#define MAX_MOUSEEVENTS	10    /* max number of mouse event to get in 1 select */
#define MAX_KEYBDEVENTS	10    /* max number of mouse event to get in 1 select */

extern void GdSleep(int dwMilliseconds);

void 
GsSelect(GR_TIMEOUT timeout)
{
	int mouseevents = 0;
	int keybdevents = 0;
	GR_TIMEOUT waittime = 0;
	GR_EVENT_GENERAL *gp;


	/* input gathering loop */
	while (1)
	{

		/* perform pre-select duties, if any */
		if(scrdev.PreSelect)
		{
			scrdev.PreSelect(&scrdev);
		}


		/* If mouse data present, service it */
		while (mousedev.Poll() > 0)
		{
			GsCheckMouseEvent();
			if (mouseevents++ > MAX_MOUSEEVENTS)
			{
				/* don't handle too many events at one shot */
				break;
			}
		}


		/* If keyboard data present, service it */
		while (kbddev.Poll() > 0)
		{
			GsCheckKeyboardEvent();
			if (keybdevents++ > MAX_KEYBDEVENTS)
			{
				/* don't handle too many events at one shot */
				break;
			}
		}

		
		/* did we process any input yet? */
		if ((mouseevents > 0) || (keybdevents > 0))
		{
			/* yep -- return without sleeping */
			return;
		}


		/* give up time-slice & sleep for a bit */
		GdSleep(POLLTIME);
		waittime += POLLTIME; 


		/* have we timed out? */
		if (waittime >= timeout)
		{
			/* special case: polling when timeout == 0 -- don't send timeout event */
			if (timeout != 0)
			{
				/* Timeout has occured.  
				** Currently return a timeout event regardless of whether client 
				**   has selected for it.
				*/
				if ((gp = (GR_EVENT_GENERAL *)GsAllocEvent(curclient)) != NULL)
				{
					gp->type = GR_EVENT_TYPE_TIMEOUT;
				}
			}
			return;
		}
	}

}

#elif MSDOS | _MINIX

void
GsSelect(GR_TIMEOUT timeout)
{
	/* If mouse data present, service it*/
	if(mousedev.Poll())
		while(GsCheckMouseEvent())
			continue;

	/* If keyboard data present, service it*/
	if(kbddev.Poll())
		while(GsCheckKeyboardEvent())
			continue;

}

#elif defined(__ECOS) && defined(CYGPKG_HAL_I386_PCMB)

// For eCos on PC, use polling for the mouse and keyboard. Also
// use select() with a short timeout for communication.
//
// This needs fixing up later.
void
GsSelect(GR_TIMEOUT timeout)
{
	fd_set	rfds;
    int     setsize = 0;
	struct  timeval tout;
    int     real_timeout;
    int     e;

	/* If mouse data present, service it*/
	if(mousedev.Poll())
		while(GsCheckMouseEvent())
			continue;

	/* If keyboard data present, service it*/
	if(kbddev.Poll())
		while(GsCheckKeyboardEvent())
			continue;

    FD_ZERO(&rfds);
	/* perform pre-select duties, if any*/
	if(rootwp->psd->PreSelect)
		rootwp->psd->PreSelect(rootwp->psd);

	/* handle client socket connections*/
	FD_SET(un_sock, &rfds);
	if (un_sock > setsize) setsize = un_sock;
	curclient = root_client;
	while(curclient) {
		if(curclient->waiting_for_event && curclient->eventhead) {
			curclient->waiting_for_event = FALSE;
			GrGetNextEventWrapperFinish(curclient->id);
			return;
		}
		FD_SET(curclient->id, &rfds);
		if(curclient->id > setsize) setsize = curclient->id;
		curclient = curclient->next;
	}

	/* Set up the timeout for the main select(): */
	if(GdGetNextTimeout(&tout, timeout) == TRUE) {
        if ((tout.tv_sec > 0) || (tout.tv_usec > 20000)) {
            tout.tv_sec  = 0;
            tout.tv_usec = 20000;
            real_timeout = 0;
        } else {
            real_timeout = 1;
        }
    } else {
        tout.tv_sec  = 0;
        tout.tv_usec = 20000;
        real_timeout = 0;
    }

	/* Wait for some input on any of the fds in the set or a timeout: */
	if((e = select(setsize+1, &rfds, NULL, NULL, &tout)) > 0) {
		/* If a client is trying to connect, accept it: */
		if(FD_ISSET(un_sock, &rfds))
			GsAcceptClient();

		/* If a client is sending us a command, handle it: */
		curclient = root_client;
		while(curclient) {
			GR_CLIENT *curclient_next;

			/* curclient may be freed in GsDropClient*/
			curclient_next = curclient->next;
			if(FD_ISSET(curclient->id, &rfds))
				GsHandleClient(curclient->id);
			curclient = curclient_next;
		}
	} 
	else if (e == 0) {
        if (real_timeout) {
            GdTimeout();
        }
	} else
		if(errno != EINTR)
			EPRINTF("Select() call in main failed\n");
}

#elif UNIX && defined(HAVESELECT)

void
GsSelect(GR_TIMEOUT timeout)
{
	fd_set	rfds;
	int 	e;
	int	setsize = 0;
	struct timeval tout, *to;
#if NONETWORK
	int	fd;
#endif

	/* perform pre-select duties, if any*/
	if(rootwp->psd->PreSelect)
		rootwp->psd->PreSelect(rootwp->psd);

	/* Set up the FDs for use in the main select(): */
	FD_ZERO(&rfds);
	if(mouse_fd >= 0) {
		FD_SET(mouse_fd, &rfds);
		if (mouse_fd > setsize)
			setsize = mouse_fd;
	}
	if(keyb_fd >= 0) {
		FD_SET(keyb_fd, &rfds);
		if (keyb_fd > setsize)
			setsize = keyb_fd;
	}
#if NONETWORK
	/* handle registered input file descriptors*/
	for (fd = 0; fd < regfdmax; fd++) {
		if (!FD_ISSET(fd, &regfdset))
			continue;

		FD_SET(fd, &rfds);
		if (fd > setsize) setsize = fd;
	}
#else /* not NONETWORK */
	/* handle client socket connections*/
	FD_SET(un_sock, &rfds);
	if (un_sock > setsize) setsize = un_sock;
	curclient = root_client;
	while(curclient) {
		if(curclient->waiting_for_event && curclient->eventhead) {
			curclient->waiting_for_event = FALSE;
			GrGetNextEventWrapperFinish(curclient->id);
			return;
		}
		FD_SET(curclient->id, &rfds);
		if(curclient->id > setsize) setsize = curclient->id;
		curclient = curclient->next;
	}
#endif /* NONETWORK */
	/* Set up the timeout for the main select(): */
	if(GdGetNextTimeout(&tout, timeout) == TRUE)
		to = &tout;
	else to = NULL;

	/* Wait for some input on any of the fds in the set or a timeout: */
	if((e = select(setsize+1, &rfds, NULL, NULL, to)) > 0) {
		/* If data is present on the mouse fd, service it: */
		if(mouse_fd >= 0 && FD_ISSET(mouse_fd, &rfds))
			while(GsCheckMouseEvent())
				continue;

		/* If data is present on the keyboard fd, service it: */
		if(keyb_fd >= 0 && FD_ISSET(keyb_fd, &rfds))
			while(GsCheckKeyboardEvent())
				continue;

#if NONETWORK
		/* check for input on registered file descriptors */
		for (fd = 0; fd < regfdmax; fd++) {
			GR_EVENT_FDINPUT *	gp;

			if (!FD_ISSET(fd, &regfdset)  ||  !FD_ISSET(fd, &rfds))
				continue;

			gp =(GR_EVENT_FDINPUT *)GsAllocEvent(curclient);
			if(gp) {
				gp->type = GR_EVENT_TYPE_FDINPUT;
				gp->fd = fd;
			}
		}
#else /* not NONETWORK */

		/* If a client is trying to connect, accept it: */
		if(FD_ISSET(un_sock, &rfds))
			GsAcceptClient();

		/* If a client is sending us a command, handle it: */
		curclient = root_client;
		while(curclient) {
			GR_CLIENT *curclient_next;

			/* curclient may be freed in GsDropClient*/
			curclient_next = curclient->next;
			if(FD_ISSET(curclient->id, &rfds))
				GsHandleClient(curclient->id);
			curclient = curclient_next;
		}
#endif /* NONETWORK */
	} 
	else if (e == 0) {
#if NONETWORK
		/* 
		 * Timeout has occured.  Currently return
		 * a timeout event regardless of whether
		 * client has selected for it.
		 */
		if(GdTimeout() == TRUE) {
			GR_EVENT_GENERAL *	gp;
			gp = (GR_EVENT_GENERAL *)GsAllocEvent(curclient);
			if(gp)
				gp->type = GR_EVENT_TYPE_TIMEOUT;
		}
#else /* not NONETWORK */
		GdTimeout();
#endif /* NONETWORK */
	} else
		if(errno != EINTR)
			EPRINTF("Select() call in main failed\n");
}
#endif /* UNIX && defined(HAVESELECT)*/

#if VTSWITCH
static void
CheckVtChange(void *arg)
{
	if(MwCheckVtChange())
		GsRedrawScreen();
	GdAddTimer(50, CheckVtChange, NULL);
}
#endif
  
/*
 * Initialize the graphics and mouse devices at startup.
 * Returns nonzero with a message printed if the initialization failed.
 */
int
GsInitialize(void)
{
	GR_WINDOW	*wp;		/* root window */
	PSD		psd;
	GR_CURSOR_ID	cid;
	static MWIMAGEBITS cursorbits[16] = {
	      0xe000, 0x9800, 0x8600, 0x4180,
	      0x4060, 0x2018, 0x2004, 0x107c,
	      0x1020, 0x0910, 0x0988, 0x0544,
	      0x0522, 0x0211, 0x000a, 0x0004
	};
	static MWIMAGEBITS cursormask[16] = {
	      0xe000, 0xf800, 0xfe00, 0x7f80,
	      0x7fe0, 0x3ff8, 0x3ffc, 0x1ffc,
	      0x1fe0, 0x0ff0, 0x0ff8, 0x077c,
	      0x073e, 0x021f, 0x000e, 0x0004
	};

	wp = (GR_WINDOW *) malloc(sizeof(GR_WINDOW));
	if (wp == NULL) {
		EPRINTF("Cannot allocate root window\n");
		return -1;
	}

	startTicks = GsGetTickCount();

	/* catch terminate signal to restore tty state*/
	signal(SIGTERM, (void *)GsTerminate);

	screensaver_delay = 0;
	screensaver_active = GR_FALSE;

	selection_owner.wid = 0;
	selection_owner.typelist = NULL;

#if !NONETWORK
	/* ignore pipe signal, sent when clients exit*/
	signal(SIGPIPE, SIG_IGN);
	signal(SIGHUP, SIG_IGN);

	if (GsOpenSocket() < 0) {
		EPRINTF("Cannot bind to named socket\n");
		free(wp);
		return -1;
	}
#endif

	if ((keyb_fd = GdOpenKeyboard()) == -1) {
		EPRINTF("Cannot initialise keyboard\n");
		/*GsCloseSocket();*/
		free(wp);
		return -1;
	}

	if ((psd = GdOpenScreen()) == NULL) {
		EPRINTF("Cannot initialise screen\n");
		/*GsCloseSocket();*/
		GdCloseKeyboard();
		free(wp);
		return -1;
	}
	GdSetPortraitMode(psd, portraitmode);

	if ((mouse_fd = GdOpenMouse()) == -1) {
		EPRINTF("Cannot initialise mouse\n");
		/*GsCloseSocket();*/
		GdCloseScreen(psd);
		GdCloseKeyboard();
		free(wp);
		return -1;
	}

	/*
	 * Create std font.
	 */
#if (HAVE_BIG5_SUPPORT | HAVE_GB2312_SUPPORT)
	/* system fixed font looks better when mixed with chinese fonts*/
	stdfont = GdCreateFont(psd, MWFONT_SYSTEM_FIXED, 0, NULL);
#else
	stdfont = GdCreateFont(psd, MWFONT_SYSTEM_VAR, 0, NULL);
#endif

	/*
	 * Initialize the root window.
	 */
	wp->psd = psd;
	wp->id = GR_ROOT_WINDOW_ID;
	wp->parent = NULL;		/* changed: was = NULL*/
	wp->owner = NULL;
	wp->children = NULL;
	wp->siblings = NULL;
	wp->next = NULL;
	wp->x = 0;
	wp->y = 0;
	wp->width = psd->xvirtres;
	wp->height = psd->yvirtres;
	wp->bordersize = 0;
	wp->background = BLACK;
	wp->bordercolor = BLACK;
	wp->nopropmask = 0;
	wp->bgpixmap = NULL;
	wp->bgpixmapflags = GR_BACKGROUND_TILE;
	wp->eventclients = NULL;
	wp->cursorid = 0;
	wp->mapped = GR_TRUE;
	wp->unmapcount = 0;
	wp->output = GR_TRUE;
	wp->props = 0;
	wp->title = NULL;

        listpp = NULL;
	listwp = wp;
	rootwp = wp;
	focuswp = wp;
	mousewp = wp;
	focusfixed = GR_FALSE;

	/*
	 * Initialize and position the default cursor.
	 */
	curcursor = NULL;
	cursorx = -1;
	cursory = -1;
	GdShowCursor(psd);
	GrMoveCursor(psd->xvirtres / 2, psd->yvirtres / 2);
	cid = GrNewCursor(16, 16, 0, 0, WHITE, BLACK, cursorbits, cursormask);
	GrSetWindowCursor(GR_ROOT_WINDOW_ID, cid);
	stdcursor = GsFindCursor(cid);

#if VTSWITCH
	MwInitVt();
	/* Check for VT change every 50 ms: */
	GdAddTimer(50, CheckVtChange, NULL);
#endif
	psd->FillRect(psd, 0, 0, psd->xvirtres-1, psd->yvirtres-1,
		GdFindColor(BLACK));

	/*
	 * Tell the mouse driver some things.
	 */
	curbuttons = 0;
	GdRestrictMouse(0, 0, psd->xvirtres - 1, psd->yvirtres - 1);
	GdMoveMouse(psd->xvirtres / 2, psd->yvirtres / 2);

	/* Force root window screen paint*/
	GsRedrawScreen();

	/* 
	 * Force the cursor to appear on the screen at startup.
	 * (not required with above GsRedrawScreen)
	GdHideCursor(psd);
	GdShowCursor(psd);
	 */

	/*
	 * All done.
	 */
	connectcount = 0;
	return 0;
}

/*
 * Here to close down the server.
 */
void
GsTerminate(void)
{
#if !NONETWORK
	GsCloseSocket();
#endif
	GdCloseScreen(rootwp->psd);
	GdCloseMouse();
	GdCloseKeyboard();
#if VTSWITCH
	MwRedrawVt(mwvterm);
#endif
	exit(0);
}

/*
 * Return # milliseconds elapsed since start of Microwindows
 * Granularity is 25 msec
 */
GR_TIMEOUT
GsGetTickCount(void)
{
#if MSDOS
#include <time.h>
	return (DWORD)(clock() * 1000 / CLOCKS_PER_SEC);
#else
#if _MINIX
	struct tms	t;
	
	return (DWORD)times(&t) * 16;
#else
#if UNIX
	struct timeval t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 25000) * 25) - startTicks;
#else
	return 0L;
#endif
#endif
#endif
}

void
GrBell(void)
{
	write(2, "\7", 1);
}
