/*
   Unix SMB/CIFS implementation.
   Character set conversion Extensions
   Copyright (C) Igor Vergeichik <iverg@mail.ru> 2001
   Copyright (C) Andrew Tridgell 2001
   Copyright (C) Simo Sorce 2001
   Copyright (C) Martin Pool 2003

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "includes.h"

/* We can parameterize this if someone complains.... JRA. */

char lp_failed_convert_char(void)
{
	return '_';
}

/**
 * @file
 *
 * @brief Character-set conversion routines built on our iconv.
 *
 * @note Samba's internal character set (at least in the 3.0 series)
 * is always the same as the one for the Unix filesystem.  It is
 * <b>not</b> necessarily UTF-8 and may be different on machines that
 * need i18n filenames to be compatible with Unix software.  It does
 * have to be a superset of ASCII.  All multibyte sequences must start
 * with a byte with the high bit set.
 *
 * @sa lib/iconv.c
 */


static smb_iconv_t conv_handles[NUM_CHARSETS][NUM_CHARSETS];
static bool conv_silent; /* Should we do a debug if the conversion fails ? */
static bool initialized = False;

/**
 * Return the name of a charset to give to iconv().
 **/
static const char *charset_name(charset_t ch)
{
	const char *ret = NULL;

	if (ch == CH_UTF16LE) ret = "UTF-16LE";
	else if (ch == CH_UTF16BE) ret = "UTF-16BE";
	else if (ch == CH_UNIX) ret = lp_unix_charset();
	else if (ch == CH_DOS) ret = lp_dos_charset();
	else if (ch == CH_DISPLAY) ret = lp_display_charset();
	else if (ch == CH_UTF8) ret = "UTF8";

#if defined(HAVE_NL_LANGINFO) && defined(CODESET)
	if (ret && !strcmp(ret, "LOCALE")) {
		const char *ln = NULL;

#ifdef HAVE_SETLOCALE
		setlocale(LC_ALL, "");
#endif
		ln = nl_langinfo(CODESET);
		if (ln) {
			/* Check whether the charset name is supported
			   by iconv */
			smb_iconv_t handle = smb_iconv_open(ln,"UCS-2LE");
			if (handle == (smb_iconv_t) -1) {
				DEBUG(5,("Locale charset '%s' unsupported, using ASCII instead\n", ln));
				ln = NULL;
			} else {
				DEBUG(5,("Substituting charset '%s' for LOCALE\n", ln));
				smb_iconv_close(handle);
			}
		}
		ret = ln;
	}
#endif

	if (!ret || !*ret) ret = "ASCII";
	return ret;
}

void lazy_initialize_conv(void)
{
	if (!initialized) {
		load_case_tables();
		init_iconv();
		initialized = true;
	}
}

/**
 * Destroy global objects allocated by init_iconv()
 **/
void gfree_charcnv(void)
{
	int c1, c2;

	for (c1=0;c1<NUM_CHARSETS;c1++) {
		for (c2=0;c2<NUM_CHARSETS;c2++) {
			if ( conv_handles[c1][c2] ) {
				smb_iconv_close( conv_handles[c1][c2] );
				conv_handles[c1][c2] = 0;
			}
		}
	}
	initialized = false;
}

/**
 * Initialize iconv conversion descriptors.
 *
 * This is called the first time it is needed, and also called again
 * every time the configuration is reloaded, because the charset or
 * codepage might have changed.
 **/
void init_iconv(void)
{
	int c1, c2;
	bool did_reload = False;
#ifdef AVM_FIX
	static short nesting = 0;

	if (nesting) return;
	nesting++;
#endif

	/* so that charset_name() works we need to get the UNIX<->UCS2 going
	   first */
	if (!conv_handles[CH_UNIX][CH_UTF16LE])
		conv_handles[CH_UNIX][CH_UTF16LE] = smb_iconv_open(charset_name(CH_UTF16LE), "ASCII");

	if (!conv_handles[CH_UTF16LE][CH_UNIX])
		conv_handles[CH_UTF16LE][CH_UNIX] = smb_iconv_open("ASCII", charset_name(CH_UTF16LE));

	for (c1=0;c1<NUM_CHARSETS;c1++) {
		for (c2=0;c2<NUM_CHARSETS;c2++) {
			const char *n1 = charset_name((charset_t)c1);
			const char *n2 = charset_name((charset_t)c2);
			if (conv_handles[c1][c2] &&
			    strcmp(n1, conv_handles[c1][c2]->from_name) == 0 &&
			    strcmp(n2, conv_handles[c1][c2]->to_name) == 0)
				continue;

			did_reload = True;

			if (conv_handles[c1][c2])
				smb_iconv_close(conv_handles[c1][c2]);

			conv_handles[c1][c2] = smb_iconv_open(n2,n1);
			if (conv_handles[c1][c2] == (smb_iconv_t)-1) {
				DEBUG(0,("init_iconv: Conversion from %s to %s not supported\n",
					 charset_name((charset_t)c1), charset_name((charset_t)c2)));
				if (c1 != CH_UTF16LE && c1 != CH_UTF16BE) {
					n1 = "ASCII";
				}
				if (c2 != CH_UTF16LE && c2 != CH_UTF16BE) {
					n2 = "ASCII";
				}
				DEBUG(0,("init_iconv: Attempting to replace with conversion from %s to %s\n",
					n1, n2 ));
				conv_handles[c1][c2] = smb_iconv_open(n2,n1);
				if (!conv_handles[c1][c2]) {
					DEBUG(0,("init_iconv: Conversion from %s to %s failed", n1, n2));
					smb_panic("init_iconv: conv_handle initialization failed");
				}
			}
		}
	}

	if (did_reload) {
		/* XXX: Does this really get called every time the dos
		 * codepage changes? */
		/* XXX: Is the did_reload test too strict? */
		conv_silent = True;
		init_valid_table();
		conv_silent = False;
	}
#ifdef AVM_FIX
	nesting--;
#endif
}

/**
 * Convert string from one encoding to another, making error checking etc
 * Slow path version - uses (slow) iconv.
 *
 * @param src pointer to source string (multibyte or singlebyte)
 * @param srclen length of the source string in bytes
 * @param dest pointer to destination string (multibyte or singlebyte)
 * @param destlen maximal length allowed for string
 * @param allow_bad_conv determines if a "best effort" conversion is acceptable (never returns errors)
 * @returns the number of bytes occupied in the destination
 *
 * Ensure the srclen contains the terminating zero.
 *
 **/

