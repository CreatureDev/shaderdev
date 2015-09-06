#ifndef __SHADEUTIL2_H__
#define __SHADEUTIL2_H__

struct dlist
{
	unsigned int id;
	char *name;
	void *dat;
	struct dlist *next;
};

struct shaderdesc
{
	char *name;
	struct dlist *al;
	struct dlist *tl;
	struct dlist *ul;
	struct dlist *bl;
};

#endif /* __SHADEUTIL2_H__ */

