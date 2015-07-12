#include "SDL.h"
#include "SDL_opengl.h"
#include "shadeutil.h"
#include <stdio.h>

#define CHECKTYPE(x) if(!shades.x) {printf("%s shader component not found!\n", #x); exit(-1);}

#define NULLTYPE 0
#define FRAGTYPE 1
#define VERTTYPE 2
#define DESCTYPE 3
#define TCTLTYPE 4
#define TEVLTYPE 5
#define GEOMTYPE 6
#define COMPTYPE 7

static SDL_Window *wind = 0;
static SDL_GLContext glct = 0;

static struct 
{
	char *comp;
	char *desc;
	char *frag;
	char *geom;
	char *tctl;
	char *tevl;
	char *vert;
} shades = {0};

void initwindow(const char *shnm)
{
	int i, j;
	char buff[1024];

	sprintf(buff, "shader test: %s", shnm);

	i = 4;
	j = 5;

	while(i)
	{
		if(wind)
		{
			SDL_DestroyWindow(wind);
			wind = 0;
		}
		SDL_GL_ResetAttributes();
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, i);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, j);
		wind = SDL_CreateWindow(buff,
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				0, 0,
				SDL_WINDOW_FULLSCREEN_DESKTOP |
				SDL_WINDOW_OPENGL);

		glct = SDL_GL_CreateContext(wind);

		if(glct)
			break;
		if(!j)
		{
			i--;
			j = 5;
		}
		else
		{
			j--;
		}
	}
}

static int dsttype(const char *str)
{
	if(!strcmp(str, "frag"))
		return FRAGTYPE;
	if(!strcmp(str, "vert"))
		return VERTTYPE;
	if(!strcmp(str, "comp"))
		return COMPTYPE;
	if(!strcmp(str, "desc"))
		return DESCTYPE;
	if(!strcmp(str, "geom"))
		return GEOMTYPE;
	if(!strcmp(str, "tctl"))
		return TCTLTYPE;
	if(!strcmp(str, "tevl"))
		return TEVLTYPE;
	return NULLTYPE;
}

static void popshades(const char *fsys)
{
	FILE *ls;
	char buff[1024], *tmp, **dst;
	int i;
	sprintf(buff, "ls %s.*", fsys);
	ls = popen(buff, "r");
	
	if(!ls)
	{
		printf("failed to view the file system!\n");
		exit(-1);
	}
	while(fgets(buff, 1024, ls))
	{
		i = strlen(buff) - 1;
		if(buff[i] == '\n')
			buff[i--] = '\0';
		tmp = buff + (i - 4);
		if(*tmp != '.')
		{	
			printf("What is this file?? %s\n", buff);
			exit(-1);
		}
		tmp++;
		switch(dsttype(tmp))
		{
		case FRAGTYPE:
			dst = &shades.frag;
			break;
		case VERTTYPE:
			dst = &shades.vert;
			break;
		case COMPTYPE:
			dst = &shades.comp;
			break;
		case DESCTYPE:
			dst = &shades.desc;
			break;
		case TCTLTYPE:
			dst = &shades.tctl;
			break;
		case TEVLTYPE:
			dst = &shades.tevl;
			break;
		case GEOMTYPE:
			dst = &shades.geom;
			break;
		default:
		case NULLTYPE:
			dst = 0;
		}
		if(!dst)
			continue;
		if(*dst)
		{
			printf("conflicting files %s and %s found!\n", buff, *dst);
			exit(-1);
		}
		*dst = (char *) malloc(i + 2);
		strcpy(*dst, buff);
	}
	pclose(ls);
	CHECKTYPE(frag)
	CHECKTYPE(vert)
	CHECKTYPE(desc)
}

static void puse(const char *prg)
{
	printf("usage\n");
	printf("%s [filesytsem]\n", prg);
}

int main(int argc, char **argv)
{	
	if(argc != 2)
	{
		puse(argv[0]);
		return -1;
	}
	
	popshades(argv[1]);
	
	getshaderdat(shades.desc);
	
	if(SDL_Init(SDL_INIT_VIDEO))
	{
		printf("failed to initialize SDL\n");
		return -1;
	}	

	initwindow(argv[1]);

	glClearColor(0.f, 0.f, 0.f, 1.f);



	for(argc = 0; argc < 100; argc++)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		SDL_GL_SwapWindow(wind);
	}	
	
	SDL_Quit();

	return 0;
}