static size_t convert_string_internal(charset_t from, charset_t to,
		      void const *src, size_t srclen, 
		      void *dest, size_t destlen, bool allow_bad_conv)
{
	size_t i_len, o_len;
	size_t retval;
	const char* inbuf = (const char*)src;
	char* outbuf = (char*)dest;
	smb_iconv_t descriptor;

	lazy_initialize_conv();

	descriptor = conv_handles[from][to];

	if (srclen == (size_t)-1) {
		if (from == CH_UTF16LE || from == CH_UTF16BE) {
			srclen = (strlen_w((const smb_ucs2_t *)src)+1) * 2;
		} else {
			srclen = strlen((const char *)src)+1;
		}
	}


	if (descriptor == (smb_iconv_t)-1 || descriptor == (smb_iconv_t)0) {
		if (!conv_silent)
			DEBUG(0,("convert_string_internal: Conversion not supported.\n"));
		return (size_t)-1;
	}

	i_len=srclen;
	o_len=destlen;

 again:

	retval = smb_iconv(descriptor, &inbuf, &i_len, &outbuf, &o_len);
	if(retval==(size_t)-1) {
	    	const char *reason="unknown error";
		switch(errno) {
			case EINVAL:
				reason="Incomplete multibyte sequence";
				if (!conv_silent)
					DEBUG(3,("convert_string_internal: Conversion error: %s(%s)\n",reason,inbuf));
				if (allow_bad_conv)
					goto use_as_is;
				return (size_t)-1;
			case E2BIG:
				reason="No more room"; 
				if (!conv_silent) {
					if (from == CH_UNIX) {
						DEBUG(3,("E2BIG: convert_string(%s,%s): srclen=%u destlen=%u - '%s'\n",
							charset_name(from), charset_name(to),
							(unsigned int)srclen, (unsigned int)destlen, (const char *)src));
					} else {
						DEBUG(3,("E2BIG: convert_string(%s,%s): srclen=%u destlen=%u\n",
							charset_name(from), charset_name(to),
							(unsigned int)srclen, (unsigned int)destlen));
					}
				}
				break;
			case EILSEQ:
				reason="Illegal multibyte sequence";
				if (!conv_silent)
					DEBUG(3,("convert_string_internal: Conversion error: %s(%s)\n",reason,inbuf));
				if (allow_bad_conv)
					goto use_as_is;
				
				return (size_t)-1;
			default:
				if (!conv_silent)
					DEBUG(0,("convert_string_internal: Conversion error: %s(%s)\n",reason,inbuf));
				return (size_t)-1;
		}
		/* smb_panic(reason); */
	}
	return destlen-o_len;

 use_as_is:

	/* 
	 * Conversion not supported. This is actually an error, but there are so
	 * many misconfigured iconv systems and smb.conf's out there we can't just
	 * fail. Do a very bad conversion instead.... JRA.
	 */

	{
		if (o_len == 0 || i_len == 0)
			return destlen - o_len;

		if (((from == CH_UTF16LE)||(from == CH_UTF16BE)) &&
				((to != CH_UTF16LE)||(to != CH_UTF16BE))) {
			/* Can't convert from utf16 any endian to multibyte.
			   Replace with the default fail char.
			*/
			if (i_len < 2)
				return destlen - o_len;
			if (i_len >= 2) {
				*outbuf = lp_failed_convert_char();

				outbuf++;
				o_len--;

				inbuf += 2;
				i_len -= 2;
			}

			if (o_len == 0 || i_len == 0)
				return destlen - o_len;

			/* Keep trying with the next char... */
			goto again;

		} else if (from != CH_UTF16LE && from != CH_UTF16BE && to == CH_UTF16LE) {
			/* Can't convert to UTF16LE - just widen by adding the
			   default fail char then zero.
			*/
			if (o_len < 2)
				return destlen - o_len;

			outbuf[0] = lp_failed_convert_char();
			outbuf[1] = '\0';

			inbuf++;
			i_len--;

			outbuf += 2;
			o_len -= 2;

			if (o_len == 0 || i_len == 0)
				return destlen - o_len;

			/* Keep trying with the next char... */
			goto again;

		} else if (from != CH_UTF16LE && from != CH_UTF16BE &&
				to != CH_UTF16LE && to != CH_UTF16BE) {
			/* Failed multibyte to multibyte. Just copy the default fail char and
				try again. */
			outbuf[0] = lp_failed_convert_char();

			inbuf++;
			i_len--;

			outbuf++;
			o_len--;

			if (o_len == 0 || i_len == 0)
				return destlen - o_len;

			/* Keep trying with the next char... */
			goto again;

		} else {
			/* Keep compiler happy.... */
			return destlen - o_len;
		}
	}
}

/**
 * Convert string from one encoding to another, making error checking etc
 * Fast path version - handles ASCII first.
 *
 * @param src pointer to source string (multibyte or singlebyte)
 * @param srclen length of the source string in bytes, or -1 for nul terminated.
 * @param dest pointer to destination string (multibyte or singlebyte)
 * @param destlen maximal length allowed for string - *NEVER* -1.
 * @param allow_bad_conv determines if a "best effort" conversion is acceptable (never returns errors)
 * @returns the number of bytes occupied in the destination
 *
 * Ensure the srclen contains the terminating zero.
 *
 * This function has been hand-tuned to provide a fast path.
 * Don't change unless you really know what you are doing. JRA.
 **/

size_t convert_string(charset_t from, charset_t to,
		      void const *src, size_t srclen, 
		      void *dest, size_t destlen, bool allow_bad_conv)
{
	/*
	 * NB. We deliberately don't do a strlen here if srclen == -1.
	 * This is very expensive over millions of calls and is taken
	 * care of in the slow path in convert_string_internal. JRA.
	 */

#ifdef DEVELOPER
	SMB_ASSERT(destlen != (size_t)-1);
#endif

	if (srclen == 0)
		return 0;

	if (from != CH_UTF16LE && from != CH_UTF16BE && to != CH_UTF16LE && to != CH_UTF16BE) {
		const unsigned char *p = (const unsigned char *)src;
		unsigned char *q = (unsigned char *)dest;
		size_t slen = srclen;
		size_t dlen = destlen;
		unsigned char lastp = '\0';
		size_t retval = 0;

		/* If all characters are ascii, fast path here. */
		while (slen && dlen) {
			if ((lastp = *p) <= 0x7f) {
				*q++ = *p++;
				if (slen != (size_t)-1) {
					slen--;
				}
				dlen--;
				retval++;
				if (!lastp)
					break;
			} else {
#ifdef BROKEN_UNICODE_COMPOSE_CHARACTERS
				goto general_case;
#else
				size_t ret = convert_string_internal(from, to, p, slen, q, dlen, allow_bad_conv);
				if (ret == (size_t)-1) {
					return ret;
				}
				return retval + ret;
#endif
			}
		}
		if (!dlen) {
			/* Even if we fast path we should note if we ran out of room. */
			if (((slen != (size_t)-1) && slen) ||
					((slen == (size_t)-1) && lastp)) {
				errno = E2BIG;
			}
		}
		return retval;
	} else if (from == CH_UTF16LE && to != CH_UTF16LE) {
		const unsigned char *p = (const unsigned char *)src;
		unsigned char *q = (unsigned char *)dest;
		size_t retval = 0;
		size_t slen = srclen;
		size_t dlen = destlen;
		unsigned char lastp = '\0';

		/* If all characters are ascii, fast path here. */
		while (((slen == (size_t)-1) || (slen >= 2)) && dlen) {
			if (((lastp = *p) <= 0x7f) && (p[1] == 0)) {
				*q++ = *p;
				if (slen != (size_t)-1) {
					slen -= 2;
				}
				p += 2;
				dlen--;
				retval++;
				if (!lastp)
					break;
			} else {
#ifdef BROKEN_UNICODE_COMPOSE_CHARACTERS
				goto general_case;
#else
				size_t ret = convert_string_internal(from, to, p, slen, q, dlen, allow_bad_conv);
				if (ret == (size_t)-1) {
					return ret;
				}
				return retval + ret;
#endif
			}
		}
		if (!dlen) {
			/* Even if we fast path we should note if we ran out of room. */
			if (((slen != (size_t)-1) && slen) ||
					((slen == (size_t)-1) && lastp)) {
				errno = E2BIG;
			}
		}
		return retval;
	} else if (from != CH_UTF16LE && from != CH_UTF16BE && to == CH_UTF16LE) {
		const unsigned char *p = (const unsigned char *)src;
		unsigned char *q = (unsigned char *)dest;
		size_t retval = 0;
		size_t slen = srclen;
		size_t dlen = destlen;
		unsigned char lastp = '\0';

		/* If all characters are ascii, fast path here. */
		while (slen && (dlen >= 2)) {
			if ((lastp = *p) <= 0x7F) {
				*q++ = *p++;
				*q++ = '\0';
				if (slen != (size_t)-1) {
					slen--;
				}
				dlen -= 2;
				retval += 2;
				if (!lastp)
					break;
			} else {
#ifdef BROKEN_UNICODE_COMPOSE_CHARACTERS
				goto general_case;
#else
				size_t ret = convert_string_internal(from, to, p, slen, q, dlen, allow_bad_conv);
				if (ret == (size_t)-1) {
					return ret;
				}
				return retval + ret;
#endif
			}
		}
		if (!dlen) {
			/* Even if we fast path we should note if we ran out of room. */
			if (((slen != (size_t)-1) && slen) ||
					((slen == (size_t)-1) && lastp)) {
				errno = E2BIG;
			}
		}
		return retval;
	}

#ifdef BROKEN_UNICODE_COMPOSE_CHARACTERS
  general_case:
#endif
	return convert_string_internal(from, to, src, srclen, dest, destlen, allow_bad_conv);
}

