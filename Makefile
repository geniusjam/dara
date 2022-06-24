CC=g++

all: dara

dara: dara.cpp
	$(CC) -Wall dara.cpp -o dara


clean:
	rm dara