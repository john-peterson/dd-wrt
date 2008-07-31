cd ../src
#cp cy_std.mak cy_conf.mak
#cp cy_std.h cy_conf.h

cd router
make shared-clean
make rc-clean
make httpd-clean

cp .config_openvpn.v24 .config
make clean
#cp Makefile.standard Makefile
rm -rf mipsel-uclibc/install
make rc-clean
make services-clean
make shared-clean
make httpd-clean
rm busybox/busybox
rm busybox/applets/busybox.o

#copy config.normal .config
cd ..
make
cd ../opt
mkdir ../src/router/mipsel-uclibc/target/etc/config
#mkdir ../src/router/mipsel-uclibc/target/etc/langpack
./sstrip/sstrip ../src/router/mipsel-uclibc/target/bin/*
./sstrip/sstrip ../src/router/mipsel-uclibc/target/sbin/*
./sstrip/sstrip ../src/router/mipsel-uclibc/target/usr/sbin/*

cp ./bin/ipkg ../src/router/mipsel-uclibc/target/bin

cp ./libgcc/* ../src/router/mipsel-uclibc/target/lib
cd ../src/router/mipsel-uclibc/target/lib
ln -s libgcc_s.so.1 libgcc_s.so
cd ../../../../../opt
cp ./etc/preinit ../src/router/mipsel-uclibc/target/etc
cp ./etc/postinit ../src/router/mipsel-uclibc/target/etc
cp ./etc/ipkg.conf ../src/router/mipsel-uclibc/target/etc
cp ./etc/config/* ../src/router/mipsel-uclibc/target/etc/config
cp ./usr/lib/smb.conf ../src/router/mipsel-uclibc/target/usr/lib
cd ../src/router/mipsel-uclibc/target/www

ln -s ../tmp/smbshare smb

cd ../../../../../opt


./strip_libs.sh


# make language packs
#diff -r -a -w ./lang/spanish/www ../src/router/www > lang/langpacks/spanish.diff
#gzip -9 lang/langpacks/spanish.diff

#copy language packs to destination
#cp ./lang/langpacks/* ../src/router/mipsel-uclibc/target/langpacks
#cp ./lang/* ../src/router/mipsel-uclibc/target/etc/langpack
../src/linux/brcm/linux.v23/scripts/squashfs/mksquashfs-lzma ../src/router/mipsel-uclibc/target target.squashfs -noappend -root-owned -le 
./make_kernel.sh
../tools/trx -o dd-wrt.v23.trx ./loader-0.02/loader.gz ../src/router/mipsel-uclibc/vmlinuz target.squashfs
../tools/trx_gs -o dd-wrt.v23_gs.trx ./loader-0.02/loader.gz ../src/router/mipsel-uclibc/vmlinuz target.squashfs
./asus/asustrx -p WL500gx -v 1.9.2.7 -o dd-wrt.v23_asus.trx ./loader-0.02/loader.gz ../src/router/mipsel-uclibc/vmlinuz target.squashfs
#add pattern
./tools/addpattern -4 -p W54G -v v4.20.6 -i dd-wrt.v23.trx -o dd-wrt.v23_wrt54g.bin -g
./tools/addpattern -4 -p W54S -v v4.70.6 -i dd-wrt.v23.trx -o dd-wrt.v23_wrt54gs.bin -g
./tools/addpattern -4 -p EWCG -v v4.20.6 -i dd-wrt.v23.trx -o dd-wrt.v23_wrt350n.bin -g
./tools/addpattern -4 -p EWCB -v v4.20.6 -i dd-wrt.v23.trx -o dd-wrt.v23_wrt300n.bin -g
./tools/addpattern -4 -p W54U -v v4.70.6 -i dd-wrt.v23.trx -o dd-wrt.v23_wrtsl54gs.bin -g
./tools/addpattern -4 -p W54s -v v1.05.0 -i dd-wrt.v23.trx -o dd-wrt.v23_wrt54gsv4.bin -g

#./tools/addpattern -i dd-wrt.v23.trx -o dd-wrt.v23_wrt54gs.bin -2 -g
#sed -e  1s,^W54S,W54G, < dd-wrt.v23_wrt54gs.bin > dd-wrt.v23_wrt54g.bin 

cp dd-wrt.v23.trx ~/GruppenLW/dd-wrt.v24_vpn_generic.bin

../tools/mkimage dd-wrt.v23.trx
cp Trailed* ~/GruppenLW/dd-wrt.v24_vpn_wrt600n.bin

#cp dd-wrt.v23.prefinal5_asus.trx ~/GruppenLW