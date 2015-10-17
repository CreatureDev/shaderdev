#include "bsptree.h"
#include <stdlib.h>

struct bsptree *newbsp(int *ps, int *dm)
{
	struct bsptree *ret = calloc(1, sizeof(struct bsptree));
	ret->pos[0] = ps[0];
	ret->pos[1] = ps[1];
	ret->dim[0] = dm[0];
	ret->dim[1] = dm[1];
	return ret;
}

void freebsp(struct bsptree *q)
{
	q->p = 0;
	if(q->c[0])
	{
		freebsp(q->c[0]);
		freebsp(q->c[1]);
	}
	free(q);
}

void splitbsp(struct bsptree *q)
{
	
}

