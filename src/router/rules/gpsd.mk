gpsd:
	cd gpsd && ./configure --host=$(ARCH)-linux --disable-fv18 --disable-python --disable-tripmate --disable-earthmate --disable-shared --without-x CFLAGS="$(COPTS) -I../ncurses/include -L../ncurses/lib" --prefix=/usr 
	make  -C gpsd

gpsd-clean:
	make  -C gpsd clean

gpsd-install:
	make  -C gpsd install DESTDIR=$(INSTALLDIR)/gpsd
	rm -rf $(INSTALLDIR)/gpsd/usr/man
	rm -rf $(INSTALLDIR)/gpsd/usr/lib
	rm -rf $(INSTALLDIR)/gpsd/usr/share
	rm -rf $(INSTALLDIR)/gpsd/usr/lib64
	rm -rf $(INSTALLDIR)/gpsd/usr/include