/**
 * Convert between character sets, allocating a new buffer for the result.
 *
 * @param ctx TALLOC_CTX to use to allocate with. If NULL use malloc.
 * (this is a bad interface and needs fixing. JRA).
 * @param srclen length of source buffer.
 * @param dest always set at least to NULL
 * @param converted_size set to the size of the allocated buffer on return
 * true
 * @note -1 is not accepted for srclen.
 *
 * @return true if new buffer was correctly allocated, and string was
 * converted.
 *
 * Ensure the srclen contains the terminating zero.
 *
 * I hate the goto's in this function. It's embarressing.....
 * There has to be a cleaner way to do this. JRA.
 **/

bool convert_string_allocate(TALLOC_CTX *ctx, charset_t from, charset_t to,
			     void const *src, size_t srclen, void *dst,
			     size_t *converted_size, bool allow_bad_conv)
{
	size_t i_len, o_len, destlen = (srclen * 3) / 2;
	size_t retval;
	const char *inbuf = (const char *)src;
	char *outbuf = NULL, *ob = NULL;
	smb_iconv_t descriptor;
	void **dest = (void **)dst;

	*dest = NULL;

	if (!converted_size) {
		errno = EINVAL;
		return false;
	}

	if (src == NULL || srclen == (size_t)-1) {
		errno = EINVAL;
		return false;
	}
	if (srclen == 0) {
		ob = ((ctx != NULL) ? talloc_strdup(ctx, "") : SMB_STRDUP(""));
		if (ob == NULL) {
			errno = ENOMEM;
			return false;
		}
		*dest = ob;
		*converted_size = 0;
		return true;
	}

	lazy_initialize_conv();

	descriptor = conv_handles[from][to];

	if (descriptor == (smb_iconv_t)-1 || descriptor == (smb_iconv_t)0) {
		if (!conv_silent)
			DEBUG(0,("convert_string_allocate: Conversion not supported.\n"));
		errno = EOPNOTSUPP;
		return false;
	}

  convert:

	/* +2 is for ucs2 null termination. */
	if ((destlen*2)+2 < destlen) {
		/* wrapped ! abort. */
		if (!conv_silent)
			DEBUG(0, ("convert_string_allocate: destlen wrapped !\n"));
		if (!ctx)
			SAFE_FREE(outbuf);
		errno = EOPNOTSUPP;
		return false;
	} else {
		destlen = destlen * 2;
	}

	/* +2 is for ucs2 null termination. */
	if (ctx) {
		ob = (char *)TALLOC_REALLOC(ctx, ob, destlen + 2);
	} else {
		ob = (char *)SMB_REALLOC(ob, destlen + 2);
	}

	if (!ob) {
		DEBUG(0, ("convert_string_allocate: realloc failed!\n"));
		errno = ENOMEM;
		return false;
	}
	outbuf = ob;
	i_len = srclen;
	o_len = destlen;

 again:

	retval = smb_iconv(descriptor,
			   &inbuf, &i_len,
			   &outbuf, &o_len);
	if(retval == (size_t)-1) 		{
	    	const char *reason="unknown error";
		switch(errno) {
			case EINVAL:
				reason="Incomplete multibyte sequence";
				if (!conv_silent)
					DEBUG(3,("convert_string_allocate: Conversion error: %s(%s)\n",reason,inbuf));
				if (allow_bad_conv)
					goto use_as_is;
				break;
			case E2BIG:
				goto convert;
			case EILSEQ:
				reason="Illegal multibyte sequence";
				if (!conv_silent)
					DEBUG(3,("convert_string_allocate: Conversion error: %s(%s)\n",reason,inbuf));
				if (allow_bad_conv)
					goto use_as_is;
				break;
		}
		if (!conv_silent)
			DEBUG(0,("Conversion error: %s(%s)\n",reason,inbuf));
		/* smb_panic(reason); */
		if (ctx) {
			TALLOC_FREE(ob);
		} else {
			SAFE_FREE(ob);
		}
		return false;
	}

  out:

	destlen = destlen - o_len;
	/* Don't shrink unless we're reclaiming a lot of
	 * space. This is in the hot codepath and these
	 * reallocs *cost*. JRA.
	 */
	if (o_len > 1024) {
		/* We're shrinking here so we know the +2 is safe from wrap. */
		if (ctx) {
			ob = (char *)TALLOC_REALLOC(ctx,ob,destlen + 2);
		} else {
			ob = (char *)SMB_REALLOC(ob,destlen + 2);
		}
	}

	if (destlen && !ob) {
		DEBUG(0, ("convert_string_allocate: out of memory!\n"));
		errno = ENOMEM;
		return false;
	}

	*dest = ob;

	/* Must ucs2 null terminate in the extra space we allocated. */
	ob[destlen] = '\0';
	ob[destlen+1] = '\0';

	*converted_size = destlen;
	return true;

 use_as_is:

	/* 
	 * Conversion not supported. This is actually an error, but there are so
	 * many misconfigured iconv systems and smb.conf's out there we can't just
	 * fail. Do a very bad conversion instead.... JRA.
	 */

	{
		if (o_len == 0 || i_len == 0)
			goto out;

		if (((from == CH_UTF16LE)||(from == CH_UTF16BE)) &&
				((to != CH_UTF16LE)||(to != CH_UTF16BE))) {
			/* Can't convert from utf16 any endian to multibyte.
			   Replace with the default fail char.
			*/

			if (i_len < 2)
				goto out;

			if (i_len >= 2) {
				*outbuf = lp_failed_convert_char();

				outbuf++;
				o_len--;

				inbuf += 2;
				i_len -= 2;
			}

			if (o_len == 0 || i_len == 0)
				goto out;

			/* Keep trying with the next char... */
			goto again;

		} else if (from != CH_UTF16LE && from != CH_UTF16BE && to == CH_UTF16LE) {
			/* Can't convert to UTF16LE - just widen by adding the
			   default fail char then zero.
			*/
			if (o_len < 2)
				goto out;

			outbuf[0] = lp_failed_convert_char();
			outbuf[1] = '\0';

			inbuf++;
			i_len--;

			outbuf += 2;
			o_len -= 2;

			if (o_len == 0 || i_len == 0)
				goto out;

			/* Keep trying with the next char... */
			goto again;

		} else if (from != CH_UTF16LE && from != CH_UTF16BE &&
				to != CH_UTF16LE && to != CH_UTF16BE) {
			/* Failed multibyte to multibyte. Just copy the default fail char and
			   try again. */
			outbuf[0] = lp_failed_convert_char();

			inbuf++;
			i_len--;

			outbuf++;
			o_len--;

			if (o_len == 0 || i_len == 0)
				goto out;

			/* Keep trying with the next char... */
			goto again;

		} else {
			/* Keep compiler happy.... */
			goto out;
		}
	}
}

