#!/bin/sh

# Tests for lp_load() via testparm.
#
# The main purpose (for now) is to test all the special handlers
# and the macro expansions.

TEMP_CONFFILE=${LIBDIR}/smb.conf.tmp
TESTPARM="$VALGRIND ${TESTPARM:-$BINDIR/testparm} --suppress-prompt --skip-logic-checks"

incdir=`dirname $0`
. $incdir/test_functions.sh

failed=0

test_include_expand_macro()
{
	MACRO=$1
	rm -f ${TEMP_CONFFILE}
	cat >${TEMP_CONFFILE}<<EOF
[global]
	include = ${TEMP_CONFFILE}.%${MACRO}
EOF
	${TESTPARM} ${TEMP_CONFFILE}
}

test_one_global_option()
{
	OPTION=$1
	rm -f ${TEMP_CONFFILE}
	cat > ${TEMP_CONFFILE}<<EOF
[global]
	${OPTION}
EOF
	${TESTPARM} ${TEMP_CONFFILE}
}

test_copy()
{
	rm -f ${TEMP_CONFFILE}
	cat > ${TEMP_CONFFILE}<<EOF
[share1]
	path = /tmp
	read only = no

[share2]
	copy = share1
EOF
	${TESTPARM} ${TEMP_CONFFILE}
}


testit "netbios name" \
	test_one_global_option "netbios name = funky" || \
	failed=`expr ${failed} + 1`

testit "netbios aliases" \
	test_one_global_option "netbios aliases = funky1 funky2 funky3" || \
	failed=`expr ${failed} + 1`

testit "netbios scope" \
	test_one_global_option "netbios scope = abc" || \
	failed=`expr ${failed} + 1`

testit "workgroup" \
	test_one_global_option "workgroup = samba" || \
	failed=`expr ${failed} + 1`

testit "display charset" \
	test_one_global_option "display charset = UTF8" || \
	failed=`expr ${failed} + 1`

testit "ldap debug level" \
	test_one_global_option "ldap debug level = 7" || \
	failed=`expr ${failed} + 1`

for LETTER in U G D I i L N M R T a d h m v w V ; do
testit "include with %${LETTER} macro expansion" \
	test_include_expand_macro "${LETTER}" || \
	failed=`expr ${failed} + 1`
done

testit "copy" \
	test_copy || \
	failed=`expr ${failed} + 1`

rm -f ${TEMP_CONFFILE}

testok $0 ${failed}

