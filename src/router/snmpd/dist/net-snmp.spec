#
# 5.4+ enables Perl by default
#
%define netsnmp_embedded_perl 1
%define netsnmp_perl_modules 1
#
# Check for -without embedded_perl
#
%{?_without_embedded_perl:%define netsnmp_embedded_perl 0}
#
# check for -without perl_modules
#
%{?_without_perl_modules:%define netsnmp_perl_modules 0}
#
# if embedded_perl or perl_modules specified, include some Perl stuff
#
%if 0%{?netsnmp_embedded_perl} || 0%{?netsnmp_perl_modules}
%define netsnmp_include_perl 1
%endif
#
# library version (see Makefile.top)
#
%define libcurrent 15
#
Summary: Tools and servers for the SNMP protocol
Name: net-snmp
Version: 5.4
# update release for vendor release. (eg 1.rh9, 1.rh72, 1.ydl3, 1.ydl23)
Release: 1
URL: http://www.net-snmp.org/
License: BSDish
Group: System Environment/Daemons
Vendor: Net-SNMP project
Source: http://prdownloads.sourceforge.net/net-snmp/net-snmp-%{version}.tar.gz
Prereq: openssl
Obsoletes: cmu-snmp ucd-snmp ucd-snmp-utils
BuildRoot: /tmp/%{name}-root
Packager: The Net-SNMP Coders <http://sourceforge.net/projects/net-snmp/>
BuildRequires: perl, beecrypt-devel elfutils-libelf-devel
# because perl(Tk) is optional, automatic dependencies will never succeed:
AutoReqProv: no
Requires: openssl, popt, rpm, zlib, bzip2-libs, beecrypt, elfutils-libelf, glibc
Provides: net-snmp, net-snmp-utils, libnetsnmp.so.%{libcurrent}, libnetsnmpagent.so.%{libcurrent}, libnetsnmphelpers.so.%{libcurrent}, libnetsnmpmibs.so.%{libcurrent}, libnetsnmptrapd.so.%{libcurrent}
%if %{netsnmp_embedded_perl}
Requires: perl
%endif

%description

Net-SNMP provides tools and libraries relating to the Simple Network
Management Protocol including: An extensible agent, An SNMP library,
tools to request or set information from SNMP agents, tools to
generate and handle SNMP traps, etc.  Using SNMP you can check the
status of a network of computers, routers, switches, servers, ... to
evaluate the state of your network.

%if %{netsnmp_embedded_perl}
This package includes embedded Perl support within the agent.
%endif

%package devel
Group: Development/Libraries
Summary: The includes and static libraries from the Net-SNMP package.
AutoReqProv: no
Requires: net-snmp = %{version}
Obsoletes: cmu-snmp-devel ucd-snmp-devel

%description devel
The net-snmp-devel package contains headers and libraries which are
useful for building SNMP applications, agents, and sub-agents.

%if %{netsnmp_include_perl}
%package perlmods
Group: System Environment/Libraries
Summary: The Perl modules provided with Net-SNMP
AutoReqProv: no
Provides: ASN.so, OID.so, SNMP.so, TrapReceiver.so, agent.so, default_store.so, perl(NetSNMP::ASN), perl(NetSNMP::OID), perl(NetSNMP::TrapReceiver), perl(NetSNMP::agent), perl(NetSNMP::agent::default_store), perl(NetSNMP::agent::netsnmp_request_infoPtr), perl(NetSNMP::agent::Support) perl(NetSNMP::default_store), perl(SNMP), perl(SNMP::DEBUGGING), perl(SNMP::DEBUG_INTERNALS), perl(SNMP::DUMP_PACKET), perl(SNMP::MIB), perl(SNMP::MIB::MIB_OPTIONS), perl(SNMP::MIB::NODE), perl(SNMP::MIB::REPLACE_NEWER), perl(SNMP::MIB::SAVE_DESCR), perl(SNMP::Session), perl(SNMP::TrapSession), perl(SNMP::VarList), perl(SNMP::Varbind), net-snmp-perlmods
Requires: net-snmp = %{version}, perl

%description perlmods
Net-SNMP provides a number of Perl modules useful when using the SNMP
protocol.  Both client and agent support modules are provided.
%endif

%prep
%if %{netsnmp_embedded_perl} == 1 && %{netsnmp_perl_modules} == 0
echo "'-with embedded_perl' requires '-with perl_modules'"
exit 1
%endif
%setup -q

%build
%configure --with-defaults --with-sys-contact="Unknown" \
	--with-mib-modules="smux" \
	--with-sysconfdir="/etc/net-snmp"               \
	--enable-shared \
	%{?netsnmp_perl_modules: --with-perl-modules="PREFIX=$RPM_BUILD_ROOT%{_prefix} INSTALLDIRS=vendor"} \
	%{!?netsnmp_perl_modules: --without-perl-modules} \
	%{?netsnmp_embedded_perl: --enable-embedded-perl} \
	%{!?netsnmp_embedded_perl: --disable-embedded-perl} \
	--with-cflags="$RPM_OPT_FLAGS"

make

%install
# ----------------------------------------------------------------------
# 'install' sets the current directory to _topdir/BUILD/{name}-{version}
# ----------------------------------------------------------------------
rm -rf $RPM_BUILD_ROOT

%makeinstall ucdincludedir=$RPM_BUILD_ROOT%{_prefix}/include/ucd-snmp \
             includedir=$RPM_BUILD_ROOT%{_prefix}/include/net-snmp