/**
 * Convert between character sets, allocating a new buffer using talloc for the result.
 *
 * @param srclen length of source buffer.
 * @param dest always set at least to NULL
 * @parm converted_size set to the number of bytes occupied by the string in
 * the destination on success.
 * @note -1 is not accepted for srclen.
 *
 * @return true if new buffer was correctly allocated, and string was
 * converted.
 */
bool convert_string_talloc(TALLOC_CTX *ctx, charset_t from, charset_t to,
			   void const *src, size_t srclen, void *dst,
			   size_t *converted_size, bool allow_bad_conv)
{
	void **dest = (void **)dst;

	*dest = NULL;
	return convert_string_allocate(ctx, from, to, src, srclen, dest,
				       converted_size, allow_bad_conv);
}

size_t unix_strupper(const char *src, size_t srclen, char *dest, size_t destlen)
{
	size_t size;
	smb_ucs2_t *buffer;

	if (!push_ucs2_allocate(&buffer, src, &size)) {
		return (size_t)-1;
	}

	if (!strupper_w(buffer) && (dest == src)) {
		free(buffer);
		return srclen;
	}

	size = convert_string(CH_UTF16LE, CH_UNIX, buffer, size, dest, destlen, True);
	free(buffer);
	return size;
}

/**
 strdup() a unix string to upper case.
**/

char *strdup_upper(const char *s)
{
	char *out_buffer = SMB_STRDUP(s);
	const unsigned char *p = (const unsigned char *)s;
	unsigned char *q = (unsigned char *)out_buffer;

	if (!q) {
		return NULL;
	}

	/* this is quite a common operation, so we want it to be
	   fast. We optimise for the ascii case, knowing that all our
	   supported multi-byte character sets are ascii-compatible
	   (ie. they match for the first 128 chars) */

	while (*p) {
		if (*p & 0x80)
			break;
		*q++ = toupper_ascii_fast(*p);
		p++;
	}

	if (*p) {
		/* MB case. */
		size_t converted_size, converted_size2;
		smb_ucs2_t *buffer = NULL;

		SAFE_FREE(out_buffer);
		if (!convert_string_allocate(NULL, CH_UNIX, CH_UTF16LE, s,
					     strlen(s) + 1,
					     (void **)(void *)&buffer,
					     &converted_size, True))
		{
			return NULL;
		}

		strupper_w(buffer);

		if (!convert_string_allocate(NULL, CH_UTF16LE, CH_UNIX, buffer,
					     converted_size,
					     (void **)(void *)&out_buffer,
					     &converted_size2, True))
		{
			TALLOC_FREE(buffer);
			return NULL;
		}

		/* Don't need the intermediate buffer
 		 * anymore.
 		 */
		TALLOC_FREE(buffer);
	}

	return out_buffer;
}

/**
 talloc_strdup() a unix string to upper case.
**/

char *talloc_strdup_upper(TALLOC_CTX *ctx, const char *s)
{
	char *out_buffer = talloc_strdup(ctx,s);
	const unsigned char *p = (const unsigned char *)s;
	unsigned char *q = (unsigned char *)out_buffer;

	if (!q) {
		return NULL;
	}

	/* this is quite a common operation, so we want it to be
	   fast. We optimise for the ascii case, knowing that all our
	   supported multi-byte character sets are ascii-compatible
	   (ie. they match for the first 128 chars) */

	while (*p) {
		if (*p & 0x80)
			break;
		*q++ = toupper_ascii_fast(*p);
		p++;
	}

	if (*p) {
		/* MB case. */
		size_t converted_size, converted_size2;
		smb_ucs2_t *ubuf = NULL;

		/* We're not using the ascii buffer above. */
		TALLOC_FREE(out_buffer);

		if (!convert_string_talloc(ctx, CH_UNIX, CH_UTF16LE, s,
					   strlen(s)+1, (void *)&ubuf,
					   &converted_size, True))
		{
			return NULL;
		}

		strupper_w(ubuf);

		if (!convert_string_talloc(ctx, CH_UTF16LE, CH_UNIX, ubuf,
					   converted_size, (void *)&out_buffer,
					   &converted_size2, True))
		{
			TALLOC_FREE(ubuf);
			return NULL;
		}

		/* Don't need the intermediate buffer
 		 * anymore.
 		 */
		TALLOC_FREE(ubuf);
	}

	return out_buffer;
}

size_t unix_strlower(const char *src, size_t srclen, char *dest, size_t destlen)
{
	size_t size;
	smb_ucs2_t *buffer = NULL;

	if (!convert_string_allocate(NULL, CH_UNIX, CH_UTF16LE, src, srclen,
				     (void **)(void *)&buffer, &size,
				     True))
	{
		smb_panic("failed to create UCS2 buffer");
	}
	if (!strlower_w(buffer) && (dest == src)) {
		SAFE_FREE(buffer);
		return srclen;
	}
	size = convert_string(CH_UTF16LE, CH_UNIX, buffer, size, dest, destlen, True);
	SAFE_FREE(buffer);
	return size;
}

/**
 strdup() a unix string to lower case.
**/

char *strdup_lower(const char *s)
{
	size_t converted_size;
	smb_ucs2_t *buffer = NULL;
	char *out_buffer;

	if (!push_ucs2_allocate(&buffer, s, &converted_size)) {
		return NULL;
	}

	strlower_w(buffer);

	if (!pull_ucs2_allocate(&out_buffer, buffer, &converted_size)) {
		SAFE_FREE(buffer);
		return NULL;
	}

	SAFE_FREE(buffer);

	return out_buffer;
}

char *talloc_strdup_lower(TALLOC_CTX *ctx, const char *s)
{
	size_t converted_size;
	smb_ucs2_t *buffer = NULL;
	char *out_buffer;

	if (!push_ucs2_talloc(ctx, &buffer, s, &converted_size)) {
		return NULL;
	}

	strlower_w(buffer);

	if (!pull_ucs2_talloc(ctx, &out_buffer, buffer, &converted_size)) {
		TALLOC_FREE(buffer);
		return NULL;
	}

	TALLOC_FREE(buffer);

	return out_buffer;
}


size_t ucs2_align(const void *base_ptr, const void *p, int flags)
{
	if (flags & (STR_NOALIGN|STR_ASCII))
		return 0;
	return PTR_DIFF(p, base_ptr) & 1;
}


/**
 * Copy a string from a char* unix src to a dos codepage string destination.
 *
 * @return the number of bytes occupied by the string in the destination.
 *
 * @param flags can include
 * <dl>
 * <dt>STR_TERMINATE</dt> <dd>means include the null termination</dd>
 * <dt>STR_UPPER</dt> <dd>means uppercase in the destination</dd>
 * </dl>
 *
 * @param dest_len the maximum length in bytes allowed in the
 * destination.
 **/
