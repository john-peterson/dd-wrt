%{

/*
 * The olsr.org Optimized Link-State Routing daemon(olsrd)
 * Copyright (c) 2004, Andreas T�nnesen(andreto@olsr.org)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met:
 *
 * * Redistributions of source code must retain the above copyright 
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright 
 *   notice, this list of conditions and the following disclaimer in 
 *   the documentation and/or other materials provided with the 
 *   distribution.
 * * Neither the name of olsr.org, olsrd nor the names of its 
 *   contributors may be used to endorse or promote products derived 
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Visit http://www.olsr.org for more information.
 *
 * If you find this software useful feel free to make a donation
 * to the project. For more information see the website or contact
 * the copyright holders.
 *
 * $Id: oscan.lex,v 1.24 2007/05/17 20:35:16 bernd67 Exp $
 */


#define YYSTYPE struct conf_token *

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "olsrd_conf.h"

#include "oparse.h"
 
/* Prototypes */
int yyget_lineno(void);
FILE * yyget_in(void);
FILE* yyget_out(void);
int yyget_leng(void);
char *yyget_text(void);
void yyset_lineno(int);
void yyset_in(FILE *);
void yyset_out(FILE *);
int yyget_debug(void);
void yyset_debug(int);
int yylex_destroy(void);
int yylex(void);

static struct conf_token *get_conf_token(void);
static struct conf_token *get_string_token(const char * const s, int const n);
static struct conf_token *get_integer_token(const char * const s);
static struct conf_token *get_floating_token(const char * const s);
static struct conf_token *get_boolean_token(const olsr_bool b);

static struct conf_token *get_conf_token(void)
{
    struct conf_token *t = calloc(1, sizeof(struct conf_token));
    if (t == NULL) {
        fprintf(stderr, "Cannot allocate %d bytes for an configuration token.\n", (int)sizeof(struct conf_token));
    }
    return t;
}

static struct conf_token *get_string_token(const char * const s, int const n)
{
    struct conf_token *rv = get_conf_token();
    if (rv != NULL) {
        rv->string = malloc(n+1);
        if (rv->string == NULL) {
            fprintf(stderr, "Cannot allocate %d bytes for string token data.\n", n+1);
            free(rv);
            return NULL;
        }
        strncpy(rv->string, s, n);
        rv->string[n] = '\0';
    }
    return rv;
}

static struct conf_token *get_integer_token(const char * const s)
{
    struct conf_token *rv = get_conf_token();
    if (rv != NULL) {
        rv->integer = strtol(s, NULL, 0);
    }
    return rv;
}

static struct conf_token *get_floating_token(const char * const s)
{
    struct conf_token *rv = get_conf_token();
    if (rv != NULL) {
        rv->floating = strtod(s, NULL);
    }
    return rv;
}

static struct conf_token *get_boolean_token(const olsr_bool b)
{
    struct conf_token *rv = get_conf_token();
    if (rv != NULL) {
        rv->boolean = b;
    }
    return rv;
}

%}

%option noyywrap

DECDIGIT [0-9]
FLOAT {DECDIGIT}+\.{DECDIGIT}+
HEXDIGIT [a-f][A-F][0-9]

IPV4ADDR ({DECDIGIT}){1,3}\.({DECDIGIT}){1,3}\.({DECDIGIT}){1,3}\.({DECDIGIT}){1,3}

HEXBYTE ([a-f]|[A-F]|[0-9]){1,4}

IP6PAT1 ({HEXBYTE}:){7}{HEXBYTE}
IP6PAT2 {HEXBYTE}::({HEXBYTE}:){0,5}{HEXBYTE}
IP6PAT3 ({HEXBYTE}:){2}:({HEXBYTE}:){0,4}{HEXBYTE}
IP6PAT4 ({HEXBYTE}:){3}:({HEXBYTE}:){0,3}{HEXBYTE}
IP6PAT5 ({HEXBYTE}:){4}:({HEXBYTE}:){0,2}{HEXBYTE}
IP6PAT6 ({HEXBYTE}:){5}:({HEXBYTE}:){0,1}{HEXBYTE}
IP6PAT7 ({HEXBYTE}:){6}:{HEXBYTE}
IP6PAT8 ({HEXBYTE}:){1,7}:
IP6PAT9 ::

IPV6ADDR {IP6PAT1}|{IP6PAT2}|{IP6PAT3}|{IP6PAT4}|{IP6PAT5}|{IP6PAT6}|{IP6PAT7}|{IP6PAT8}|{IP6PAT9}

%%

\s*"#".*\n {
    current_line++;
    return TOK_COMMENT;
}

\{ {
    yylval = NULL;
    return TOK_OPEN;
}

\} {
    yylval = NULL;
    return TOK_CLOSE;
}

\"[^\"]*\" {
    yylval = get_string_token(yytext + 1, yyleng - 2);
    if (yylval == NULL) {
        yyterminate();
    }
    return TOK_STRING;
}

0x{HEXDIGIT}+ {
    yylval = get_integer_token(yytext);
    return TOK_INTEGER;
}

{FLOAT} {
    yylval = get_floating_token(yytext);
    return TOK_FLOAT;
}

{IPV4ADDR} {
    yylval = get_string_token(yytext, yyleng + 1);
    if (yylval == NULL) {
        yyterminate();
    }
    return TOK_IP4_ADDR;
}

