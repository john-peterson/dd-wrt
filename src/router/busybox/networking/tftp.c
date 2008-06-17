/* vi: set sw=4 ts=4: */
/* -------------------------------------------------------------------------
 * tftp.c
 *
 * A simple tftp client/server for busybox.
 * Tries to follow RFC1350.
 * Only "octet" mode supported.
 * Optional blocksize negotiation (RFC2347 + RFC2348)
 *
 * Copyright (C) 2001 Magnus Damm <damm@opensource.se>
 *
 * Parts of the code based on:
 *
 * atftp:  Copyright (C) 2000 Jean-Pierre Lefebvre <helix@step.polymtl.ca>
 *                        and Remi Lefebvre <remi@debian.org>
 *
 * utftp:  Copyright (C) 1999 Uwe Ohse <uwe@ohse.de>
 *
 * tftpd added by Denys Vlasenko & Vladimir Dronnikov
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 * ------------------------------------------------------------------------- */

#include "libbb.h"

#if ENABLE_FEATURE_TFTP_GET || ENABLE_FEATURE_TFTP_PUT

#define TFTP_BLKSIZE_DEFAULT       512  /* according to RFC 1350, don't change */
#define TFTP_BLKSIZE_DEFAULT_STR "512"
#define TFTP_TIMEOUT_MS             50
#define TFTP_MAXTIMEOUT_MS        2000
#define TFTP_NUM_RETRIES            12  /* number of backed-off retries */

/* opcodes we support */
#define TFTP_RRQ   1
#define TFTP_WRQ   2
#define TFTP_DATA  3
#define TFTP_ACK   4
#define TFTP_ERROR 5
#define TFTP_OACK  6

/* error codes sent over network (we use only 0, 3 and 8) */
/* generic (error message is included in the packet) */
#define ERR_UNSPEC   0
#define ERR_NOFILE   1
#define ERR_ACCESS   2
/* disk full or allocation exceeded */
#define ERR_WRITE    3
#define ERR_OP       4
#define ERR_BAD_ID   5
#define ERR_EXIST    6
#define ERR_BAD_USER 7
#define ERR_BAD_OPT  8

/* masks coming from getopt32 */
enum {
	TFTP_OPT_GET = (1 << 0),
	TFTP_OPT_PUT = (1 << 1),
	/* pseudo option: if set, it's tftpd */
	TFTPD_OPT = (1 << 7) * ENABLE_TFTPD,
	TFTPD_OPT_r = (1 << 8) * ENABLE_TFTPD,
	TFTPD_OPT_c = (1 << 9) * ENABLE_TFTPD,
	TFTPD_OPT_u = (1 << 10) * ENABLE_TFTPD,
};

#if ENABLE_FEATURE_TFTP_GET && !ENABLE_FEATURE_TFTP_PUT
#define USE_GETPUT(...)
#define CMD_GET(cmd) 1
#define CMD_PUT(cmd) 0
#elif !ENABLE_FEATURE_TFTP_GET && ENABLE_FEATURE_TFTP_PUT
#define USE_GETPUT(...)
#define CMD_GET(cmd) 0
#define CMD_PUT(cmd) 1
#else
#define USE_GETPUT(...) __VA_ARGS__
#define CMD_GET(cmd) ((cmd) & TFTP_OPT_GET)
#define CMD_PUT(cmd) ((cmd) & TFTP_OPT_PUT)
#endif
/* NB: in the code below
 * CMD_GET(cmd) and CMD_PUT(cmd) are mutually exclusive
 */


struct globals {
	/* u16 TFTP_ERROR; u16 reason; both network-endian, then error text: */
	uint8_t error_pkt[4 + 32];
	char *user_opt;
	/* used in tftpd_main(), a bit big for stack: */
	char block_buf[TFTP_BLKSIZE_DEFAULT];
};
#define G (*(struct globals*)&bb_common_bufsiz1)
#define block_buf        (G.block_buf   )
#define user_opt         (G.user_opt    )
#define error_pkt        (G.error_pkt   )
#define INIT_G() \
	do { \
	} while (0)

#define error_pkt_reason (error_pkt[3])
#define error_pkt_str    (error_pkt + 4)


#if ENABLE_FEATURE_TFTP_BLOCKSIZE

