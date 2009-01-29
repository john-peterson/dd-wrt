/*
 * $Id: ACLUrlPath.cc,v 1.4 2007/05/29 13:31:36 amosjeffries Exp $
 *
 * DEBUG: section 28    Access Control
 * AUTHOR: Duane Wessels
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
 *
 * Copyright (c) 2003, Robert Collins <robertc@squid-cache.org>
 */

#include "squid.h"
#include "ACLUrlPath.h"
#include "ACLChecklist.h"
#include "ACLRegexData.h"
#include "HttpRequest.h"

ACL::Prototype ACLUrlPath::LegacyRegistryProtoype(&ACLUrlPath::RegistryEntry_, "pattern");
ACL::Prototype ACLUrlPath::RegistryProtoype(&ACLUrlPath::RegistryEntry_, "urlpath_regex");
ACLStrategised<char const *> ACLUrlPath::RegistryEntry_(new ACLRegexData, ACLUrlPathStrategy::Instance(), "urlpath_regex");

int
ACLUrlPathStrategy::match (ACLData<char const *> * &data, ACLChecklist *checklist)
{
    char *esc_buf = xstrdup(checklist->request->urlpath.buf());
    rfc1738_unescape(esc_buf);
    int result = data->match(esc_buf);
    safe_free(esc_buf);
    return result;
}

ACLUrlPathStrategy *
ACLUrlPathStrategy::Instance()
{
    return &Instance_;
}

ACLUrlPathStrategy ACLUrlPathStrategy::Instance_;