size_t push_ascii(void *dest, const char *src, size_t dest_len, int flags)
{
	size_t src_len = strlen(src);
	char *tmpbuf = NULL;
	size_t ret;

	/* No longer allow a length of -1. */
	if (dest_len == (size_t)-1) {
		smb_panic("push_ascii - dest_len == -1");
	}

	if (flags & STR_UPPER) {
		tmpbuf = SMB_STRDUP(src);
		if (!tmpbuf) {
			smb_panic("malloc fail");
		}
		strupper_m(tmpbuf);
		src = tmpbuf;
	}

	if (flags & (STR_TERMINATE | STR_TERMINATE_ASCII)) {
		src_len++;
	}

	ret = convert_string(CH_UNIX, CH_DOS, src, src_len, dest, dest_len, True);
	if (ret == (size_t)-1 &&
			(flags & (STR_TERMINATE | STR_TERMINATE_ASCII))
			&& dest_len > 0) {
		((char *)dest)[0] = '\0';
	}
	SAFE_FREE(tmpbuf);
	return ret;
}

size_t push_ascii_fstring(void *dest, const char *src)
{
	return push_ascii(dest, src, sizeof(fstring), STR_TERMINATE);
}

/********************************************************************
 Push an nstring - ensure null terminated. Written by
 moriyama@miraclelinux.com (MORIYAMA Masayuki).
********************************************************************/

size_t push_ascii_nstring(void *dest, const char *src)
{
	size_t i, buffer_len, dest_len;
	smb_ucs2_t *buffer;

	conv_silent = True;
	if (!push_ucs2_allocate(&buffer, src, &buffer_len)) {
		smb_panic("failed to create UCS2 buffer");
	}

	/* We're using buffer_len below to count ucs2 characters, not bytes. */
	buffer_len /= sizeof(smb_ucs2_t);

	dest_len = 0;
	for (i = 0; buffer[i] != 0 && (i < buffer_len); i++) {
		unsigned char mb[10];
		/* Convert one smb_ucs2_t character at a time. */
		size_t mb_len = convert_string(CH_UTF16LE, CH_DOS, buffer+i, sizeof(smb_ucs2_t), mb, sizeof(mb), False);
		if ((mb_len != (size_t)-1) && (dest_len + mb_len <= MAX_NETBIOSNAME_LEN - 1)) {
			memcpy((char *)dest + dest_len, mb, mb_len);
			dest_len += mb_len;
		} else {
			errno = E2BIG;
			break;
		}
	}
	((char *)dest)[dest_len] = '\0';

	SAFE_FREE(buffer);
	conv_silent = False;
	return dest_len;
}

/********************************************************************
 Push and malloc an ascii string. src and dest null terminated.
********************************************************************/

bool push_ascii_allocate(char **dest, const char *src, size_t *converted_size)
{
	size_t src_len = strlen(src)+1;

	*dest = NULL;
	return convert_string_allocate(NULL, CH_UNIX, CH_DOS, src, src_len,
				       (void **)dest, converted_size, True);
}

/**
 * Copy a string from a dos codepage source to a unix char* destination.
 *
 * The resulting string in "dest" is always null terminated.
 *
 * @param flags can have:
 * <dl>
 * <dt>STR_TERMINATE</dt>
 * <dd>STR_TERMINATE means the string in @p src
 * is null terminated, and src_len is ignored.</dd>
 * </dl>
 *
 * @param src_len is the length of the source area in bytes.
 * @returns the number of bytes occupied by the string in @p src.
 **/
size_t pull_ascii(char *dest, const void *src, size_t dest_len, size_t src_len, int flags)
{
	size_t ret;

	if (dest_len == (size_t)-1) {
		/* No longer allow dest_len of -1. */
		smb_panic("pull_ascii - invalid dest_len of -1");
	}

	if (flags & STR_TERMINATE) {
		if (src_len == (size_t)-1) {
			src_len = strlen((const char *)src) + 1;
		} else {
			size_t len = strnlen((const char *)src, src_len);
			if (len < src_len)
				len++;
			src_len = len;
		}
	}

	ret = convert_string(CH_DOS, CH_UNIX, src, src_len, dest, dest_len, True);
	if (ret == (size_t)-1) {
		ret = 0;
		dest_len = 0;
	}

	if (dest_len && ret) {
		/* Did we already process the terminating zero ? */
		if (dest[MIN(ret-1, dest_len-1)] != 0) {
			dest[MIN(ret, dest_len-1)] = 0;
		}
	} else  {
		dest[0] = 0;
	}

	return src_len;
}

/**
 * Copy a string from a dos codepage source to a unix char* destination.
 Talloc version.
 Uses malloc if TALLOC_CTX is NULL (this is a bad interface and
 needs fixing. JRA).
 *
 * The resulting string in "dest" is always null terminated.
 *
 * @param flags can have:
 * <dl>
 * <dt>STR_TERMINATE</dt>
 * <dd>STR_TERMINATE means the string in @p src
 * is null terminated, and src_len is ignored.</dd>
 * </dl>
 *
 * @param src_len is the length of the source area in bytes.
 * @returns the number of bytes occupied by the string in @p src.
 **/

static size_t pull_ascii_base_talloc(TALLOC_CTX *ctx,
					char **ppdest,
					const void *src,
					size_t src_len,
					int flags)
{
	char *dest = NULL;
	size_t dest_len;

#ifdef DEVELOPER
	/* Ensure we never use the braindead "malloc" varient. */
	if (ctx == NULL) {
		smb_panic("NULL talloc CTX in pull_ascii_base_talloc\n");
	}
#endif

	*ppdest = NULL;

	if (!src_len) {
		return 0;
	}

	if (flags & STR_TERMINATE) {
		if (src_len == (size_t)-1) {
			src_len = strlen((const char *)src) + 1;
		} else {
			size_t len = strnlen((const char *)src, src_len);
			if (len < src_len)
				len++;
			src_len = len;
		}
		/* Ensure we don't use an insane length from the client. */
		if (src_len >= 1024*1024) {
			char *msg = talloc_asprintf(ctx,
					"Bad src length (%u) in "
					"pull_ascii_base_talloc",
					(unsigned int)src_len);
			smb_panic(msg);
		}
	} else {
		/* Can't have an unlimited length
 		 * non STR_TERMINATE'd.
 		 */
		if (src_len == (size_t)-1) {
			errno = EINVAL;
			return 0;
		}
	}

	/* src_len != -1 here. */

	if (!convert_string_allocate(ctx, CH_DOS, CH_UNIX, src, src_len, &dest,
				     &dest_len, True)) {
		dest_len = 0;
	}

	if (dest_len && dest) {
		/* Did we already process the terminating zero ? */
		if (dest[dest_len-1] != 0) {
			size_t size = talloc_get_size(dest);
			/* Have we got space to append the '\0' ? */
			if (size <= dest_len) {
				/* No, realloc. */
				dest = TALLOC_REALLOC_ARRAY(ctx, dest, char,
						dest_len+1);
				if (!dest) {
					/* talloc fail. */
					dest_len = (size_t)-1;
					return 0;
				}
			}
			/* Yay - space ! */
			dest[dest_len] = '\0';
			dest_len++;
		}
	} else if (dest) {
		dest[0] = 0;
	}

	*ppdest = dest;
	return src_len;
}

size_t pull_ascii_fstring(char *dest, const void *src)
{
	return pull_ascii(dest, src, sizeof(fstring), -1, STR_TERMINATE);
}