static int tftp_blksize_check(const char *blksize_str, int maxsize)
{
	/* Check if the blksize is valid:
	 * RFC2348 says between 8 and 65464,
	 * but our implementation makes it impossible
	 * to use blksizes smaller than 22 octets. */
	unsigned blksize = bb_strtou(blksize_str, NULL, 10);
	if (errno
	 || (blksize < 24) || (blksize > maxsize)
	) {
		bb_error_msg("bad blocksize '%s'", blksize_str);
		return -1;
	}
#if ENABLE_DEBUG_TFTP
	bb_error_msg("using blksize %u", blksize);
#endif
	return blksize;
}

static char *tftp_get_blksize(char *buf, int len)
{
#define option "blksize"
	int opt_val = 0;
	int opt_found = 0;
	int k;

	/* buf points to:
	 * "opt_name<NUL>opt_val<NUL>opt_name2<NUL>opt_val2<NUL>..." */

	while (len > 0) {
		/* Make sure options are terminated correctly */
		for (k = 0; k < len; k++) {
			if (buf[k] == '\0') {
				goto nul_found;
			}
		}
		return NULL;
 nul_found:
		if (opt_val == 0) { /* it's "name" part */
			if (strcasecmp(buf, option) == 0) {
				opt_found = 1;
			}
		} else if (opt_found) {
			return buf;
		}

		k++;
		buf += k;
		len -= k;
		opt_val ^= 1;
	}

	return NULL;
#undef option
}

#endif

