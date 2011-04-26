jansson-configure:
	cd jansson && ./configure --host=$(ARCH)-linux CC=$(CC) CFLAGS="$(COPTS) -ffunction-sections -fdata-sections -Wl,--gc-sections"

jansson:
	$(MAKE) -C jansson

jansson-clean:
	if test -e "jansson/Makefile"; then make -C jansson clean; fi
	@true

jansson-install:
	install -D jansson/src/.libs/libjansson.so $(INSTALLDIR)/jansson/usr/lib/libjansson.so.0
