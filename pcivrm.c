#include "pcivrm.h"
#include <CL/cl.h>
#include <CL/cl_ext.h>
#include <GL/gl.h>
#include "SDL.h"
#include "context.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

static struct pci_context ctx = {0};
static const void *mem = (void *) 0xc2370000;

#define ASSERT(x) void *j[2] = {&&ERR, &&GOOD}; goto *j[(x)];


int getdevinfo()
{
	unsigned int i, r;
	cl_device_id *dev;
	cl_device_topology_amd t;
	clGetContextInfo(ctx.clc, CL_CONTEXT_NUM_DEVICES, sizeof(i), &i, 0);
	r = i * sizeof(cl_device_id);
	dev = (cl_device_id *) alloca(r);
	clGetContextInfo(ctx.clc, CL_CONTEXT_DEVICES, r, dev, 0);
	
	
	
	return 0;	
}



#define MAP_FLG (MAP_ANONYMOUS | MAP_PRIVATE)

int pcivrm_init()
{
	ASSERT(mem == mmap((void *) mem, 0x10000, PROT_WRITE, MAP_FLG, 0, 0))
ERR:
	puts("failed to allocate desired memory location");
	return -1;
GOOD:
	return build_context(&ctx);
}

#undef ASSERT

#undef MAP_FLG