static int tftp_protocol(
		len_and_sockaddr *our_lsa,
		len_and_sockaddr *peer_lsa,
		const char *local_file,
		USE_TFTP(const char *remote_file,)
		int blksize)
{
#if !ENABLE_TFTP
#define remote_file NULL
#endif
	struct pollfd pfd[1];
#define socket_fd (pfd[0].fd)
	int len;
	int send_len;
	USE_FEATURE_TFTP_BLOCKSIZE(smallint want_option_ack = 0;)
	smallint finished = 0;
	uint16_t opcode;
	uint16_t block_nr;
	uint16_t recv_blk;
	int open_mode, local_fd;
	int retries, waittime_ms;
	int io_bufsize = blksize + 4;
	char *cp;
	/* Can't use RESERVE_CONFIG_BUFFER here since the allocation
	 * size varies meaning BUFFERS_GO_ON_STACK would fail */
	/* We must keep the transmit and receive buffers seperate */
	/* In case we rcv a garbage pkt and we need to rexmit the last pkt */
	char *xbuf = xmalloc(io_bufsize);
	char *rbuf = xmalloc(io_bufsize);

	socket_fd = xsocket(peer_lsa->u.sa.sa_family, SOCK_DGRAM, 0);
	setsockopt_reuseaddr(socket_fd);

	block_nr = 1;
	cp = xbuf + 2;

	if (!ENABLE_TFTP || our_lsa) {
		/* tftpd */

		/* Create a socket which is:
		 * 1. bound to IP:port peer sent 1st datagram to,
		 * 2. connected to peer's IP:port
		 * This way we will answer from the IP:port peer
		 * expects, will not get any other packets on
		 * the socket, and also plain read/write will work. */
		xbind(socket_fd, &our_lsa->u.sa, our_lsa->len);
		xconnect(socket_fd, &peer_lsa->u.sa, peer_lsa->len);

		/* Is there an error already? Send pkt and bail out */
		if (error_pkt_reason || error_pkt_str[0])
			goto send_err_pkt;

		if (CMD_GET(option_mask32)) {
			/* it's upload - we must ACK 1st packet (with filename)
			 * as if it's "block 0" */
			block_nr = 0;
		}

		if (user_opt) {
			struct passwd *pw = getpwnam(user_opt);
			if (!pw)
				bb_error_msg_and_die("unknown user '%s'", user_opt);
			change_identity(pw); /* initgroups, setgid, setuid */
		}
	}

	/* Open local file (must be after changing user) */
	if (CMD_PUT(option_mask32)) {
		open_mode = O_RDONLY;
	} else {
		open_mode = O_WRONLY | O_TRUNC | O_CREAT;
#if ENABLE_TFTPD
		if ((option_mask32 & (TFTPD_OPT+TFTPD_OPT_c)) == TFTPD_OPT) {
			/* tftpd without -c */
			open_mode = O_WRONLY | O_TRUNC;
		}
#endif
	}
	if (!(option_mask32 & TFTPD_OPT)) {
		local_fd = CMD_GET(option_mask32) ? STDOUT_FILENO : STDIN_FILENO;
		if (NOT_LONE_DASH(local_file))
			local_fd = xopen(local_file, open_mode);
	} else {
		local_fd = open_or_warn(local_file, open_mode);
		if (local_fd < 0) {
			/*error_pkt_reason = ERR_NOFILE/ERR_ACCESS?*/
			strcpy(error_pkt_str, "can't open file");
			goto send_err_pkt;
		}
	}

	if (!ENABLE_TFTP || our_lsa) {
#if ENABLE_FEATURE_TFTP_BLOCKSIZE
		if (blksize != TFTP_BLKSIZE_DEFAULT) {
			/* Create and send OACK packet. */
			/* For the download case, block_nr is still 1 -
			 * we expect 1st ACK from peer to be for (block_nr-1),
			 * that is, for "block 0" which is our OACK pkt */
			opcode = TFTP_OACK;
			goto add_blksize_opt;
		}
#endif
	}
	else {
/* Removing it, or using if() statement instead may lead to
 * "warning: null argument where non-null required": */
#if ENABLE_TFTP
		/* tftp */

		/* We can't (and don't really need to) bind the socket:
		 * we don't know from which local IP datagrams will be sent,
		 * but kernel will pick the same IP every time (unless routing
		 * table is changed), thus peer will see dgrams consistently
		 * coming from the same IP.
		 * We would like to connect the socket, but since peer's
		 * UDP code can be less perfect than ours, _peer's_ IP:port
		 * in replies may differ from IP:port we used to send
		 * our first packet. We can connect() only when we get
		 * first reply. */

		/* build opcode */
		opcode = TFTP_WRQ;
		if (CMD_GET(option_mask32)) {
			opcode = TFTP_RRQ;
		}
		/* add filename and mode */
		/* fill in packet if the filename fits into xbuf */
		len = strlen(remote_file) + 1;
		if (2 + len + sizeof("octet") >= io_bufsize) {
			bb_error_msg("remote filename is too long");
			goto ret;
		}
		strcpy(cp, remote_file);
		cp += len;
		/* add "mode" part of the package */
		strcpy(cp, "octet");
		cp += sizeof("octet");

#if ENABLE_FEATURE_TFTP_BLOCKSIZE
		if (blksize == TFTP_BLKSIZE_DEFAULT)
			goto send_pkt;

		/* Non-standard blocksize: add option to pkt */
		if ((&xbuf[io_bufsize - 1] - cp) < sizeof("blksize NNNNN")) {
			bb_error_msg("remote filename is too long");
			goto ret;
		}
		want_option_ack = 1;
#endif
#endif /* ENABLE_TFTP */

#if ENABLE_FEATURE_TFTP_BLOCKSIZE
 add_blksize_opt:
		/* add "blksize", <nul>, blksize, <nul> */
		strcpy(cp, "blksize");
		cp += sizeof("blksize");
		cp += snprintf(cp, 6, "%d", blksize) + 1;
#endif
		/* First packet is built, so skip packet generation */
		goto send_pkt;
	}

	/* Using mostly goto's - continue/break will be less clear
	 * in where we actually jump to */
	while (1) {
		/* Build ACK or DATA */
		cp = xbuf + 2;
		*((uint16_t*)cp) = htons(block_nr);
		cp += 2;
		block_nr++;
		opcode = TFTP_ACK;
		if (CMD_PUT(option_mask32)) {
			opcode = TFTP_DATA;
			len = full_read(local_fd, cp, blksize);
			if (len < 0) {
				goto send_read_err_pkt;
			}
			if (len != blksize) {
				finished = 1;
			}
			cp += len;
		}
 send_pkt:
		/* Send packet */
		*((uint16_t*)xbuf) = htons(opcode); /* fill in opcode part */
		send_len = cp - xbuf;
		/* NB: send_len value is preserved in code below
		 * for potential resend */

		retries = TFTP_NUM_RETRIES;	/* re-initialize */
		waittime_ms = TFTP_TIMEOUT_MS;

 send_again:
#if ENABLE_DEBUG_TFTP
		fprintf(stderr, "sending %u bytes\n", send_len);
		for (cp = xbuf; cp < &xbuf[send_len]; cp++)
			fprintf(stderr, "%02x ", (unsigned char) *cp);
		fprintf(stderr, "\n");
#endif
		xsendto(socket_fd, xbuf, send_len, &peer_lsa->u.sa, peer_lsa->len);
		/* Was it final ACK? then exit */
		if (finished && (opcode == TFTP_ACK))
			goto ret;

 recv_again:
		/* Receive packet */
		/*pfd[0].fd = socket_fd;*/
		pfd[0].events = POLLIN;
		switch (safe_poll(pfd, 1, waittime_ms)) {
		default:
			/*bb_perror_msg("poll"); - done in safe_poll */
			goto ret;
		case 0:
			retries--;
			if (retries == 0) {
				bb_error_msg("timeout");
				goto ret; /* no err packet sent */
			}

			/* exponential backoff with limit */
			waittime_ms += waittime_ms/2;
			if (waittime_ms > TFTP_MAXTIMEOUT_MS) {
				waittime_ms = TFTP_MAXTIMEOUT_MS;
			}

			goto send_again; /* resend last sent pkt */
		case 1:
			if (!our_lsa) {
				/* tftp (not tftpd!) receiving 1st packet */
				our_lsa = ((void*)(ptrdiff_t)-1); /* not NULL */
				len = recvfrom(socket_fd, rbuf, io_bufsize, 0,
						&peer_lsa->u.sa, &peer_lsa->len);
				/* Our first dgram went to port 69
				 * but reply may come from different one.
				 * Remember and use this new port (and IP) */
				if (len >= 0)
					xconnect(socket_fd, &peer_lsa->u.sa, peer_lsa->len);
			} else {
				/* tftpd, or not the very first packet:
				 * socket is connect()ed, can just read from it. */
				/* Don't full_read()!
				 * This is not TCP, one read == one pkt! */
				len = safe_read(socket_fd, rbuf, io_bufsize);
			}
			if (len < 0) {
				goto send_read_err_pkt;
			}
			if (len < 4) { /* too small? */
				goto recv_again;
			}
		}

		/* Process recv'ed packet */
		opcode = ntohs( ((uint16_t*)rbuf)[0] );
		recv_blk = ntohs( ((uint16_t*)rbuf)[1] );
#if ENABLE_DEBUG_TFTP
		fprintf(stderr, "received %d bytes: %04x %04x\n", len, opcode, recv_blk);
#endif

		if (opcode == TFTP_ERROR) {
			static const char errcode_str[] =
				"\0"
				"file not found\0"
				"access violation\0"
				"disk full\0"
				"bad operation\0"
				"unknown transfer id\0"
				"file already exists\0"
				"no such user\0"
				"bad option";

			const char *msg = "";

			if (len > 4 && rbuf[4] != '\0') {
				msg = &rbuf[4];
				rbuf[io_bufsize - 1] = '\0'; /* paranoia */
			} else if (recv_blk <= 8) {
				msg = nth_string(errcode_str, recv_blk);
			}
			bb_error_msg("server error: (%u) %s", recv_blk, msg);
			goto ret;
		}

#if ENABLE_FEATURE_TFTP_BLOCKSIZE
		if (want_option_ack) {
			want_option_ack = 0;
			if (opcode == TFTP_OACK) {
				/* server seems to support options */
				char *res;

				res = tftp_get_blksize(&rbuf[2], len - 2);
				if (res) {
					blksize = tftp_blksize_check(res, blksize);
					if (blksize < 0) {
						error_pkt_reason = ERR_BAD_OPT;
						goto send_err_pkt;
					}
					io_bufsize = blksize + 4;
					/* Send ACK for OACK ("block" no: 0) */
					block_nr = 0;
					continue;
				}
				/* rfc2347:
				 * "An option not acknowledged by the server
				 *  must be ignored by the client and server
				 *  as if it were never requested." */
			}
			bb_error_msg("server only supports blocksize of 512");
			blksize = TFTP_BLKSIZE_DEFAULT;
			io_bufsize = TFTP_BLKSIZE_DEFAULT + 4;
		}
#endif
		/* block_nr is already advanced to next block# we expect
		 * to get / block# we are about to send next time */

		if (CMD_GET(option_mask32) && (opcode == TFTP_DATA)) {
			if (recv_blk == block_nr) {
				int sz = full_write(local_fd, &rbuf[4], len - 4);
				if (sz != len - 4) {
					strcpy(error_pkt_str, bb_msg_write_error);
					error_pkt_reason = ERR_WRITE;
					goto send_err_pkt;
				}
				if (sz != blksize) {
					finished = 1;
				}
				continue; /* send ACK */
			}
			if (recv_blk == (block_nr - 1)) {
				/* Server lost our TFTP_ACK.  Resend it */
				block_nr = recv_blk;
				continue;
			}
		}

		if (CMD_PUT(option_mask32) && (opcode == TFTP_ACK)) {
			/* did peer ACK our last DATA pkt? */
			if (recv_blk == (uint16_t) (block_nr - 1)) {
				if (finished)
					goto ret;
				continue; /* send next block */
			}
		}
		/* Awww... recv'd packet is not recognized! */
		goto recv_again;
		/* why recv_again? - rfc1123 says:
		 * "The sender (i.e., the side originating the DATA packets)
		 *  must never resend the current DATA packet on receipt
		 *  of a duplicate ACK".
		 * DATA pkts are resent ONLY on timeout.
		 * Thus "goto send_again" will ba a bad mistake above.
		 * See:
		 * http://en.wikipedia.org/wiki/Sorcerer's_Apprentice_Syndrome
		 */
	} /* end of "while (1)" */
 ret:
	if (ENABLE_FEATURE_CLEAN_UP) {
		close(local_fd);
		close(socket_fd);
		free(xbuf);
		free(rbuf);
	}
	return finished == 0; /* returns 1 on failure */

 send_read_err_pkt:
	strcpy(error_pkt_str, bb_msg_read_error);
 send_err_pkt:
	if (error_pkt_str[0])
		bb_error_msg(error_pkt_str);
	error_pkt[1] = TFTP_ERROR;
	xsendto(socket_fd, error_pkt, 4 + 1 + strlen(error_pkt_str),
			&peer_lsa->u.sa, peer_lsa->len);
	return EXIT_FAILURE;
}

