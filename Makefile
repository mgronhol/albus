CC=gcc
FLAGS=-Os -Wall -pedantic
LIBS=libalshainbus.o libabustcp.o libabusutils.o

all: abustcp abus abusloop test-client test-master test-client-loop

abustcp:
	$(CC) $(FLAGS) libAbusTCP.c -c -o libabustcp.o

abus:
	$(CC) $(FLAGS) libAlshainbus.c -c -o libalshainbus.o

abusloop:
	$(CC) $(FLAGS) libAbusUtils.c -c -o libabusutils.o

test-client: abustcp abus
	$(CC) $(FLAGS) test-client.c -o test-c $(LIBS)
	
test-master: abustcp abus
	$(CC) $(FLAGS) test-master.c -o test-m $(LIBS)

test-client-loop: abustcp abus abusloop
	$(CC) $(FLAGS) test-client-loop.c -o test-c-loop $(LIBS)
