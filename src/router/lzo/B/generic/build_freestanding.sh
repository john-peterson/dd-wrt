#! /bin/sh
# vi:ts=4:et

test "X${top_srcdir}" = X && top_srcdir=`echo "$0" | sed 's,[^/]*$,,'`../..

LZO_CFG_FREESTANDING=1

. $top_srcdir/B/generic/build.sh
