#include "pcivrm.h"
#include <CL/cl.h>
#include <CL/cl_ext.h>
#include <GL/gl.h>
#include "SDL.h"
#include "context.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#define ADDR (0xc237ffff)

static struct pci_context ctx = {0};

static const void *mem = (void *) 0xc2370000;

#define ASSERT(x) void *j[2] = {&&ERR, &&GOOD}; goto *j[(x)];

int snipecode(const char *pth)
{
	
}

int loadcode()
{

}e

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