/* When pulling an nstring it can expand into a larger size (dos cp -> utf8). Cope with this. */

size_t pull_ascii_nstring(char *dest, size_t dest_len, const void *src)
{
	return pull_ascii(dest, src, dest_len, sizeof(nstring)-1, STR_TERMINATE);
}

/**
 * Copy a string from a char* src to a unicode destination.
 *
 * @returns the number of bytes occupied by the string in the destination.
 *
 * @param flags can have:
 *
 * <dl>
 * <dt>STR_TERMINATE <dd>means include the null termination.
 * <dt>STR_UPPER     <dd>means uppercase in the destination.
 * <dt>STR_NOALIGN   <dd>means don't do alignment.
 * </dl>
 *
 * @param dest_len is the maximum length allowed in the
 * destination.
 **/

size_t push_ucs2(const void *base_ptr, void *dest, const char *src, size_t dest_len, int flags)
{
	size_t len=0;
	size_t src_len;
	size_t ret;

	if (dest_len == (size_t)-1) {
		/* No longer allow dest_len of -1. */
		smb_panic("push_ucs2 - invalid dest_len of -1");
	}

	if (flags & STR_TERMINATE)
		src_len = (size_t)-1;
	else
		src_len = strlen(src);

	if (ucs2_align(base_ptr, dest, flags)) {
		*(char *)dest = 0;
		dest = (void *)((char *)dest + 1);
		if (dest_len)
			dest_len--;
		len++;
	}

	/* ucs2 is always a multiple of 2 bytes */
	dest_len &= ~1;

	ret =  convert_string(CH_UNIX, CH_UTF16LE, src, src_len, dest, dest_len, True);
	if (ret == (size_t)-1) {
		if ((flags & STR_TERMINATE) &&
				dest &&
				dest_len) {
			*(char *)dest = 0;
		}
		return len;
	}

	len += ret;

	if (flags & STR_UPPER) {
		smb_ucs2_t *dest_ucs2 = (smb_ucs2_t *)dest;
		size_t i;

		/* We check for i < (ret / 2) below as the dest string isn't null
		   terminated if STR_TERMINATE isn't set. */

		for (i = 0; i < (ret / 2) && i < (dest_len / 2) && dest_ucs2[i]; i++) {
			smb_ucs2_t v = toupper_w(dest_ucs2[i]);
			if (v != dest_ucs2[i]) {
				dest_ucs2[i] = v;
			}
		}
	}

	return len;
}


/**
 * Copy a string from a unix char* src to a UCS2 destination,
 * allocating a buffer using talloc().
 *
 * @param dest always set at least to NULL 
 * @parm converted_size set to the number of bytes occupied by the string in
 * the destination on success.
 *
 * @return true if new buffer was correctly allocated, and string was
 * converted.
 **/
bool push_ucs2_talloc(TALLOC_CTX *ctx, smb_ucs2_t **dest, const char *src,
		      size_t *converted_size)
{
	size_t src_len = strlen(src)+1;

	*dest = NULL;
	return convert_string_talloc(ctx, CH_UNIX, CH_UTF16LE, src, src_len,
				     (void **)dest, converted_size, True);
}


/**
 * Copy a string from a unix char* src to a UCS2 destination, allocating a buffer
 *
 * @param dest always set at least to NULL 
 * @parm converted_size set to the number of bytes occupied by the string in
 * the destination on success.
 *
 * @return true if new buffer was correctly allocated, and string was
 * converted.
 **/

bool push_ucs2_allocate(smb_ucs2_t **dest, const char *src,
			size_t *converted_size)
{
	size_t src_len = strlen(src)+1;

	*dest = NULL;
	return convert_string_allocate(NULL, CH_UNIX, CH_UTF16LE, src, src_len,
				       (void **)dest, converted_size, True);
}

/**
 Copy a string from a char* src to a UTF-8 destination.
 Return the number of bytes occupied by the string in the destination
 Flags can have:
  STR_TERMINATE means include the null termination
  STR_UPPER     means uppercase in the destination
 dest_len is the maximum length allowed in the destination. If dest_len
 is -1 then no maxiumum is used.
**/

static size_t push_utf8(void *dest, const char *src, size_t dest_len, int flags)
{
	size_t src_len = 0;
	size_t ret;
	char *tmpbuf = NULL;

	if (dest_len == (size_t)-1) {
		/* No longer allow dest_len of -1. */
		smb_panic("push_utf8 - invalid dest_len of -1");
	}

	if (flags & STR_UPPER) {
		tmpbuf = strdup_upper(src);
		if (!tmpbuf) {
			return (size_t)-1;
		}
		src = tmpbuf;
		src_len = strlen(src);
	}

	src_len = strlen(src);
	if (flags & STR_TERMINATE) {
		src_len++;
	}

	ret = convert_string(CH_UNIX, CH_UTF8, src, src_len, dest, dest_len, True);
	SAFE_FREE(tmpbuf);
	return ret;
}

size_t push_utf8_fstring(void *dest, const char *src)
{
	return push_utf8(dest, src, sizeof(fstring), STR_TERMINATE);
}

/**
 * Copy a string from a unix char* src to a UTF-8 destination, allocating a buffer using talloc
 *
 * @param dest always set at least to NULL 
 * @parm converted_size set to the number of bytes occupied by the string in
 * the destination on success.
 *
 * @return true if new buffer was correctly allocated, and string was
 * converted.
 **/

bool push_utf8_talloc(TALLOC_CTX *ctx, char **dest, const char *src,
		      size_t *converted_size)
{
	size_t src_len = strlen(src)+1;

	*dest = NULL;
	return convert_string_talloc(ctx, CH_UNIX, CH_UTF8, src, src_len,
				     (void**)dest, converted_size, True);
}

/**
 * Copy a string from a unix char* src to a UTF-8 destination, allocating a buffer
 *
 * @param dest always set at least to NULL 
 * @parm converted_size set to the number of bytes occupied by the string in
 * the destination on success.
 *
 * @return true if new buffer was correctly allocated, and string was
 * converted.
 **/

bool push_utf8_allocate(char **dest, const char *src, size_t *converted_size)
{
	size_t src_len = strlen(src)+1;

	*dest = NULL;
	return convert_string_allocate(NULL, CH_UNIX, CH_UTF8, src, src_len,
				       (void **)dest, converted_size, True);
}

/**
 Copy a string from a ucs2 source to a unix char* destination.
 Flags can have:
  STR_TERMINATE means the string in src is null terminated.
  STR_NOALIGN   means don't try to align.
 if STR_TERMINATE is set then src_len is ignored if it is -1.
 src_len is the length of the source area in bytes
 Return the number of bytes occupied by the string in src.
 The resulting string in "dest" is always null terminated.
**/

