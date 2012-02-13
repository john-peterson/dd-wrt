ifeq ($(CONFIG_HOTPLUG2),y)
export SAMBA3_EXTRA:= -DHAVE_DDINOTIFY
endif
CONFIGURE_VARS += \
	ac_cv_file__proc_sys_kernel_core_pattern=yes \
	libreplace_cv_HAVE_C99_VSNPRINTF=yes \
	libreplace_cv_HAVE_GETADDRINFO=yes \
	libreplace_cv_HAVE_IFACE_IFCONF=yes \
	LINUX_LFS_SUPPORT=yes \
	samba_cv_CC_NEGATIVE_ENUM_VALUES=yes \
	samba_cv_HAVE_GETTIMEOFDAY_TZ=yes \
	samba_cv_HAVE_IFACE_IFCONF=yes \
	samba_cv_HAVE_KERNEL_OPLOCKS_LINUX=yes \
	samba_cv_HAVE_SECURE_MKSTEMP=yes \
	samba_cv_HAVE_WRFILE_KEYTAB=no \
	samba_cv_USE_SETREUID=yes \
	samba_cv_USE_SETRESUID=yes \
	samba_cv_have_setreuid=yes \
	samba_cv_have_setresuid=yes

CONFIGURE_ARGS += \
	--host=$(ARCH)-linux \
	--exec-prefix=/usr \
	--prefix=/ \
	--disable-avahi \
	--disable-cups \
	--disable-pie \
	--disable-relro \
	--disable-static \
	--disable-swat \
	--disable-shared-libs \
	--with-codepagedir=/etc/samba \
	--with-configdir=/etc/samba \
	--with-included-iniparser \
	--with-included-popt \
	--with-lockdir=/var/lock \
	--with-logfilebase=/var/log \
	--with-nmbdsocketdir=/var/nmbd \
	--with-piddir=/var/run \
	--with-privatedir=/etc/samba \
	--with-sendfile-support \
	--without-cluster-support \
	--without-ads \
	--without-krb5 \
	--without-ldap \
	--without-pam \
	--without-winbind \
	--without-libtdb \
	--without-libtalloc \
	--without-libnetapi \
	--without-libsmbclient \
	--without-libsmbsharemodes \
	--without-libaddns \
	--with-shared-modules=
	


samba3-configure:
	if ! test -e "samba36/source3/Makefile"; then	cd samba36/source3 && ./configure $(CONFIGURE_VARS) $(CONFIGURE_ARGS) CFLAGS="$(COPTS) -DMAX_DEBUG_LEVEL=-1  -ffunction-sections -fdata-sections -Wl,--gc-sections $(LTO) $(SAMBA3_EXTRA)" LDFLAGS="$(COPTS) -DMAX_DEBUG_LEVEL=-1  -ffunction-sections -fdata-sections -Wl,--gc-sections $(LTO) $(SAMBA3_EXTRA)"; fi

samba3: samba3-configure
	$(MAKE) -C samba36/source3 all WITH_LFS=yes DYNEXP= 

samba3-install:
	mkdir -p $(INSTALLDIR)/samba3
	install -D samba36/source3/bin/samba_multicall $(INSTALLDIR)/samba3/usr/sbin/samba_multicall
	install -D samba36/source3/bin/smbpasswd $(INSTALLDIR)/samba3/usr/sbin/smbpasswd
	install -D samba36/config/samba3.webnas $(INSTALLDIR)/samba3/etc/config/samba3.webnas
	install -D samba36/config/samba3.nvramconfig $(INSTALLDIR)/samba3/etc/config/samba3.nvramconfig
	cd  $(INSTALLDIR)/samba3/usr/sbin && ln -sf samba_multicall smbd
	cd  $(INSTALLDIR)/samba3/usr/sbin && ln -sf samba_multicall nmbd


samba3-clean:
	-$(MAKE) -C samba36/source3 clean
