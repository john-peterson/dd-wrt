ifeq ($(ARCH),mipsel)
madwifi:
	make -C madwifi.dev/madwifi.dev KERNELPATH=$(LINUXDIR) TARGET=mipsisa32-le-elf
	make -C madwifi.dev/madwifi.dev/tools TARGET=mipsisa32-le-elf BINDIR=$(INSTALLDIR)/madwifi/usr/sbin

madwifi-clean:
	make -C madwifi.dev/madwifi.dev clean KERNELPATH=$(LINUXDIR) TARGET=mipsisa32-le-elf
	make -C madwifi.dev/madwifi.dev/tools BINDIR=$(INSTALLDIR)/madwifi/usr/sbin clean

madwifi-install:
	mkdir -p $(INSTALLDIR)/madwifi/usr/sbin
	make -C madwifi.dev/madwifi.dev/tools BINDIR=$(INSTALLDIR)/madwifi/usr/sbin install
	make -C madwifi.dev/madwifi.dev KERNELPATH=$(LINUXDIR) BINDIR=/usr/sbin DESTDIR=$(INSTALLDIR)/madwifi TARGET=mipsisa32-le-elf install
endif

ifeq ($(ARCH),armeb)
madwifi:
ifeq ($(CONFIG_BOESE),y)
	make -C madwifi.dev/madwifi.dev KERNELPATH=$(LINUXDIR) TARGET=xscale-boese-be-elf  
	make -C madwifi.dev/madwifi.dev/tools CFLAGS="-DBOESE=1 $(COPTS)" TARGET=xscale-boese-be-elf BINDIR=$(INSTALLDIR)/madwifi/usr/sbin
#	make -C madwifi.dev/madwifi.dev/diag TARGET=xscale-$(MADFLAG)be-elf BINDIR=$(INSTALLDIR)/madwifi/usr/sbin 

else
	make -C madwifi.dev/madwifi.dev KERNELPATH=$(LINUXDIR) TARGET=xscale-$(MADFLAG)be-elf  
	make -C madwifi.dev/madwifi.dev/tools CFLAGS="$(CONFIG_MADWIFIFLAGS) $(COPTS)" TARGET=xscale-$(MADFLAG)be-elf BINDIR=$(INSTALLDIR)/madwifi/usr/sbin 
#	make -C madwifi.dev/madwifi.dev/diag TARGET=xscale-$(MADFLAG)be-elf BINDIR=$(INSTALLDIR)/madwifi/usr/sbin 

endif

madwifi-clean:
ifeq ($(CONFIG_BOESE),y)
	make -C madwifi.dev/madwifi.dev clean KERNELPATH=$(LINUXDIR) TARGET=xscale-boese-be-elf
else
	make -C madwifi.dev/madwifi.dev clean KERNELPATH=$(LINUXDIR) TARGET=xscale-$(MADFLAG)be-elf
#	make -C madwifi.dev/madwifi.dev/diag TARGET=xscale-$(MADFLAG)be-elf BINDIR=$(INSTALLDIR)/madwifi/usr/sbin clean
endif
	make -C madwifi.dev/madwifi.dev/tools BINDIR=$(INSTALLDIR)/madwifi/usr/sbin clean
#	make -C madwifi.dev/madwifi.dev/diag TARGET=xscale-$(MADFLAG)be-elf BINDIR=$(INSTALLDIR)/madwifi/usr/sbin clean

madwifi-install:
	mkdir -p $(INSTALLDIR)/madwifi/usr/sbin
	make -C madwifi.dev/madwifi.dev/tools BINDIR=$(INSTALLDIR)/madwifi/usr/sbin install
#	make -C madwifi.dev/madwifi.dev/diag TARGET=xscale-$(MADFLAG)be-elf BINDIR=$(INSTALLDIR)/madwifi/usr/sbin install
ifeq ($(CONFIG_BOESE),y)
	make -C madwifi.dev/madwifi.dev KERNELPATH=$(LINUXDIR) BINDIR=/usr/sbin DESTDIR=$(INSTALLDIR)/madwifi TARGET=xscale-boese-be-elf install
else
	make -C madwifi.dev/madwifi.dev KERNELPATH=$(LINUXDIR) BINDIR=/usr/sbin DESTDIR=$(INSTALLDIR)/madwifi TARGET=xscale-$(MADFLAG)be-elf install
endif
endif