size_t pull_ucs2(const void *base_ptr, char *dest, const void *src, size_t dest_len, size_t src_len, int flags)
{
	size_t ret;

	if (dest_len == (size_t)-1) {
		/* No longer allow dest_len of -1. */
		smb_panic("pull_ucs2 - invalid dest_len of -1");
	}

	if (!src_len) {
		if (dest && dest_len > 0) {
			dest[0] = '\0';
		}
		return 0;
	}

	if (ucs2_align(base_ptr, src, flags)) {
		src = (const void *)((const char *)src + 1);
		if (src_len != (size_t)-1)
			src_len--;
	}

	if (flags & STR_TERMINATE) {
		/* src_len -1 is the default for null terminated strings. */
		if (src_len != (size_t)-1) {
			size_t len = strnlen_w((const smb_ucs2_t *)src,
						src_len/2);
			if (len < src_len/2)
				len++;
			src_len = len*2;
		}
	}

	/* ucs2 is always a multiple of 2 bytes */
	if (src_len != (size_t)-1)
		src_len &= ~1;

	ret = convert_string(CH_UTF16LE, CH_UNIX, src, src_len, dest, dest_len, True);
	if (ret == (size_t)-1) {
		ret = 0;
		dest_len = 0;
	}

	if (src_len == (size_t)-1)
		src_len = ret*2;

	if (dest_len && ret) {
		/* Did we already process the terminating zero ? */
		if (dest[MIN(ret-1, dest_len-1)] != 0) {
			dest[MIN(ret, dest_len-1)] = 0;
		}
	} else {
		dest[0] = 0;
	}

	return src_len;
}

/**
 Copy a string from a ucs2 source to a unix char* destination.
 Talloc version with a base pointer.
 Uses malloc if TALLOC_CTX is NULL (this is a bad interface and
 needs fixing. JRA).
 Flags can have:
  STR_TERMINATE means the string in src is null terminated.
  STR_NOALIGN   means don't try to align.
 if STR_TERMINATE is set then src_len is ignored if it is -1.
 src_len is the length of the source area in bytes
 Return the number of bytes occupied by the string in src.
 The resulting string in "dest" is always null terminated.
**/

size_t pull_ucs2_base_talloc(TALLOC_CTX *ctx,
			const void *base_ptr,
			char **ppdest,
			const void *src,
			size_t src_len,
			int flags)
{
	char *dest;
	size_t dest_len;

	*ppdest = NULL;

#ifdef DEVELOPER
	/* Ensure we never use the braindead "malloc" varient. */
	if (ctx == NULL) {
		smb_panic("NULL talloc CTX in pull_ucs2_base_talloc\n");
	}
#endif

	if (!src_len) {
		return 0;
	}

	if (ucs2_align(base_ptr, src, flags)) {
		src = (const void *)((const char *)src + 1);
		if (src_len != (size_t)-1)
			src_len--;
	}

	if (flags & STR_TERMINATE) {
		/* src_len -1 is the default for null terminated strings. */
		if (src_len != (size_t)-1) {
			size_t len = strnlen_w((const smb_ucs2_t *)src,
						src_len/2);
			if (len < src_len/2)
				len++;
			src_len = len*2;
		} else {
			/*
			 * src_len == -1 - alloc interface won't take this
			 * so we must calculate.
			 */
			src_len = (strlen_w((const smb_ucs2_t *)src)+1)*sizeof(smb_ucs2_t);
		}
		/* Ensure we don't use an insane length from the client. */
		if (src_len >= 1024*1024) {
			smb_panic("Bad src length in pull_ucs2_base_talloc\n");
		}
	} else {
		/* Can't have an unlimited length
		 * non STR_TERMINATE'd.
		 */
		if (src_len == (size_t)-1) {
			errno = EINVAL;
			return 0;
		}
	}

	/* src_len != -1 here. */

	/* ucs2 is always a multiple of 2 bytes */
	src_len &= ~1;

	if (!convert_string_talloc(ctx, CH_UTF16LE, CH_UNIX, src, src_len,
				   (void *)&dest, &dest_len, True)) {
		dest_len = 0;
	}

	if (dest_len) {
		/* Did we already process the terminating zero ? */
		if (dest[dest_len-1] != 0) {
			size_t size = talloc_get_size(dest);
			/* Have we got space to append the '\0' ? */
			if (size <= dest_len) {
				/* No, realloc. */
				dest = TALLOC_REALLOC_ARRAY(ctx, dest, char,
						dest_len+1);
				if (!dest) {
					/* talloc fail. */
					dest_len = (size_t)-1;
					return 0;
				}
			}
			/* Yay - space ! */
			dest[dest_len] = '\0';
			dest_len++;
		}
	} else if (dest) {
		dest[0] = 0;
	}

	*ppdest = dest;
	return src_len;
}

size_t pull_ucs2_fstring(char *dest, const void *src)
{
	return pull_ucs2(NULL, dest, src, sizeof(fstring), -1, STR_TERMINATE);
}

/**
 * Copy a string from a UCS2 src to a unix char * destination, allocating a buffer using talloc
 *
 * @param dest always set at least to NULL 
 * @parm converted_size set to the number of bytes occupied by the string in
 * the destination on success.
 *
 * @return true if new buffer was correctly allocated, and string was
 * converted.
 **/

bool pull_ucs2_talloc(TALLOC_CTX *ctx, char **dest, const smb_ucs2_t *src,
		      size_t *converted_size)
{
	size_t src_len = (strlen_w(src)+1) * sizeof(smb_ucs2_t);

	*dest = NULL;
	return convert_string_talloc(ctx, CH_UTF16LE, CH_UNIX, src, src_len,
				     (void **)dest, converted_size, True);
}

/**
 * Copy a string from a UCS2 src to a unix char * destination, allocating a buffer
 *
 * @param dest always set at least to NULL 
 * @parm converted_size set to the number of bytes occupied by the string in
 * the destination on success.
 * @return true if new buffer was correctly allocated, and string was
 * converted.
 **/

bool pull_ucs2_allocate(char **dest, const smb_ucs2_t *src,
			size_t *converted_size)
{
	size_t src_len = (strlen_w(src)+1) * sizeof(smb_ucs2_t);

	*dest = NULL;
	return convert_string_allocate(NULL, CH_UTF16LE, CH_UNIX, src, src_len,
				       (void **)dest, converted_size, True);
}

/**
 * Copy a string from a UTF-8 src to a unix char * destination, allocating a buffer using talloc
 *
 * @param dest always set at least to NULL 
 * @parm converted_size set to the number of bytes occupied by the string in
 * the destination on success.
 *
 * @return true if new buffer was correctly allocated, and string was
 * converted.
 **/

bool pull_utf8_talloc(TALLOC_CTX *ctx, char **dest, const char *src,
		      size_t *converted_size)
{
	size_t src_len = strlen(src)+1;

	*dest = NULL;
	return convert_string_talloc(ctx, CH_UTF8, CH_UNIX, src, src_len,
				     (void **)dest, converted_size, True);
}

/**
 * Copy a string from a UTF-8 src to a unix char * destination, allocating a buffer
 *
 * @param dest always set at least to NULL 
 * @parm converted_size set to the number of bytes occupied by the string in
 * the destination on success.
 *
 * @return true if new buffer was correctly allocated, and string was
 * converted.
 **/

bool pull_utf8_allocate(char **dest, const char *src, size_t *converted_size)
{
	size_t src_len = strlen(src)+1;

	*dest = NULL;
	return convert_string_allocate(NULL, CH_UTF8, CH_UNIX, src, src_len,
				       (void **)dest, converted_size, True);
}
 
/**
 * Copy a string from a DOS src to a unix char * destination, allocating a buffer using talloc
 *
 * @param dest always set at least to NULL 
 * @parm converted_size set to the number of bytes occupied by the string in
 * the destination on success.
 *
 * @return true if new buffer was correctly allocated, and string was
 * converted.
 **/

bool pull_ascii_talloc(TALLOC_CTX *ctx, char **dest, const char *src,
		       size_t *converted_size)
{
	size_t src_len = strlen(src)+1;

	*dest = NULL;
	return convert_string_talloc(ctx, CH_DOS, CH_UNIX, src, src_len,
				     (void **)dest, converted_size, True);
}

