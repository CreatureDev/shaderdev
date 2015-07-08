.c.o:
	$(CC) $(CFLG) -c $<
CC=gcc
GLFLG=`pkg-config gl --cflags` -I/usr/include/GL/
CFLG=-Wall -g `sdl2-config --cflags` $(GLFLG)
LFLG=`sdl2-config --libs` -lGL
OBJ=shadeutil.o
PRG=parsetest

all:$(PRG)

parsetest:parsetest.o $(OBJ)
	$(CC) $(CFLG) -o $@ $^ $(LFLG) ;\
	/bin/rm -f *.o

clean:
	/bin/rm -f *.o a.out $(PRG)
