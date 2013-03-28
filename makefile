CC=gcc
FLAGS=-lm

all: layer
	rm -f bin/*.o

layer: bin/layer.o bin/nodes.o bin/polygons.o bin/draw.o bin/blocks.o
	$(CC) $(FLAGS) bin/layer.o bin/nodes.o bin/polygons.o bin/draw.o bin/blocks.o -o bin/layer

bin/layer.o: src/layer.c
	$(CC) -c src/layer.c -o bin/layer.o

bin/nodes.o: src/nodes.c
	$(CC) -c src/nodes.c -o bin/nodes.o

bin/polygons.o: src/polygons.c
	$(CC) -c src/polygons.c -o bin/polygons.o

bin/draw.o: src/draw.c
	$(CC) -c src/draw.c -o bin/draw.o

bin/blocks.o: src/blocks.c
	$(CC) -c src/blocks.c -o bin/blocks.o

install:
	

clean:
	rm -f bin/*