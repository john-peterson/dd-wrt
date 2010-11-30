

asterisk-configure:
	-rm asterisk/menuselect.makeopts
	cd asterisk && ./configure --host=$(ARCH)-linux-uclibc \
	--without-curl \
	--without-curses \
	--with-gsm=internal \
	--without-gtk \
	--without-gtk2 \
	--without-isdnnet \
	--without-kde \
	--without-misdn \
	--without-nbs \
	--with-ncurses="$(TOP)/ncurses" \
	--without-netsnmp \
	--without-newt \
	--without-odbc \
	--without-ogg \
	--without-osptk \
	--without-pri \
	--without-qt \
	--without-radius \
	--without-sdl \
	--without-spandsp \
	--without-suppserv \
	--without-tds \
	--without-termcap \
	--without-tinfo \
	--without-vorbis \
	--without-vpb \
	--with-z="$(TOP)/zlib" \
	--disable-xmldoc \
	--without-dahdi \
	--without-gnutls \
	--without-iksemel

asterisk:
	$(MAKE) -C asterisk \
		include/asterisk/version.h \
		include/asterisk/buildopts.h defaults.h \
		makeopts.embed_rules
	ASTCFLAGS="$(COPTS) -DLOW_MEMORY -fPIC -I$(TOP)/ncurses/include" \
	ASTLDFLAGS="$(COPTS) -DLOW_MEMORY -fPIC -L$(TOP)/ncurses/lib" \
	$(MAKE) -C asterisk \
		ASTVARLIBDIR="/usr/lib/asterisk" \
		NOISY_BUILD="1" \
		DEBUG="" \
		OPTIMIZE="" \
		all
	make -C asterisk

asterisk-install:
	ASTCFLAGS="$(COPTS) -DLOW_MEMORY -fPIC -I$(TOP)/ncurses/include" \
	ASTLDFLAGS="$(COPTS) -DLOW_MEMORY -fPIC -L$(TOP)/ncurses/lib" \
	$(MAKE) -C asterisk \
		ASTVARLIBDIR="/usr/lib/asterisk" \
		NOISY_BUILD="1" \
		DEBUG="" \
		OPTIMIZE="" \
		DESTDIR=/tmp/asterisk \
		install samples
	ASTCFLAGS="$(COPTS) -DLOW_MEMORY -fPIC -I$(TOP)/ncurses/include" \
	ASTLDFLAGS="$(COPTS) -DLOW_MEMORY -fPIC -L$(TOP)/ncurses/lib" \
	$(MAKE) -C asterisk \
		ASTVARLIBDIR="/usr/lib/asterisk" \
		NOISY_BUILD="1" \
		DEBUG="" \
		OPTIMIZE="" \
		DESTDIR=/tmp/asterisk \
		adsi
