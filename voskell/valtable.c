#include "valtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int inthash(unsigned int a)
{
	a = (a+0x7ed55d16) + (a<<12);
	a = (a^0xc761c23c) ^ (a>>19);
	a = (a+0x165667b1) + (a<<5);
	a = (a+0xd3a2646c) ^ (a<<9);
	a = (a+0xfd7046c5) + (a<<3);
	a = (a^0xb55a4f09) ^ (a>>16);
	return a;
}

void insertvaltable(struct valtable *vt, unsigned int in, const char *nm)
{
	struct valdesc *new, *col;
	new = malloc(sizeof(struct valdesc));
	new->id = in;
	new->hash = inthash(in);
	col = vt->vals[new->hash % vt->size];
	if(!col)
	{
		vt->vals[new->hash % vt->size] = new;
		new->next = 0;
		new->type = malloc(strlen(nm) + 1);
		strcpy(new->type, nm);
		return;
	}
	if(col->id == new->id)
	{
		free(new);
		return;
	}
	while(col->next)
	{
		col = col->next;
		if(col->id == new->id)
		{
			free(new);
			return;
		}
	}
	col->next = new;
	new->next = 0;
	new->type = malloc(strlen(nm) + 1);
	strcoy(new->type, nm);
}

