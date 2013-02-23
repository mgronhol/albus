CC=gcc
FLAGS=-Os -Wall -pedantic
LIBS=libalshainbus.o libalbustcp.o libalbusutils.o

all: albustcp albus albusloop test-client test-master test-client-loop

albustcp:
	$(CC) $(FLAGS) libAlbusTCP.c -c -o libalbustcp.o

albus:
	$(CC) $(FLAGS) libAlshainbus.c -c -o libalshainbus.o

albusloop:
	$(CC) $(FLAGS) libAlbusUtils.c -c -o libalbusutils.o

test-client: albustcp albus
	$(CC) $(FLAGS) test-client.c -o test-c $(LIBS)
	
test-master: albustcp albus
	$(CC) $(FLAGS) test-master.c -o test-m $(LIBS)

test-client-loop: albustcp albus albusloop
	$(CC) $(FLAGS) test-client-loop.c -o test-c-loop $(LIBS)

clean:
	rm *.o
	rm test-c
	rm test-c-loop
	rm test-m
	rm *.pyc
	