ifeq ($(ARCH),mips)
ifeq ($(ARCHITECTURE),fonera)
madwifi:
	make -C madwifi.dev/madwifi.dev KERNELPATH=$(LINUXDIR) TARGET=ap51 BUS=AHB TOOLPATH=$(LINUXDIR) LDOPTS="--no-warn-mismatch"
	make -C madwifi.dev/madwifi.dev/tools TARGET=ap51 BINDIR=$(INSTALLDIR)/madwifi/usr/sbin BUS=AHB TOOLPATH=$(LINUXDIR)

madwifi-clean:
	make -C madwifi.dev/madwifi.dev clean KERNELPATH=$(LINUXDIR) TARGET=ap51 BUS=AHB TOOLPATH=$(LINUXDIR)
	make -C madwifi.dev/madwifi.dev/tools BINDIR=$(INSTALLDIR)/madwifi/usr/sbin clean BUS=AHB TOOLPATH=$(LINUXDIR)

madwifi-install:
	mkdir -p $(INSTALLDIR)/madwifi/usr/sbin
	make -C madwifi.dev/madwifi.dev/tools BINDIR=$(INSTALLDIR)/madwifi/usr/sbin BUS=AHB TOOLPATH=$(LINUXDIR) install
	make -C madwifi.dev/madwifi.dev/ KERNELPATH=$(LINUXDIR) BINDIR=/usr/sbin BUS=AHB TOOLPATH=$(LINUXDIR) DESTDIR=$(INSTALLDIR)/madwifi TARGET=ap51 install
else
ifeq ($(ARCHITECTURE),whrag108)
madwifi:
#	make -C madwifi.dev/madwifi.dev/diag TARGET=xscale-$(MADFLAG)be-elf BINDIR=$(INSTALLDIR)/madwifi/usr/sbin 
ifeq ($(CONFIG_BOESE),y)
	make -C madwifi.dev/madwifi.dev KERNELPATH=$(LINUXDIR) TARGET=ap30-boese-be-elf  BUS=AHB   TOOLPATH=$(LINUXDIR)
	make -C madwifi.dev/madwifi.dev/tools CFLAGS="-DBOESE=1 $(COPTS)" TARGET=ap30-boese-be-elf BUS=AHB  BINDIR=$(INSTALLDIR)/madwifi/usr/sbin  TOOLPATH=$(LINUXDIR)
else
	make -C madwifi.dev/madwifi.dev KERNELPATH=$(LINUXDIR) TARGET=ap30-$(MADFLAG)  BUS=AHB   TOOLPATH=$(LINUXDIR)
	make -C madwifi.dev/madwifi.dev/tools CFLAGS="$(CONFIG_MADWIFIFLAGS) $(COPTS) -DNEED_PRINTF" TARGET=ap30-$(MADFLAG)elf  TOOLPATH=$(LINUXDIR) BUS=AHB BINDIR=$(INSTALLDIR)/madwifi/usr/sbin 
endif

madwifi-clean:
#	make -C madwifi.dev/madwifi.dev/diag TARGET=xscale-$(MADFLAG)be-elf BINDIR=$(INSTALLDIR)/madwifi/usr/sbin clean
ifeq ($(CONFIG_BOESE),y)
	make -C madwifi.dev/madwifi.dev clean KERNELPATH=$(LINUXDIR) TARGET=ap30-boese-be-elf  BUS=AHB  TOOLPATH=$(LINUXDIR)
else
	make -C madwifi.dev/madwifi.dev clean KERNELPATH=$(LINUXDIR) TARGET=ap30-$(MADFLAG) BUS=AHB  TOOLPATH=$(LINUXDIR)
endif
	make -C madwifi.dev/madwifi.dev/tools BINDIR=$(INSTALLDIR)/madwifi/usr/sbin clean 

madwifi-install:
	mkdir -p $(INSTALLDIR)/madwifi/usr/sbin
	make -C madwifi.dev/madwifi.dev/tools BINDIR=$(INSTALLDIR)/madwifi/usr/sbin install
#	make -C madwifi.dev/madwifi.dev/diag TARGET=xscale-$(MADFLAG)be-elf BINDIR=$(INSTALLDIR)/madwifi/usr/sbin install
ifeq ($(CONFIG_BOESE),y)
	make -C madwifi.dev/madwifi.dev KERNELPATH=$(LINUXDIR) BINDIR=/usr/sbin BUS=AHB TOOLPATH=$(LINUXDIR)  DESTDIR=$(INSTALLDIR)/madwifi TARGET=ap30-boese-be-elf install
else
	make -C madwifi.dev/madwifi.dev KERNELPATH=$(LINUXDIR) BINDIR=/usr/sbin BUS=AHB TOOLPATH=$(LINUXDIR)  DESTDIR=$(INSTALLDIR)/madwifi TARGET=ap30-$(MADFLAG) install
