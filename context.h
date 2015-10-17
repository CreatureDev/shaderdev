#ifndef _CONTEXT_H__
#define _CONTEXT_H__

struct pci_context
{
	SDL_Window *wind;
	SDL_Renderer *rend;
	SDL_GLContext glc;
	cl_context clc;
};

int build_context(struct pci_context *c);

#endif /* _CONTEXT_H__ */

