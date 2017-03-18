
randmain: randmain.c randcpuid.c
	$(CC) $(CFLAGS) -c randmain.c -o randmain.o 
	$(CC) $(CFLAGS) -c randcpuid.c -o randcpuid.o 
	$(CC) $(CFLAGS) randmain.o randcpuid.o -ldl -Wl,-rpath=$(PWD) -o randmain

randlibhw.so: randlibhw.c
	$(CC) $(CFLAGS) -fPIC -c randlibhw.c -o randlibhw.o
	$(CC) $(CFLAGS) -shared -o randlibhw.so randlibhw.o	

randlibsw.so: randlibsw.c
	$(CC) $(CFLAGS) -fPIC -c randlibsw.c -o randlibsw.o
	$(CC) $(CFLAGS) -shared -o randlibsw.so randlibsw.o