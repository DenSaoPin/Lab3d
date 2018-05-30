CC=g++
CFLAGS=-I./include
LDFLAGS=-lncurses

build: obj/lab3d.o obj/main.o obj/consoledrawer.o
	$(CC) -o bin/lab3d $^ $(LDFLAGS)

Debug: CFLAGS+=-g
Debug: build

cleanDebug: clean

obj/%.o: src/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f ./obj/*
	rm -f ./bin/*
