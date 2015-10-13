.c.o:
	$(CC) $(CFLG) -c $<
CC=gcc
GLFLG=`pkg-config gl --cflags` -I/usr/include/GL/
CFLG=-Wall -g `sdl2-config --cflags` $(GLFLG)
LFLG=`sdl2-config --libs` -lGL -lSDL2_ttf -lOpenCL -lSDL2_image
OBJ=fsa_main.o
PRG=shader_fsa

all:$(PRG) done

shader_fsa:$(OBJ)
	$(CC) $(CFLG) -o $@ $^ $(LFLG)

done:
	/bin/rm -f *.o a.out

clean:
	/bin/rm -f *.o a.out $(PRG)