#	sed 's|/var/lib/asterisk|/usr/lib/asterisk|g' $(INSTALLDIR)/asterisk/etc/asterisk/musiconhold.conf
	$(INSTALL_DIR) -p $(INSTALLDIR)/asterisk/etc/asterisk
	for f in asterisk extensions features \
		logger manager modules \
		sip sip_notify rtp; do \
		$(CP) /tmp/asterisk/etc/asterisk/$$f.conf $(INSTALLDIR)/asterisk/etc/asterisk/ ; \
	done
	$(INSTALL_DIR) $(INSTALLDIR)/asterisk/usr/lib/asterisk/modules
	for f in app_dial app_echo app_playback app_macro \
		chan_sip \
		codec_ulaw codec_gsm \
		format_gsm format_pcm format_wav format_wav_gsm \
		pbx_config \
		func_strings func_timeout func_callerid; do \
		$(CP) /tmp/asterisk/usr/lib/asterisk/modules/$$f.so $(INSTALLDIR)/asterisk/usr/lib/asterisk/modules/ ; \
	done
	$(INSTALL_DIR) $(INSTALLDIR)/asterisk/usr/sbin
	$(CP) /tmp/asterisk/usr/sbin/asterisk $(INSTALLDIR)/asterisk/usr/sbin/
	$(CP) /tmp/asterisk/usr/sbin/astgenkey $(INSTALLDIR)/asterisk/usr/sbin/
	$(CP) /tmp/asterisk/usr/sbin/astcanary $(INSTALLDIR)/asterisk/usr/sbin/
	ln -s asterisk $(INSTALLDIR)/asterisk/usr/sbin/rasterisk
	$(INSTALL_DIR) $(INSTALLDIR)/asterisk/etc/asterisk
	$(INSTALL_DATA) /tmp/asterisk/etc/asterisk/voicemail.conf $(INSTALLDIR)/asterisk/etc/asterisk/
	$(INSTALL_DIR)  $(INSTALLDIR)/asterisk/usr/lib/asterisk/modules
	$(INSTALL_BIN) /tmp/asterisk/usr/lib/asterisk/modules/*voicemail.so $(INSTALLDIR)/asterisk/usr/lib/asterisk/modules/
	$(INSTALL_BIN) /tmp/asterisk/usr/lib/asterisk/modules/res_adsi.so $(INSTALLDIR)/asterisk/usr/lib/asterisk/modules/
#	$(INSTALL_DIR) $(INSTALLDIR)/asterisk/usr/lib/asterisk/sounds/
#	$(CP) /tmp/asterisk/usr/lib/asterisk/sounds/en/vm-*.gsm $(INSTALLDIR)/asterisk/usr/lib/asterisk/sounds/

	$(INSTALL_DIR) $(INSTALLDIR)/asterisk/etc/asterisk
	$(INSTALL_DATA) /tmp/asterisk/etc/asterisk/iax.conf $(INSTALLDIR)/asterisk/etc/asterisk/
	$(INSTALL_DATA) /tmp/asterisk/etc/asterisk/iaxprov.conf $(INSTALLDIR)/asterisk/etc/asterisk/

	$(INSTALL_DIR) $(INSTALLDIR)/asterisk/usr/lib/asterisk/modules
	$(INSTALL_BIN) /tmp/asterisk/usr/lib/asterisk/modules/chan* $(INSTALLDIR)/asterisk/usr/lib/asterisk/modules/

	$(INSTALL_DIR) $(INSTALLDIR)/asterisk/usr/lib/asterisk/modules
	$(INSTALL_BIN) /tmp/asterisk/usr/lib/asterisk/modules/app_system.so $(INSTALLDIR)/asterisk/usr/lib/asterisk/modules/

	$(INSTALL_DIR) $(INSTALLDIR)/asterisk/usr/lib/asterisk/modules
	$(INSTALL_BIN) /tmp/asterisk/usr/lib/asterisk/modules/format* $(INSTALLDIR)/asterisk/usr/lib/asterisk/modules/

	$(INSTALL_DIR) $(INSTALLDIR)/asterisk/etc/asterisk
	$(INSTALL_DATA) /tmp/asterisk/etc/asterisk/cdr*.conf $(INSTALLDIR)/asterisk/etc/asterisk/
	$(INSTALL_DIR) $(INSTALLDIR)/asterisk/usr/lib/asterisk/modules
	$(INSTALL_BIN) /tmp/asterisk/usr/lib/asterisk/modules/*cdr*.so $(INSTALLDIR)/asterisk/usr/lib/asterisk/modules/

	$(INSTALL_DIR) $(INSTALLDIR)/asterisk/etc/asterisk
	$(INSTALL_DATA) /tmp/asterisk/etc/asterisk/musiconhold.conf $(INSTALLDIR)/asterisk/etc/asterisk/
	$(INSTALL_DIR) $(INSTALLDIR)/asterisk/usr/lib/asterisk/modules
	$(INSTALL_BIN) /tmp/asterisk/usr/lib/asterisk/modules/res* $(INSTALLDIR)/asterisk/usr/lib/asterisk/modules/
	rm -rf /tmp/asterisk

