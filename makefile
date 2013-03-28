CC=gcc
FLAGS=-lm

all: dirs main clean

main: bin/main.o bin/nodes.o bin/polygons.o bin/draw.o bin/blocks.o
	$(CC) $(FLAGS) bin/main.o bin/nodes.o bin/polygons.o bin/draw.o bin/blocks.o -o bin/main

bin/main.o: src/main.c
	$(CC) -c src/main.c -o bin/main.o

bin/nodes.o: src/nodes.c
	$(CC) -c src/nodes.c -o bin/nodes.o

bin/polygons.o: src/polygons.c
	$(CC) -c src/polygons.c -o bin/polygons.o

bin/draw.o: src/draw.c
	$(CC) -c src/draw.c -o bin/draw.o

bin/blocks.o: src/blocks.c
	$(CC) -c src/blocks.c -o bin/blocks.o

dirs:
	mkdir -p bin

installdirs:


install:
	

clean:
	rm -f bin/*.o