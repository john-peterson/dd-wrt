openvpn-configure:
	cd lzo && ./configure --host=$(ARCH)-linux CFLAGS="$(COPTS)"
ifeq ($(CONFIG_NEWMEDIA),y)
	cd openvpn && ./configure --host=$(ARCH)-linux CPPFLAGS="-I../lzo/include -I../openssl/include -L../lzo -L../openssl -L../lzo/src/.libs" --enable-static --disable-shared --disable-pthread --disable-plugins --disable-debug --enable-password-save --enable-management --enable-lzo --enable-small --enable-server CFLAGS="$(COPTS)" LDFLAGS="-L../openssl -L../lzo -L../lzo/src/.libs"
else
	cd openvpn && ./configure --host=$(ARCH)-linux CPPFLAGS="-I../lzo/include -I../openssl/include -L../lzo -L../openssl -L../lzo/src/.libs" --enable-static --disable-shared --disable-pthread --disable-plugins --disable-debug --disable-management --disable-socks --enable-lzo --enable-small --enable-server --enable-http --enable-password-save CFLAGS="$(COPTS)" LDFLAGS="-L../openssl -L../lzo -L../lzo/src/.libs"
endif

openvpn:
	make -C lzo clean
	make -C lzo
#	rm -f openssl/*.so*
ifeq ($(CONFIG_NEWMEDIA),y)
	make -C openvpn clean
else
	make -C openvpn clean
endif
	make -C openvpn

openvpn-install:
	install -D openvpn/openvpn $(INSTALLDIR)/openvpn/usr/sbin/openvpn
ifneq ($(CONFIG_NEWMEDIA),y)	
	install -D openvpn/config/openvpn.nvramconfig $(INSTALLDIR)/openvpn/etc/config/openvpn.nvramconfig
	install -D openvpn/config/openvpn.webservices $(INSTALLDIR)/openvpn/etc/config/openvpn.webservices
else
	install -D openvpn/config2/openvpn.nvramconfig $(INSTALLDIR)/openvpn/etc/config/openvpn.nvramconfig
	install -D openvpn/config2/openvpn.webservices $(INSTALLDIR)/openvpn/etc/config/openvpn.webservices
endif

openvpn-clean:
	if test -e "openvpn/Makefile"; then make -C openvpn clean; fi



