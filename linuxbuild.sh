

gcc -D_LINUX -fPIC -g -c gm0.c -o gm0.o
gcc -D_LINUX -fPIC -g -c gm0_comms.c -o gm0_comms.o
gcc -D_LINUX -fPIC -g -c linuxcomms.c -o linuxcomms.o

gcc -D_LINUX -shared -Wl,-soname,libgm.so.1 -lc -lm -lpthread -o libgm.so.1.0.0 gm0.o gm0_comms.o linuxcomms.o

cp libgm.so.1.0.0 /usr/local/lib/
ldconfig /usr/local/lib
ln -s /usr/local/lib/libgm.so.1.0.0 /usr/local/lib/libgm.so
ln -s /usr/local/lib/libgm.so.1.0.0 /usr/local/lib/libgm.so.1

#gcc -D_LINUX -I gm0lib/src/ tools/src/gm0test.c -lc -lgm -o tools/gmtest

#gcc -D_LINUX -I gm0lib/src/ tools/src/gmtime.c -lc -lgm -o tools/gmtime

#gcc -D_LINUX -I gm0lib/src/ tools/src/gmupload.c -lc -lgm -o tools/gmupload

#gcc -D_LINUX -I gm0lib/src/ tools/src/gmdata.c -lc -lgm -o tools/gmdata

#gcc -D_LINUX -I gm0lib/src/ tools/src/gmcmd.c -lc -lgm -o tools/gmcmd