#if ENABLE_TFTP

int tftp_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int tftp_main(int argc ATTRIBUTE_UNUSED, char **argv)
{
	len_and_sockaddr *peer_lsa;
	const char *local_file = NULL;
	const char *remote_file = NULL;
#if ENABLE_FEATURE_TFTP_BLOCKSIZE
	const char *blksize_str = TFTP_BLKSIZE_DEFAULT_STR;
#endif
	int blksize;
	int result;
	int port;
	USE_GETPUT(int opt;)

	INIT_G();

	/* -p or -g is mandatory, and they are mutually exclusive */
	opt_complementary = "" USE_FEATURE_TFTP_GET("g:") USE_FEATURE_TFTP_PUT("p:")
			USE_GETPUT("g--p:p--g:");

	USE_GETPUT(opt =) getopt32(argv,
			USE_FEATURE_TFTP_GET("g") USE_FEATURE_TFTP_PUT("p")
				"l:r:" USE_FEATURE_TFTP_BLOCKSIZE("b:"),
			&local_file, &remote_file
			USE_FEATURE_TFTP_BLOCKSIZE(, &blksize_str));
	argv += optind;

#if ENABLE_FEATURE_TFTP_BLOCKSIZE
	/* Check if the blksize is valid:
	 * RFC2348 says between 8 and 65464 */
	blksize = tftp_blksize_check(blksize_str, 65564);
	if (blksize < 0) {
		//bb_error_msg("bad block size");
		return EXIT_FAILURE;
	}
#else
	blksize = TFTP_BLKSIZE_DEFAULT;
#endif

	if (!local_file)
		local_file = remote_file;
	if (!remote_file)
		remote_file = local_file;
	/* Error if filename or host is not known */
	if (!remote_file || !argv[0])
		bb_show_usage();

	port = bb_lookup_port(argv[1], "udp", 69);
	peer_lsa = xhost2sockaddr(argv[0], port);

#if ENABLE_DEBUG_TFTP
	fprintf(stderr, "using server '%s', remote_file '%s', local_file '%s'\n",
			xmalloc_sockaddr2dotted(&peer_lsa->u.sa),
			remote_file, local_file);
#endif

	result = tftp_protocol(
			NULL /* our_lsa*/, peer_lsa,
			local_file, remote_file,
			blksize);

	if (result != EXIT_SUCCESS && NOT_LONE_DASH(local_file) && CMD_GET(opt)) {
		unlink(local_file);
	}
	return result;
}

