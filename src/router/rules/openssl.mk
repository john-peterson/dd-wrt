openssl:
	$(MAKE) -C openssl CC="$(ARCH)-linux-uclibc-gcc -I$(TOP)/zlib" MAKEDEPPROG=$(ARCH)-linux-uclibc-gcc 
	$(MAKE) -C openssl build-shared CC="$(ARCH)-linux-uclibc-gcc -I$(TOP)/zlib" MAKEDEPPROG=$(ARCH)-linux-uclibc-gcc
	$(MAKE) -C openssl build_apps CC="$(ARCH)-linux-uclibc-gcc -I$(TOP)/zlib" MAKEDEPPROG=$(ARCH)-linux-uclibc-gcc
#ifeq ($(ARCH),mipsel)
#ifneq ($(ARCHITECTURE),rb532)
#	rm -f openssl/libcrypto.so.0.9.8
#	rm -f openssl/libssl.so.0.9.8
#endif
#endif

openssl-install:
ifeq ($(CONFIG_MADWIFI),y)
	install -D openssl/libcrypto.so.0.9.8 $(INSTALLDIR)/openssl/usr/lib/libcrypto.so.0.9.8
	install -D openssl/libssl.so.0.9.8 $(INSTALLDIR)/openssl/usr/lib/libssl.so.0.9.8
endif
ifneq ($(ARCH),mipsel)
	install -D openssl/apps/openssl $(INSTALLDIR)/openssl/usr/sbin/openssl
endif
#ifeq ($(ARCHITECTURE),rb532)
#	install -D openssl/libcrypto.so.0.9.8 $(INSTALLDIR)/openssl/usr/lib/libcrypto.so.0.9.8
#	install -D openssl/libssl.so.0.9.8 $(INSTALLDIR)/openssl/usr/lib/libssl.so.0.9.8
#	install -D openssl/apps/openssl $(INSTALLDIR)/openssl/usr/sbin/openssl
#endif
#	@true

openssl-clean:
	$(MAKE) -C openssl clean

