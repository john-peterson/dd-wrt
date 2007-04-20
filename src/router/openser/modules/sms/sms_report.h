/*
 * $Id: sms_report.h,v 1.1.1.1 2005/06/13 16:47:45 bogdan_iancu Exp $
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
 */

#ifndef _H_SMS_REPORT_DEF
#define _H_SMS_REPORT_DEF

#include "../../str.h"
#include "sms_funcs.h"

#define NR_CELLS  256


int    init_report_queue();
void   destroy_report_queue();
void   add_sms_into_report_queue(int id, struct sms_msg *sms, char *, int );
int    relay_report_to_queue(int id, char *phone, int status, int *old_status);
void   check_timeout_in_report_queue();
str*   get_error_str(int status);
void   remove_sms_from_report_queue(int id);
str*   get_text_from_report_queue(int id);
struct sms_msg* get_sms_from_report_queue(int id);
void   set_gettime_function();


#endif
