CC = gcc
COMPILE = $(CC) -c 
CFLAGS = -Wall -Wno-unused-result -pthread -O3

rsa: main.o bigint.o cipher.o sha256.o
	$(CC) $(CFLAGS) -o rsa main.o bigint.o cipher.o sha256.o

main.o: main.c bigint.h
	$(COMPILE) $(CFLAGS) main.c 

bigint.o: bigint.c bigint.h
	$(COMPILE) $(CFLAGS) bigint.c

cipher.o: cipher.c cipher.h
	$(COMPILE) $(CFLAGS) cipher.c

sha256.o: sha256.c sha256.h
	$(COMPILE) $(CFLAGS) sha256.c

clean:
	rm -f rsa *.o