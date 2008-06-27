proftpd-configure:
	cd proftpd && ./configure --host=mipsel-linux --prefix=/tmp/proftpd 
	
proftpd:
	$(MAKE) -C proftpd

proftpd-clean:
	$(MAKE) -C proftpd clean

proftpd-install:
	install -D proftpd/proftpd $(INSTALLDIR)/proftpd/usr/sbin/proftpd
	install -D proftpd/proftpd.conf $(INSTALLDIR)/proftpd/etc/config/proftpd.conf
	install -D proftpd/proftpd.startup $(INSTALLDIR)/proftpd/etc/config/proftpd.startup
	install -D proftpd/config/ftp.webservices $(INSTALLDIR)/proftpd/etc/config/ftp.webservices
	install -D proftpd/config/proftpd.nvramconfig $(INSTALLDIR)/proftpd/etc/config/proftpd.nvramconfig
	$(STRIP) $(INSTALLDIR)/proftpd/usr/sbin/proftpd


