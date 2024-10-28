CC = gcc
COMPILE = $(CC) -c 
CFLAGS = -Wall -Wno-unused-result -g

rsa: main.o bigint.o cipher.o
	echo "#define DEBUG 0" > config.h
	$(CC) $(CFLAGS) -o rsa main.o bigint.o cipher.o

debug: main.o bigint.o cipher.o
	echo "#define DEBUG 1" > config.h
	$(CC) $(CFLAGS) -o rsa main.o bigint.o cipher.o

main.o: main.c bigint.h
	$(COMPILE) $(CFLAGS) main.c 

bigint.o: bigint.c bigint.h
	$(COMPILE) $(CFLAGS) bigint.c

cipher.o: cipher.c cipher.h
	$(COMPILE) $(CFLAGS) cipher.c

clean:
	rm -f rsa *.o