{IPV6ADDR} {
    yylval = get_string_token(yytext, yyleng + 1);
    if (yylval == NULL) {
        yyterminate();
    }
    return TOK_IP6_ADDR;
}

"default" {
    yylval = NULL;
    return TOK_DEFAULT;
}

{DECDIGIT}+ {
    yylval = get_integer_token(yytext);
    return TOK_INTEGER;
}


"yes" {
    yylval = get_boolean_token(OLSR_TRUE);
    return TOK_BOOLEAN;
}

"no" {
    yylval = get_boolean_token(OLSR_FALSE);
    return TOK_BOOLEAN;
}

"site-local" {
    yylval = get_boolean_token(OLSR_TRUE);
    return TOK_IP6TYPE;
}

"global" {
    yylval = get_boolean_token(OLSR_FALSE);
    return TOK_IP6TYPE;
}

"Host" {
    yylval = NULL;
    return TOK_HOSTLABEL;
}

"Net" {
    yylval = NULL;
    return TOK_NETLABEL;
}

"MaxConnections" {
    yylval = NULL;
    return TOK_MAXIPC;
}

"DebugLevel" {
    yylval = NULL;
    return TOK_DEBUGLEVEL;
}

"IpVersion" {
    yylval = NULL;
    return TOK_IPVERSION;
}

"NicChgsPollInt" {
    yylval = NULL;
    return TOK_NICCHGSPOLLRT;
}

"Hna4" {
    yylval = NULL;
    return TOK_HNA4;
}

"Hna6" {
    yylval = NULL;
    return TOK_HNA6;
}

"LoadPlugin" {
    yylval = NULL;
    return TOK_PLUGIN;
}

"PlParam" {
    yylval = NULL;
    return TOK_PLPARAM;
}

"Interface" {
    yylval = NULL;
    return TOK_INTERFACE;
}

"AllowNoInt" {
    yylval = NULL;
    return TOK_NOINT;
}

"TosValue" {
    yylval = NULL;
    return TOK_TOS;
}

"Willingness" {
    yylval = NULL;
    return TOK_WILLINGNESS;
}

"IpcConnect" {
    yylval = NULL;
    return TOK_IPCCON;
}

"UseHysteresis" {
    yylval = NULL;
    return TOK_USEHYST;
}

"HystScaling" {
    yylval = NULL;
    return TOK_HYSTSCALE;
}

"HystThrHigh" {
    yylval = NULL;
    return TOK_HYSTUPPER;
}

"HystThrLow" {
    yylval = NULL;
    return TOK_HYSTLOWER;
}

"Pollrate" {
    yylval = NULL;
    return TOK_POLLRATE;
}


"TcRedundancy" {
    yylval = NULL;
    return TOK_TCREDUNDANCY;
}

"MprCoverage" {
    yylval = NULL;
    return TOK_MPRCOVERAGE;
}

"LinkQualityLevel" {
    yylval = NULL;
    return TOK_LQ_LEVEL;
}

"LinkQualityFishEye" {
    yylval = NULL;
    return TOK_LQ_FISH;
}

"LinkQualityDijkstraLimit" {
    yylval = NULL;
    return TOK_LQ_DLIMIT;
}

"LinkQualityWinSize" {
    yylval = NULL;
    return TOK_LQ_WSIZE;
}

"LinkQualityMult" {
    yylval = NULL;
    return TOK_LQ_MULT;
}

"ClearScreen" {
    yylval = NULL;
    return TOK_CLEAR_SCREEN;
}

"Weight" {
    yylval = NULL;
    return TOK_IFWEIGHT;
}

"Ip4Broadcast" {
    yylval = NULL;
    return TOK_IP4BROADCAST;
}
"Ip6AddrType" {
    yylval = NULL;
    return TOK_IP6ADDRTYPE;
}
"Ip6MulticastSite" {
    yylval = NULL;
    return TOK_IP6MULTISITE;
}
"Ip6MulticastGlobal" {
    yylval = NULL;
    return TOK_IP6MULTIGLOBAL;
}
"HelloInterval" {
    yylval = NULL;
    return TOK_HELLOINT;
}
"HelloValidityTime" {
    yylval = NULL;
    return TOK_HELLOVAL;
}
"TcInterval" {
    yylval = NULL;
    return TOK_TCINT;
}
"TcValidityTime" {
    yylval = NULL;
    return TOK_TCVAL;
}
"MidInterval" {
    yylval = NULL;
    return TOK_MIDINT;
}
"MidValidityTime" {
    yylval = NULL;
    return TOK_MIDVAL;
}
"HnaInterval" {
    yylval = NULL;
    return TOK_HNAINT;
}
"HnaValidityTime" {
    yylval = NULL;
    return TOK_HNAVAL;
}
"AutoDetectChanges" {
    yylval = NULL;
    return TOK_AUTODETCHG;
}


\n|\r\n {
    current_line++;
}

\ |\t

. {
  /* Do nothing */
  //fprintf(stderr, "Failed to parse line %d of configuration file.\n",
  //      current_line);
  //yyterminate();
  //yy_fatal_error("Parsing failed.\n");

  /* To avoid compiler warning (stupid...) */
  if(0)
    yyunput(0, NULL);
}

%%
