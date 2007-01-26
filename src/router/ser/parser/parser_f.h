/* 
 * $Id: parser_f.h,v 1.2 2005/06/16 11:37:54 miconda Exp $
 *
 * Copyright (C) 2001-2003 FhG Fokus
 *
 * This file is part of openser, a free SIP server.
 *
 * openser is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version
 *
 * openser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * History
 * --------
 * 2003-02-28 scratchpad compatibility abandoned (jiri)
 * 2003-03-24 find_not_quoted function added (janakj)
 */


#ifndef parser_f_h
#define parser_f_h

#include "../str.h"

char* eat_line(char* buffer, unsigned int len);

/* turn the most frequently called functions into inline functions */

inline static char* eat_space_end(const char* p, const char* pend)
{
	for(;(p<pend)&&(*p==' ' || *p=='\t') ;p++);
	return (char *)p;
}
#define SP(_c) ((_c)=='\t' || (_c)==' ')
inline static char* eat_lws_end(const char* p, const char* pend)
{
	while(p<pend) {
		if (SP(*p)) p++;
		/* BTW--I really dislike line folding; -jiri */
		else if (*p=='\n' && p+1<pend && SP(*(p+1))) p+=2;
		else if (*p=='\r' && p+2<pend && *(p+1)=='\n'
					&& SP(*(p+2))) p+=3;
		else break; /* no whitespace encountered */
	}
	return (char *)p;
}



inline static char* eat_token_end(const char* p, const char* pend)
{
	for (;(p<pend)&&(*p!=' ')&&(*p!='\t')&&(*p!='\n')&&(*p!='\r'); p++);
	return (char *)p;
}



inline static char* eat_token2_end(const char* p, const char* pend, char delim)
{
	for (;(p<pend)&&(*p!=(delim))&&(*p!='\n')&&(*p!='\r'); p++);
	return (char *)p;
}



inline static int is_empty_end(const char* p, const char* pend )
{
	p=eat_space_end(p, pend );
	return ((p<(pend )) && (*p=='\r' || *p=='\n'));
}


/*
 * Find a character occurrence that is not quoted
 */
inline static char* find_not_quoted(str* _s, char _c)
{
	int quoted = 0, i;
	
	for(i = 0; i < _s->len; i++) {
		if (!quoted) {
			if (_s->s[i] == '\"') quoted = 1;
			else if (_s->s[i] == _c) return _s->s + i;
		} else {
			if ((_s->s[i] == '\"') && (_s->s[i - 1] != '\\')) quoted = 0;
		}
	}
	return 0;
}


#endif /* parser_f_h */
