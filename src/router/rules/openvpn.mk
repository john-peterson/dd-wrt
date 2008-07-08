openvpn-configure: openssl
	cd lzo && ./configure --host=$(ARCH)-linux CFLAGS="$(COPTS)  -ffunction-sections -fdata-sections -Wl,--gc-sections"
	cd openvpn && ./configure --host=$(ARCH)-linux CPPFLAGS="-ffunction-sections -fdata-sections -Wl,--gc-sections -I../lzo/include -I../openssl/include -L../lzo -L../openssl -L../lzo/src/.libs" --enable-static --disable-shared --disable-pthread --disable-plugins --disable-debug --enable-password-save --enable-management --enable-lzo --enable-small --enable-server CFLAGS="$(COPTS)  -ffunction-sections -fdata-sections -Wl,--gc-sections" LDFLAGS="-L../openssl -L../lzo -L../lzo/src/.libs  -ffunction-sections -fdata-sections -Wl,--gc-sections"
	make -C lzo

openvpn: openssl
#ifeq ($(CONFIG_NEWMEDIA),y)
#else
#	cd openvpn && ./configure --host=$(ARCH)-linux CPPFLAGS="-ffunction-sections -fdata-sections -Wl,--gc-sections -I../lzo/include -I../openssl/include -L../lzo -L../openssl -L../lzo/src/.libs" --enable-static --disable-shared --disable-pthread --disable-plugins --disable-debug --disable-management --disable-socks --enable-lzo --enable-small --enable-server --enable-http --enable-password-save CFLAGS="$(COPTS)  -ffunction-sections -fdata-sections -Wl,--gc-sections" LDFLAGS="-L../openssl -L../lzo -L../lzo/src/.libs  -ffunction-sections -fdata-sections -Wl,--gc-sections"
#endif
	make -C lzo clean
	make -C lzo
ifneq ($(CONFIG_MADWIFI),y)
	rm -f openssl/*.so*
endif
ifeq ($(CONFIG_NEWMEDIA),y)
	make -C openvpn clean
else
	make -C openvpn clean
endif
	make -C openvpn

openvpn-install:
	install -D openvpn/openvpn $(INSTALLDIR)/openvpn/usr/sbin/openvpn

ifeq ($(CONFIG_AIRNET),y)
	install -D openvpn/config-airnet/openvpncl.nvramconfig $(INSTALLDIR)/openvpn/etc/config/openvpncl.nvramconfig
	install -D openvpn/config-airnet/openvpncl.webservices $(INSTALLDIR)/openvpn/etc/config/openvpncl.webservices
	install -D openvpn/config-airnet/openvpn.nvramconfig $(INSTALLDIR)/openvpn/etc/config/openvpn.nvramconfig
	install -D openvpn/config-airnet/openvpn.webservices $(INSTALLDIR)/openvpn/etc/config/openvpn.webservices
else
	install -D openvpn/config/openvpncl.nvramconfig $(INSTALLDIR)/openvpn/etc/config/openvpncl.nvramconfig
	install -D openvpn/config/openvpncl.webservices $(INSTALLDIR)/openvpn/etc/config/openvpncl.webservices
	install -D openvpn/config2/openvpn.nvramconfig $(INSTALLDIR)/openvpn/etc/config/openvpn.nvramconfig
	install -D openvpn/config2/openvpn.webservices $(INSTALLDIR)/openvpn/etc/config/openvpn.webservices
endif

openvpn-clean:
	if test -e "openvpn/Makefile"; then make -C openvpn clean; fi



