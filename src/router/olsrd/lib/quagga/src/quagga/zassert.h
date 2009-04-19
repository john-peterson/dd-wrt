
/*
 */

#ifndef _QUAGGA_ASSERT_H
#define _QUAGGA_ASSERT_H

extern void _zlog_assert_failed(const char *assertion, const char *file, unsigned int line, const char *function)
  __attribute__ ((noreturn));

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#define __ASSERT_FUNCTION    __func__
#elif defined(__GNUC__)
#define __ASSERT_FUNCTION    __FUNCTION__
#else
#define __ASSERT_FUNCTION    NULL
#endif

#define zassert(EX) ((void)((EX) ?  0 :	\
			    (_zlog_assert_failed(#EX, __FILE__, __LINE__, \
						 __ASSERT_FUNCTION), 0)))

#undef assert
#define assert(EX) zassert(EX)

#endif /* _QUAGGA_ASSERT_H */

/*
 * Local Variables:
 * c-basic-offset: 2
 * indent-tabs-mode: nil
 * End:
 */
