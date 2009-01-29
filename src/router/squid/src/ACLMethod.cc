
/*
 * $Id: ACLMethod.cc,v 1.3 2003/10/20 12:33:01 robertc Exp $
 *
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
#include "ACLMethod.h"
#include "ACLMethodData.h"
#include "ACLChecklist.h"
#include "HttpRequest.h"

/* explicit template instantiation required for some systems */

template class ACLStrategised<method_t>

;

ACL::Prototype ACLMethod::RegistryProtoype(&ACLMethod::RegistryEntry_, "method");

ACLStrategised<method_t> ACLMethod::RegistryEntry_(new ACLMethodData, ACLMethodStrategy::Instance(), "method");

int
ACLMethodStrategy::match (ACLData<MatchType> * &data, ACLChecklist *checklist)
{
    return data->match (checklist->request->method);
}

ACLMethodStrategy *
ACLMethodStrategy::Instance()
{
    return &Instance_;
}

ACLMethodStrategy ACLMethodStrategy::Instance_;
