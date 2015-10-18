#ifndef _HASHLIB_H__
#define _HASHLIB_H__

typedef struct htable_s htable_t;
typedef struct hbukt_s hbukt_t;

struct htable_s
{
	hbukt_t **bukts;
	int bukcnt;
	int itmcnt;	
};

struct hbukt_s
{
	hbukt_t *next;
	char *key;
	unsigned int dat;
	unsigned int hval;
};

htable_t *htablecreate(unsigned int);
void htabledestroy(htable_t *);

unsigned int htablesearch(const char *, htable_t *);
void htableinsert(const char *, unsigned int, htable_t *);
unsigned int htableremove(const char *, htable_t *);

#endif

