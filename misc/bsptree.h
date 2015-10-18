#ifndef _QUADTREE_H__
#define _QUADTREE_H__

struct bsptree
{
	struct bsptree *p;
	struct bsptree *c[2];
	int pos[2];
};

struct bsptree *newbsp(int *ps);
void freebsp(struct bsptree *q);
void splitbsp(struct bsptree *q);

#endif /* _QUADTREE_H__ */

