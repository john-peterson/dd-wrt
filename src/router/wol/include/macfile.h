/*
 *	wol - wake on lan client
 *
 *	$Id: macfile.h,v 1.1.2.1 2004/06/20 21:55:17 nikki Exp $
 *
 *	Copyright (C) 2000-2002 Thomas Krennwallner <krennwallner@aon.at>
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 *	USA.
 */


#ifndef _MACFILE_H
#define _MACFILE_H

#include <stdio.h>

int macfile_parse (FILE *fp, char **mac_str, char **host, unsigned int *port,
										char **passwd);

#endif /* _MACFILE_H */
