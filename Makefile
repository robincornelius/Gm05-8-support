# the linux part of this project is currenty too small to warrent automake 
# and i dont know how to use it vey well


EXE_DIR = /usr/local/bin
LIB_DIR = /usr/local/lib

CC = gcc

CFLAGS = -D_LINUX -Wall -O2
CLIBFLAGS = -D_LINUX -fPIC

COMPILE = $(CC) $(CFLAGS) -c
COMPILELIB = $(CC) $(CLIBFLAGS) -c


all: gm0lib dlltest

clean:
	rm *.o
	rm dlltest
	rm libgm.so.*

install: installlib installexample

uninstall: uninstalllib uninstallexample 


# ***********************************************************

gm0lib: gm0.o gm0_comms.o linuxcomms.o
	$(CC) -D_LINUX -shared -Wl,-soname,libgm.so.1 -lc -lm -lpthread -o libgm.so.1.0.0 gm0.o gm0_comms.o linuxcomms.o

gm0.o:
	 $(COMPILELIB) gm0.c -o gm0.o

gm0_comms.o:
	$(COMPILELIB) gm0_comms.c -o gm0_comms.o

linuxcomms.o:
	$(COMPILELIB) linuxcomms.c -o linuxcomms.o

installlib:
	install libgm.so.1.0.0 $(LIB_DIR)
	ln -s $(LIB_DIR)/libgm.so.1.0.0 $(LIB_DIR)/libgm.so
	ln -s $(LIB_DIR)/libgm.so.1.0.0 $(LIB_DIR)/libgm.so.1
	echo "You may need to run ldconfig on $(LIB_DIR)"
 
uninstalllib:
	rm $(LIB_DIR)/libgm.so.1.0.0
	rm $(LIB_DIR)/libgm.so
	rm $(LIB_DIR)/libgm.so.1
 
# *************** DLL TEST ***************************
 
dlltest:
	$(COMPILE) -D_LINUX dlltest.c -lgm -o dlltest

installexample:
	install dlltest $(EXE_DIR)/dlltest
 
uninstallexample:
	rm $(EXE_DIR)/dlltest
 

  