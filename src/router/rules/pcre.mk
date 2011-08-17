pcre-configure:
	cd pcre && ./configure --host=$(ARCH)-linux-uclibc CFLAGS="$(COPTS) -DNEED_PRINTF" --prefix=/usr --disable-xmldoc --enable-utf8 --enable-unicode-properties --disable-pcretest-libreadline

pcre:
	$(MAKE) -C pcre CFLAGS="$(COPTS) -DNEED_PRINTF" CXXFLAGS="$(COPTS) -DNEED_PRINTF" CPPFLAGS="$(COPTS) -DNEED_PRINTF"

pcre-clean:
	$(MAKE) -C pcre clean CFLAGS="$(COPTS) -DNEED_PRINTF"

pcre-install:
	install -D pcre/.libs/libpcre.so.0 $(INSTALLDIR)/pcre/usr/lib/libpcre.so.0
	install -D pcre/.libs/libpcrecpp.so.0 $(INSTALLDIR)/pcre/usr/lib/libpcrecpp.so.0
	install -D pcre/.libs/libpcreposix.so.0 $(INSTALLDIR)/pcre/usr/lib/libpcreposix.so.0
