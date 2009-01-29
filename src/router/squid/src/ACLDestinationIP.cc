/*
 * $Id$
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
 * Copyright (c) 2003, Robert Collins <robertc@squid-cache.org>
 */

#include "squid.h"
#include "ACLDestinationIP.h"
#include "ACLChecklist.h"
#include "HttpRequest.h"

char const *
ACLDestinationIP::typeString() const
{
    return "dst";
}

int
ACLDestinationIP::match(ACLChecklist *checklist)
{
    const ipcache_addrs *ia = ipcache_gethostbyname(checklist->request->host, IP_LOOKUP_IF_MISS);

    if (ia) {
        /* Entry in cache found */

        for (int k = 0; k < (int) ia->count; k++) {
            if (ACLIP::match(ia->in_addrs[k]))
                return 1;
        }

        return 0;
    } else if (!checklist->request->flags.destinationIPLookedUp()) {
        /* No entry in cache, lookup not attempted */
        debugs(28, 3, "aclMatchAcl: Can't yet compare '" << name << "' ACL for '" << checklist->request->host << "'");
        checklist->changeState (DestinationIPLookup::Instance());
        return 0;
    } else {
        return 0;
    }
}

DestinationIPLookup DestinationIPLookup::instance_;

DestinationIPLookup *
DestinationIPLookup::Instance()
{
    return &instance_;
}

void
DestinationIPLookup::checkForAsync(ACLChecklist *checklist)const
{
    checklist->asyncInProgress(true);
    ipcache_nbgethostbyname(checklist->request->host, LookupDone, checklist);
}

void
DestinationIPLookup::LookupDone(const ipcache_addrs * ia, void *data)
{
    ACLChecklist *checklist = (ACLChecklist *)data;
    assert (checklist->asyncState() == DestinationIPLookup::Instance());
    checklist->request->flags.destinationIPLookupCompleted();
    checklist->asyncInProgress(false);
    checklist->changeState (ACLChecklist::NullState::Instance());
    checklist->check();
}

ACL::Prototype ACLDestinationIP::RegistryProtoype(&ACLDestinationIP::RegistryEntry_, "dst");

ACLDestinationIP ACLDestinationIP::RegistryEntry_;

ACL *
ACLDestinationIP::clone() const
{
    return new ACLDestinationIP(*this);
}