/**
 Copy a string from a char* src to a unicode or ascii
 dos codepage destination choosing unicode or ascii based on the 
 flags in the SMB buffer starting at base_ptr.
 Return the number of bytes occupied by the string in the destination.
 flags can have:
  STR_TERMINATE means include the null termination.
  STR_UPPER     means uppercase in the destination.
  STR_ASCII     use ascii even with unicode packet.
  STR_NOALIGN   means don't do alignment.
 dest_len is the maximum length allowed in the destination. If dest_len
 is -1 then no maxiumum is used.
**/

size_t push_string_fn(const char *function, unsigned int line,
		      const void *base_ptr, uint16 flags2,
		      void *dest, const char *src,
		      size_t dest_len, int flags)
{
#ifdef DEVELOPER
	/* We really need to zero fill here, not clobber
	 * region, as we want to ensure that valgrind thinks
	 * all of the outgoing buffer has been written to
	 * so a send() or write() won't trap an error.
	 * JRA.
	 */
#if 0
	clobber_region(function, line, dest, dest_len);
#else
	memset(dest, '\0', dest_len);
#endif
#endif

	if (!(flags & STR_ASCII) && \
	    ((flags & STR_UNICODE || \
	      (flags2 & FLAGS2_UNICODE_STRINGS)))) {
		return push_ucs2(base_ptr, dest, src, dest_len, flags);
	}
	return push_ascii(dest, src, dest_len, flags);
}


/**
 Copy a string from a unicode or ascii source (depending on
 the packet flags) to a char* destination.
 Flags can have:
  STR_TERMINATE means the string in src is null terminated.
  STR_UNICODE   means to force as unicode.
  STR_ASCII     use ascii even with unicode packet.
  STR_NOALIGN   means don't do alignment.
 if STR_TERMINATE is set then src_len is ignored is it is -1
 src_len is the length of the source area in bytes.
 Return the number of bytes occupied by the string in src.
 The resulting string in "dest" is always null terminated.
**/

size_t pull_string_fn(const char *function,
			unsigned int line,
			const void *base_ptr,
			uint16 smb_flags2,
			char *dest,
			const void *src,
			size_t dest_len,
			size_t src_len,
			int flags)
{
#ifdef DEVELOPER
	clobber_region(function, line, dest, dest_len);
#endif

	if ((base_ptr == NULL) && ((flags & (STR_ASCII|STR_UNICODE)) == 0)) {
		smb_panic("No base ptr to get flg2 and neither ASCII nor "
			  "UNICODE defined");
	}

	if (!(flags & STR_ASCII) && \
	    ((flags & STR_UNICODE || \
	      (smb_flags2 & FLAGS2_UNICODE_STRINGS)))) {
		return pull_ucs2(base_ptr, dest, src, dest_len, src_len, flags);
	}
	return pull_ascii(dest, src, dest_len, src_len, flags);
}

/**
 Copy a string from a unicode or ascii source (depending on
 the packet flags) to a char* destination.
 Variant that uses talloc.
 Flags can have:
  STR_TERMINATE means the string in src is null terminated.
  STR_UNICODE   means to force as unicode.
  STR_ASCII     use ascii even with unicode packet.
  STR_NOALIGN   means don't do alignment.
 if STR_TERMINATE is set then src_len is ignored is it is -1
 src_len is the length of the source area in bytes.
 Return the number of bytes occupied by the string in src.
 The resulting string in "dest" is always null terminated.
**/

size_t pull_string_talloc_fn(const char *function,
			unsigned int line,
			TALLOC_CTX *ctx,
			const void *base_ptr,
			uint16 smb_flags2,
			char **ppdest,
			const void *src,
			size_t src_len,
			int flags)
{
	if ((base_ptr == NULL) && ((flags & (STR_ASCII|STR_UNICODE)) == 0)) {
		smb_panic("No base ptr to get flg2 and neither ASCII nor "
			  "UNICODE defined");
	}

	if (!(flags & STR_ASCII) && \
	    ((flags & STR_UNICODE || \
	      (smb_flags2 & FLAGS2_UNICODE_STRINGS)))) {
		return pull_ucs2_base_talloc(ctx,
					base_ptr,
					ppdest,
					src,
					src_len,
					flags);
	}
	return pull_ascii_base_talloc(ctx,
					ppdest,
					src,
					src_len,
					flags);
}


size_t align_string(const void *base_ptr, const char *p, int flags)
{
	if (!(flags & STR_ASCII) && \
	    ((flags & STR_UNICODE || \
	      (SVAL(base_ptr, smb_flg2) & FLAGS2_UNICODE_STRINGS)))) {
		return ucs2_align(base_ptr, p, flags);
	}
	return 0;
}

/*
  Return the unicode codepoint for the next multi-byte CH_UNIX character
  in the string. The unicode codepoint (codepoint_t) is an unsinged 32 bit value.

  Also return the number of bytes consumed (which tells the caller
  how many bytes to skip to get to the next CH_UNIX character).

  Return INVALID_CODEPOINT if the next character cannot be converted.
*/

codepoint_t next_codepoint(const char *str, size_t *size)
{
	/* It cannot occupy more than 4 bytes in UTF16 format */
	uint8_t buf[4];
	smb_iconv_t descriptor;
	size_t ilen_orig;
	size_t ilen;
	size_t olen;
	char *outbuf;

	if ((str[0] & 0x80) == 0) {
		*size = 1;
		return (codepoint_t)str[0];
	}

	/* We assume that no multi-byte character can take
	   more than 5 bytes. This is OK as we only
	   support codepoints up to 1M */

	ilen_orig = strnlen(str, 5);
	ilen = ilen_orig;

        lazy_initialize_conv();

        descriptor = conv_handles[CH_UNIX][CH_UTF16LE];
	if (descriptor == (smb_iconv_t)-1 || descriptor == (smb_iconv_t)0) {
		*size = 1;
		return INVALID_CODEPOINT;
	}

	/* This looks a little strange, but it is needed to cope
	   with codepoints above 64k which are encoded as per RFC2781. */
	olen = 2;
	outbuf = (char *)buf;
	smb_iconv(descriptor, &str, &ilen, &outbuf, &olen);
	if (olen == 2) {
		/* We failed to convert to a 2 byte character.
		   See if we can convert to a 4 UTF16-LE byte char encoding.
		*/
		olen = 4;
		outbuf = (char *)buf;
		smb_iconv(descriptor,  &str, &ilen, &outbuf, &olen);
		if (olen == 4) {
			/* We didn't convert any bytes */
			*size = 1;
			return INVALID_CODEPOINT;
		}
		olen = 4 - olen;
	} else {
		olen = 2 - olen;
	}

	*size = ilen_orig - ilen;

	if (olen == 2) {
		/* 2 byte, UTF16-LE encoded value. */
		return (codepoint_t)SVAL(buf, 0);
	}
	if (olen == 4) {
		/* Decode a 4 byte UTF16-LE character manually.
		   See RFC2871 for the encoding machanism.
		*/
		codepoint_t w1 = SVAL(buf,0) & ~0xD800;
		codepoint_t w2 = SVAL(buf,2) & ~0xDC00;

		return (codepoint_t)0x10000 +
				(w1 << 10) + w2;
	}

	/* no other length is valid */
	return INVALID_CODEPOINT;
}
