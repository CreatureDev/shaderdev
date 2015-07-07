.c.o:
  $(CC) $(CFLG) -c $<
CC=gcc
CFLG=-Wall -g `sdl2-config --cflags` `pkg-config gl --cflags`
LFLG=`sdl2-config --libs` -lGL
OBJ=
PRG=shadeparser

all:$(PRG)

shadeparser:shadeparser.o
  $(CC) $(CFLG) -o $@ $^ $(LFLG);\
  /bin/rm -f *.o

clean:
  /bin/rm -f *.o a.out $(PRG)
