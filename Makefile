.c.o:
	$(CC) $(CFLG) -c $<
CC=gcc
GLFLG=`pkg-config gl --cflags` -I/usr/include/GL/
CFLG=-Wall -g `sdl2-config --cflags` $(GLFLG)
LFLG=`sdl2-config --libs` -lGL -lSDL2_image
OBJ=shadeutil.o
PRG=test_shader

all:$(PRG)

test_shader:runshader.o $(OBJ)
	$(CC) $(CFLG) -o $@ $^ $(LFLG); /bin/rm -f *.o a.out


clean:
	/bin/rm -f *.o a.out $(PRG)