endif
else
ifeq ($(ARCHITECTURE),ca8)
madwifi:
#	make -C madwifi.dev/madwifi.dev/diag TARGET=xscale-$(MADFLAG)be-elf BINDIR=$(INSTALLDIR)/madwifi/usr/sbin 
ifeq ($(CONFIG_BOESE),y)
	make -C madwifi.dev/madwifi.dev KERNELPATH=$(LINUXDIR) TARGET=ap43-boese-be-elf  BUS=AHB   TOOLPATH=$(LINUXDIR)
	make -C madwifi.dev/madwifi.dev/tools CFLAGS="-DBOESE=1 $(COPTS)" TARGET=ap43-boese-be-elf BUS=AHB  BINDIR=$(INSTALLDIR)/madwifi/usr/sbin  TOOLPATH=$(LINUXDIR)
else
	make -C madwifi.dev/madwifi.dev KERNELPATH=$(LINUXDIR) TARGET=ap43-$(MADFLAG)  BUS=AHB   TOOLPATH=$(LINUXDIR)
	make -C madwifi.dev/madwifi.dev/tools CFLAGS="$(CONFIG_MADWIFIFLAGS) $(COPTS) -DNEED_PRINTF" TARGET=ap43-$(MADFLAG)elf  TOOLPATH=$(LINUXDIR) BUS=AHB BINDIR=$(INSTALLDIR)/madwifi/usr/sbin 
endif

madwifi-clean:
#	make -C madwifi.dev/madwifi.dev/diag TARGET=xscale-$(MADFLAG)be-elf BINDIR=$(INSTALLDIR)/madwifi/usr/sbin clean
ifeq ($(CONFIG_BOESE),y)
	make -C madwifi.dev/madwifi.dev clean KERNELPATH=$(LINUXDIR) TARGET=ap43-boese-be-elf  BUS=AHB  TOOLPATH=$(LINUXDIR)
else
	make -C madwifi.dev/madwifi.dev clean KERNELPATH=$(LINUXDIR) TARGET=ap43-$(MADFLAG) BUS=AHB  TOOLPATH=$(LINUXDIR)
endif
	make -C madwifi.dev/madwifi.dev/tools BINDIR=$(INSTALLDIR)/madwifi/usr/sbin clean 

madwifi-install:
	mkdir -p $(INSTALLDIR)/madwifi/usr/sbin
	make -C madwifi.dev/madwifi.dev/tools BINDIR=$(INSTALLDIR)/madwifi/usr/sbin install
#	make -C madwifi.dev/madwifi.dev/diag TARGET=xscale-$(MADFLAG)be-elf BINDIR=$(INSTALLDIR)/madwifi/usr/sbin install
ifeq ($(CONFIG_BOESE),y)
	make -C madwifi.dev/madwifi.dev KERNELPATH=$(LINUXDIR) BINDIR=/usr/sbin BUS=AHB TOOLPATH=$(LINUXDIR)  DESTDIR=$(INSTALLDIR)/madwifi TARGET=ap43-boese-be-elf install
else
	make -C madwifi.dev/madwifi.dev KERNELPATH=$(LINUXDIR) BINDIR=/usr/sbin BUS=AHB TOOLPATH=$(LINUXDIR)  DESTDIR=$(INSTALLDIR)/madwifi TARGET=ap43-$(MADFLAG) install
endif

else
madwifi:
	make -C madwifi/madwifi.dev KERNELPATH=$(LINUXDIR) TARGET=mipsisa32-be-elf
	make -C madwifi/madwifi.dev/tools TARGET=mipsisa32-be-elf BINDIR=$(INSTALLDIR)/madwifi/usr/sbin

madwifi-clean:
	make -C madwifi/madwifi.dev clean KERNELPATH=$(LINUXDIR) TARGET=mipsisa32-be-elf
	make -C madwifi/madwifi.dev/tools BINDIR=$(INSTALLDIR)/madwifi/usr/sbin clean

madwifi-install:
	mkdir -p $(INSTALLDIR)/madwifi/usr/sbin
	make -C madwifi/madwifi.dev/tools BINDIR=$(INSTALLDIR)/madwifi/usr/sbin install
	make -C madwifi/madwifi.dev KERNELPATH=$(LINUXDIR) BINDIR=/usr/sbin DESTDIR=$(INSTALLDIR)/madwifi TARGET=mipsisa32-be-elf install
endif
endif
endif
endif
