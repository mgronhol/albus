CC=gcc
FLAGS=-Os -Wall -pedantic
LIBS=libalshainbus.o libabustcp.o

all: abustcp abus test-client test-master

abustcp:
	$(CC) $(FLAGS) libAbusTCP.c -c -o libabustcp.o

abus:
	$(CC) $(FLAGS) libAlshainbus.c -c -o libalshainbus.o

test-client: abustcp abus
	$(CC) $(FLAGS) test-client.c -o test-c $(LIBS)
	
test-master: abustcp abus
	$(CC) $(FLAGS) test-master.c -o test-m $(LIBS)
