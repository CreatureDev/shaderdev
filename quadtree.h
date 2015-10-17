#ifndef _QUADTREE_H__
#define _QUADTREE_H__

struct quadtree
{
	struct quadtree *p;
	struct quadtree *c[4];
	int pos[2];
	int dim[2];
};

struct quadtree *newqtree(int *ps, int *dm);
void freeqtree(struct quadtree *q);
void splitqtree(struct quadtree *q);

#endif /* _QUADTREE_H__ */

