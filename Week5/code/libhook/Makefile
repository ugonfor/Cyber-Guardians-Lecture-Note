RM 	= rm 
CC 	= gcc

All:
	$(CC) -c -fPIC  hook.c 
	$(CC) -shared hook.o -o libhook.so -ldl
	$(CC) -o test test.c
clean:
	$(RM) libhook.so hook.o test