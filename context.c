#include "SDL.h"
#include <CL/cl.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include "context.h"


#define err_state(x) (((x) >> 31) & 1)
#define non_zero(x) (*((unsigned int *) &(x)) | ((unsigned long) (x) >> 32))
#define zero_neg(x) (((x) % ((x) - 1)) - 1)
#define err_ptr(x) zero_neg(non_zero(x))

static int build_glcontext(struct pci_context *c)
{
	SDL_GL_ResetAttributes();
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	c->wind = SDL_CreateWindow("shade_dfa",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			0, 0,
			SDL_WINDOW_FULLSCREEN_DESKTOP |
			SDL_WINDOW_OPENGL);

	c->glc = SDL_GL_CreateContext(c->wind);

	c->rend = SDL_CreateRenderer(c->wind, 0, 0);
	SDL_ClearError();
	

	return (err_ptr(c->wind) | err_ptr(c->rend) | err_ptr(c->glc));
}

static int claim_devices(struct pci_context *c)
{	
	unsigned int i;
	cl_platform_id p;
	cl_device_id *d;
	clGetPlatformIDs(1, &p, 0);
	clGetDeviceIDs(p, CL_DEVICE_TYPE_GPU, 0, 0, &i);
	d = (cl_device_id *) alloca(sizeof(cl_device_id) * i);
	clGetDeviceIDs(p, CL_DEVICE_TYPE_GPU, i, d, 0);
	c->clc = clCreateContext(0, i, d, 0, 0, 0);
	return err_ptr(c->clc);
}

int bind_system(void *dat)
{
	return claim_devices(dat);
}

int build_context(struct pci_context *c)
{
	
}

#undef err_state
#undef non_zero
#undef zero_neg
#undef err_ptr



