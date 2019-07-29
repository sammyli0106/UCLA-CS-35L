# makefile that include randmain.mk that builds program randmain
# First: use static linking to combine randmain.o and randcpuid.o into randmain
# Second: use dynamic linking to link the C library necessary files before main called
# Third: after main is called, use dynamic link to compile the c files with options

# include instructions for building
# randmain, randlibhw.so, and randlibsw.so.

randlibhw.so: randlibhw.c
	$(CC) $(CFLAGS) -fPIC randlibhw.c -shared -o randlibhw.so

randlibsw.so: randlibsw.c
	$(CC) $(CFLAGS) -fPIC randlibsw.c -shared -o randlibsw.so

randmain: randmain.c randcpuid.c
	$(CC) $(CFLAGS) -ldl -Wl,-rpath=$(PWD) randmain.c randcpuid.c -o randmain




