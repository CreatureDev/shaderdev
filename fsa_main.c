#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <CL/cl.h>
#include <stdlib.h>
#include <stdio.h>

#define err_state(x) (((x) >> 31) & 1)
#define non_zero(x) (*((unsigned int *) &(x)) | ((unsigned long) (x) >> 32))
#define zero_neg(x) (((x) % ((x) - 1)) - 1)
#define err_ptr(x) zero_neg(non_zero(x))

static int m_st = 0;
static SDL_Window *wind = 0;
static SDL_Renderer *rend = 0;
static SDL_GLContext glc = 0;
static cl_context clc = 0;

static int build_glcontext()
{
	SDL_GL_ResetAttributes();
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	wind = SDL_CreateWindow("shade_dfa",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			0, 0,
			SDL_WINDOW_FULLSCREEN_DESKTOP |
			SDL_WINDOW_OPENGL);

	glc = SDL_GL_CreateContext(wind);

	rend = SDL_CreateRenderer(wind, 0, 0);
	SDL_ClearError();

	return (err_ptr(wind) | err_ptr(rend) | err_ptr(glc));
}

static int build_clcontext()
{	
	unsigned int i;
	cl_platform_id p;
	cl_device_id *d;
	clGetPlatformIDs(1, &p, 0);
	clGetDeviceIDs(p, CL_DEVICE_TYPE_GPU, 0, 0, &i);
	d = (cl_device_id *) alloca(sizeof(cl_device_id) * i);
	clGetDeviceIDs(p, CL_DEVICE_TYPE_GPU, i, d, 0);
	clc = clCreateContext(0, i, d, 0, 0, 0);
	return err_ptr(clc);
}

static int build_imcontext()
{
	int i = IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_JPG;
	return ~zero_neg(i - (IMG_Init(i) & i));
}

static int build_context()
{
	register int r;
	r = build_glcontext();
	r |= build_clcontext();
	r |= TTF_Init();
	r |= build_imcontext();
	return r;
}

int main(int argc, char **argv)
{
	void *jtb[2] = {&&RUN, &&ERR};
	m_st = err_state(SDL_Init(SDL_INIT_EVERYTHING));
	m_st |= err_state(build_context());
	goto *jtb[m_st];
RUN:
	puts("make this shit usable tbh");
	
	return 0;
ERR:
	puts(SDL_GetError());
	return -1;
}

