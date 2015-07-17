#ifndef __CIBUTIL_H__
#define __CIBUTIL_H__

struct texture;

struct texture *loadcib(const char *);

void freetexture(struct texture *);

#endif