#endif /* ENABLE_TFTP */

#if ENABLE_TFTPD

/* TODO: libbb candidate? */
static len_and_sockaddr *get_sock_lsa(int s)
{
	len_and_sockaddr *lsa;
	socklen_t len = 0;

	if (getsockname(s, NULL, &len) != 0)
		return NULL;
	lsa = xzalloc(LSA_LEN_SIZE + len);
	lsa->len = len;
	getsockname(s, &lsa->u.sa, &lsa->len);
	return lsa;
}

int tftpd_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int tftpd_main(int argc ATTRIBUTE_UNUSED, char **argv)
{
	len_and_sockaddr *our_lsa;
	len_and_sockaddr *peer_lsa;
	char *local_file, *mode;
	const char *error_msg;
	int opt, result, opcode;
	int blksize = TFTP_BLKSIZE_DEFAULT;

	INIT_G();

	our_lsa = get_sock_lsa(STDIN_FILENO);
	if (!our_lsa)
		bb_perror_msg_and_die("stdin is not a socket");
	peer_lsa = xzalloc(LSA_LEN_SIZE + our_lsa->len);
	peer_lsa->len = our_lsa->len;

	/* Shifting to not collide with TFTP_OPTs */
	opt = option_mask32 = TFTPD_OPT | (getopt32(argv, "rcu:", &user_opt) << 8);
	argv += optind;
	if (argv[0])
		xchdir(argv[0]);

	result = recv_from_to(STDIN_FILENO, block_buf, sizeof(block_buf),
			0 /* flags */,
			&peer_lsa->u.sa, &our_lsa->u.sa, our_lsa->len);

	error_msg = "malformed packet";
	opcode = ntohs(*(uint16_t*)block_buf);
	if (result < 4 || result >= sizeof(block_buf)
	 || block_buf[result-1] != '\0'
	 || (USE_FEATURE_TFTP_PUT(opcode != TFTP_RRQ) /* not download */
	     USE_GETPUT(&&)
	     USE_FEATURE_TFTP_GET(opcode != TFTP_WRQ) /* not upload */
	    )
	) {
		goto err;
	}
	local_file = block_buf + 2;
	if (local_file[0] == '.' || strstr(local_file, "/.")) {
		error_msg = "dot in file name";
		goto err;
	}
	mode = local_file + strlen(local_file) + 1;
	if (mode >= block_buf + result || strcmp(mode, "octet") != 0) {
		goto err;
	}
#if ENABLE_FEATURE_TFTP_BLOCKSIZE
	{
		char *res;
		char *opt_str = mode + sizeof("octet");
		int opt_len = block_buf + result - opt_str;
		if (opt_len > 0) {
			res = tftp_get_blksize(opt_str, opt_len);
			if (res) {
				blksize = tftp_blksize_check(res, 65564);
				if (blksize < 0) {
					error_pkt_reason = ERR_BAD_OPT;
					/* will just send error pkt */
					goto do_proto;
				}
			}
		}
	}
#endif

	if (!ENABLE_FEATURE_TFTP_PUT || opcode == TFTP_WRQ) {
		if (opt & TFTPD_OPT_r) {
			/* This would mean "disk full" - not true */
			/*error_pkt_reason = ERR_WRITE;*/
			error_msg = bb_msg_write_error;
			goto err;
		}
		USE_GETPUT(option_mask32 |= TFTP_OPT_GET;) /* will receive file's data */
	} else {
		USE_GETPUT(option_mask32 |= TFTP_OPT_PUT;) /* will send file's data */
	}

	close(STDIN_FILENO); /* close old, possibly wildcard socket */
	/* tftp_protocol() will create new one, bound to particular local IP */

	/* NB: if error_pkt_str or error_pkt_reason is set up,
	 * tftp_protocol() just sends one error pkt and returns */
 do_proto:
	result = tftp_protocol(
		our_lsa, peer_lsa,
		local_file, USE_TFTP(NULL /*remote_file*/,)
		blksize
	);

	return result;
 err:
	strcpy(error_pkt_str, error_msg);
	goto do_proto;
}

#endif /* ENABLE_TFTPD */

#endif /* ENABLE_FEATURE_TFTP_GET || ENABLE_FEATURE_TFTP_PUT */