# Remove 'snmpinform' from the temporary directory because it is a
# symbolic link, which cannot be handled by the rpm installation process.
%__rm -f $RPM_BUILD_ROOT%{_prefix}/bin/snmpinform
# install the init script
mkdir -p $RPM_BUILD_ROOT/etc/rc.d/init.d
perl -i -p -e 's@/usr/local/share/snmp/@/etc/snmp/@g;s@usr/local@%{_prefix}@g' dist/snmpd-init.d
install -m 755 dist/snmpd-init.d $RPM_BUILD_ROOT/etc/rc.d/init.d/snmpd

%if %{netsnmp_include_perl}
# unneeded Perl stuff
find $RPM_BUILD_ROOT/%{_libdir}/perl5/ -name Bundle -type d | xargs rm -rf
find $RPM_BUILD_ROOT/%{_libdir}/perl5/ -name perllocal.pod | xargs rm -f

# store a copy of installed Perl stuff.  It's too complex to predict
(xxdir=`pwd` && cd $RPM_BUILD_ROOT && find usr/lib*/perl5 -type f | sed 's/^/\//' > $xxdir/net-snmp-perl-files)
%endif

%post
# ----------------------------------------------------------------------
# The 'post' script is executed just after the package is installed.
# ----------------------------------------------------------------------
# Create the symbolic link 'snmpinform' after all other files have
# been installed.
%__rm -f $RPM_INSTALL_PREFIX/bin/snmpinform
%__ln_s $RPM_INSTALL_PREFIX/bin/snmptrap $RPM_INSTALL_PREFIX/bin/snmpinform

# run ldconfig
PATH="$PATH:/sbin" ldconfig -n $RPM_INSTALL_PREFIX/lib

%preun
# ----------------------------------------------------------------------
# The 'preun' script is executed just before the package is erased.
# ----------------------------------------------------------------------
# Remove the symbolic link 'snmpinform' before anything else, in case
# it is in a directory that rpm wants to remove (at present, it isn't).
%__rm -f $RPM_INSTALL_PREFIX/bin/snmpinform

%postun
# ----------------------------------------------------------------------
# The 'postun' script is executed just after the package is erased.
# ----------------------------------------------------------------------
PATH="$PATH:/sbin" ldconfig -n $RPM_INSTALL_PREFIX/lib

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)

# Install the following documentation in _defaultdocdir/{name}-{version}/
%doc AGENT.txt ChangeLog CodingStyle COPYING
%doc EXAMPLE.conf.def FAQ INSTALL NEWS PORTING TODO
%doc README README.agentx README.hpux11 README.krb5
%doc README.snmpv3 README.solaris README.thread README.win32
%doc README.aix README.osX README.tru64 README.irix README.agent-mibs
%doc README.Panasonic_AM3X.txt
	 
#%config(noreplace) /etc/net-snmp/snmpd.conf
	 
#%{_datadir}/snmp/snmpconf-data
%{_datadir}/snmp

%{_bindir}
%{_sbindir}
%{_mandir}/man1/*
# don't include Perl man pages, which start with caps
%{_mandir}/man3/[^A-Z]*
%{_mandir}/man5/*
%{_mandir}/man8/*
%{_libdir}/*.so*
/etc/rc.d/init.d/snmpd

%files devel
%defattr(-,root,root)

%{_includedir}
%{_libdir}/*.a
%{_libdir}/*.la

%if %{netsnmp_include_perl}
%files -f net-snmp-perl-files perlmods
%defattr(-,root,root)
%{_mandir}/man3/*::*
%{_mandir}/man3/SNMP*
%endif

%verifyscript
echo "No additional verification is done for net-snmp"

%changelog
* Wed Nov 23 2006 Thomas Anders <tanders@users.sf.net>
- fixes for 5.4 and 64-bit platforms
- enable Perl by default, but allow for --without perl_modules|embedded_perl
- add netsnmp_ prefix for local defines

* Fri Sep  1 2006 Thomas Anders <tanders@users.sf.net>
- Update to 5.4.dev
- introduce %{libcurrent}
- use new disman/event name
- add: README.aix README.osX README.tru64 README.irix README.agent-mibs
  README.Panasonic_AM3X.txt
- add new NetSNMP::agent::Support

* Fri Jan 13 2006 hardaker <hardaker@users.sf.net>
- Update to 5.3.0.1

* Wed Dec 28 2005 hardaker <hardaker@users.sf.net>
- Update to 5.3

* Tue Oct 28 2003 rs <rstory@users.sourceforge.net>
- fix conditional perl build after reading rpm docs

* Sat Oct  4 2003 rs <rstory@users.sourceforge.net> - 5.0.9-4
- fix to build without requiring arguments
- separate embedded perl and perl modules options
- fix fix for init.d script for non-/usr/local installation

* Fri Sep 26 2003 Wes Hardaker <hardaker@users.sourceforge.net>
- fix perl's UseNumeric
- fix init.d script for non-/usr/local installation

* Fri Sep 12 2003 Wes Hardaker <hardaker@users.sourceforge.net>
- fixes for 5.0.9's perl support

* Mon Sep 01 2003 Wes Hardaker <hardaker@users.sourceforge.net>
- added perl support

* Wed Oct 09 2002 Wes Hardaker <hardaker@users.sourceforge.net>
- Incorperated most of Mark Harig's better version of the rpm spec and Makefile

* Wed Oct 09 2002 Wes Hardaker <hardaker@users.sourceforge.net>
- Made it possibly almost usable.

* Mon Apr 22 2002 Robert Story <rstory@users.sourceforge.net>
- created
