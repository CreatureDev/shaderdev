#include "SDL.h"
#include "shadeutil.h"
#include "gl.h"
#include "glext.h"
#include <stdio.h>

unsigned int glCreateShader(GLenum);
void glShaderSource(unsigned int, unsigned int, const char **, const int *);
void glCompileShader(unsigned int);
void glGetShaderiv(unsigned int, GLenum, int *);
void glGetShaderInfoLog(unsigned int, unsigned int, unsigned int *, char *);
void glAttachShader(unsigned int, unsigned int);
void glDeleteShader(unsigned int);
unsigned int glCreateProgram();
void glLinkProgram(unsigned int);
void glGetProgramiv(unsigned int, GLenum, int *);
void glGetProgramInfoLog(unsigned int, unsigned int, unsigned int *, char *);
void glUseProgram(unsigned int);
void glGenVertexArrays(unsigned int, unsigned int *);
void glBindVertexArray(unsigned int);
void glDeleteProgram(unsigned int);
void glDisable(GLenum);
void glEnable(GLenum);
void glCullFace(GLenum);
void glDepthFunc(GLenum);

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

static unsigned int glp;
static unsigned int vao;

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

static void compileattach(const char *shn, unsigned int gt)
{
	FILE *fl;
	char *src;
	unsigned int sid;
	int st;
	long l;
	fl = fopen(shn, "r");
	if(!fl)
	{
		printf("failed to open shader %s\n", shn);
		exit(-1);
	}
	fseek(fl, 0, SEEK_END);
	l = ftell(fl);
	fseek(fl, 0, SEEK_SET);
	src = (char *) calloc(l+1, 1);
	fread(src, 1, l, fl);
	fclose(fl);
	sid = glCreateShader(gt);
	glShaderSource(sid, 1, (const char **) &src, 0);
	glCompileShader(sid);	

	glGetShaderiv(sid, GL_COMPILE_STATUS, &st);
	if(!st)
	{
		printf("failed to compile shader %s\n", shn);
		glGetShaderInfoLog(sid, l, 0, src);
		printf("%s", src);
		exit(-1);
	}
	glAttachShader(glp, sid);

	glDeleteShader(sid);

	free(src);
}

static void makeprogram()
{
	char buff[2048];
	int st;
	glp = glCreateProgram();
	if(shades.comp)
		compileattach(shades.comp, GL_COMPUTE_SHADER);
	if(shades.frag)
		compileattach(shades.frag, GL_FRAGMENT_SHADER);
	if(shades.geom)
		compileattach(shades.geom, GL_GEOMETRY_SHADER);
	if(shades.tctl)
		compileattach(shades.tctl, GL_TESS_CONTROL_SHADER);
	if(shades.tevl)
		compileattach(shades.tevl, GL_TESS_EVALUATION_SHADER);
	if(shades.vert)
		compileattach(shades.vert, GL_VERTEX_SHADER);
	
	glLinkProgram(glp);
	glGetProgramiv(glp, GL_LINK_STATUS, &st);
	if(!st)
	{
		printf("failed to link shader program\n");
		glGetProgramInfoLog(glp, 2048, 0, buff);
		printf("%s", buff);
		exit(-1);
	}
	glUseProgram(glp);
}

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
	if(!i)
	{
		printf("Failed to create an OpenGL context\n");
		exit(-1);
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
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	

	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(wind);
	
	makeprogram();
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	bindshaderdat(glp);

	SDL_GL_SetSwapInterval(1);	


	
	SDL_Event e;
	for(;;) 
	{
		while(SDL_PollEvent(&e))
			if(e.type == SDL_QUIT) 
				goto QUITIT;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		updateshader(glp);
		rendershader();
		glFinish();
		SDL_GL_SwapWindow(wind);
	}


QUITIT:
	glDeleteProgram(glp);
	freedesc();
	SDL_GL_DeleteContext(glct);
	
	SDL_Quit();

	return 0;
}

