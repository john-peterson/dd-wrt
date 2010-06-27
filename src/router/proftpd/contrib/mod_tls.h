/*
 * ProFTPD - mod_tls API
 * Copyright (c) 2002-2009 TJ Saunders
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307, USA.
 *
 * As a special exemption, TJ Saunders and other respective copyright holders
 * give permission to link this program with OpenSSL, and distribute the
 * resulting executable, without including the source code for OpenSSL in the
 * source distribution.
 *
 * $Id: mod_tls.h,v 1.1 2009/03/10 05:10:29 castaglia Exp $
 */

#ifndef MOD_TLS_H
#define MOD_TLS_H

#include "conf.h"

#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>

/* For mod_tls-related modules wishing to log info to the TLSLog file. */
int tls_log(const char *, ...)
#ifdef __GNUC__
       __attribute__ ((format (printf, 1, 2)));
#else   
       ;
#endif

/* API for modules that which to register SSL session cache handlers. */

#if OPENSSL_VERSION_NUMBER >= 0x0090707f
# define TLS_D2I_SSL_SESSION_CONST	const
#else
# define TLS_D2I_SSL_SESSION_CONST
#endif

typedef struct sess_cache_st {
  const char *cache_name;

  /* Memory pool for this cache. */
  pool *cache_pool;

  /* Arbitrary cache-specific data */
  void *cache_data;

  /* Timeout (in secs) of entries in this cache. */
  long cache_timeout;

  /* Additional OpenSSL session caching flags desired by the session cache
   * handler.  These will be OR'd with SSL_SESS_CACHE_SERVER.  See
   * SSL_CTX_set_session_cache_mode(3) for details.
   */
  long cache_mode;

  /* Initialize the cache handler. Returns zero on success, -1 otherwise (with
   * errno set appropriately).
   */
  int (*open)(struct sess_cache_st *cache, char *info, long timeout);

  /* Destroy the cache handler, cleaning up any associated resources.  Returns
   * zero on success, -1 otherwise (with errno set appropriately).
   */
  int (*close)(struct sess_cache_st *cache);

  /* Add a new session entry to the cache.  The provided sess_id is effectively
   * the cache lookup key.
   */
  int (*add)(struct sess_cache_st *cache, unsigned char *sess_id,
    unsigned int sess_id_len, time_t expires, SSL_SESSION *sess);

  /* Retrieve a session from the cache, using the provided sess_id key. */
  SSL_SESSION *(*get)(struct sess_cache_st *cache, unsigned char *sess_id,
    unsigned int sess_id_len);

  /* Remove the specified session from the cache. */
  int (*delete)(struct sess_cache_st *cache, unsigned char *sess_id,
    unsigned int sess_id_len);

  /* Clear the cache of all sessions, regardless of their normal expiration
   * time.  Returns the number of cleared sessions on success, -1 otherwise
   * (with errno set appropriately).
   */
  int (*clear)(struct sess_cache_st *cache);

  /* Remove the entire cache.  Returns zero on success, -1 otherwise (with
   * errno set appropriately).
   */
  int (*remove)(struct sess_cache_st *cache);

  /* Query the cache for information: count of sessions currently cached,
   * hits/misses/expirations, etc.  Returns zero on success, -1 otherwise
   * (with errno set appropriately).
   */
  int (*status)(struct sess_cache_st *cache, void (*writef)(void *, const char *, ...), void *arg, int flags);

} tls_sess_cache_t;

/* Use this flag to indicate to the status callback that details on all
 * sessions in the cache are to be shown.  These details include the
 * session ID, session ID context, session creation time, session expiration
 * time, session protocol (SSLv3, TLSv1, etc), and ciphersuite.
 */
#define TLS_SESS_CACHE_STATUS_FL_SHOW_SESSIONS		0x001

int tls_sess_cache_register(const char *name, tls_sess_cache_t *handler);
int tls_sess_cache_unregister(const char *name);

#endif /* MOD_TLS_H */
