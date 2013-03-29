SHELL=/bin/sh
CC=gcc
CFLAGS=-lm 
ALL_CFLAGS=-I. $(CFLAGS)
INSTALL=install
INSTALL_PROGRAM=$(INSTALL)
INSTALL_DATA=${INSTALL} -m 644

all: dirs main clean

main: bin/main.o bin/nodes.o bin/polygons.o bin/draw.o bin/blocks.o bin/labels.o
	$(CC) $(ALL_CFLAGS) bin/main.o bin/nodes.o bin/polygons.o bin/draw.o bin/blocks.o bin/labels.o -o spatialplot

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

bin/labels.o: src/labels.c
	$(CC) -c src/labels.c -o bin/labels.o

dirs:
	mkdir -p bin
	mkdir -p features
	mkdir -p grid

installdirs:
	

install:
	

clean:
	rm -f bin/*.o