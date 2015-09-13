#ifndef __VALTABLE_H__
#define __VALTABLE_H__

struct valdesc
{
	unsigned int id;
	unsigned int hash;
	char *type;
	void *dflt;
	struct valdesc *next;
}

struct valtable
{
	unsigned int size;
	struct valdesc **vals;
};

void insertvaltable(struct valtable *, unsigned int, const char *);


#endif /* __VALTABLE_H__ */

