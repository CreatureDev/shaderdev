#ifndef __HASHLIB_H__
#define __HASHLIB_H__

struct hashbucket
{
	struct hashbucket *next;
	void *key;
	unsigned int hash;
	void *dat;
	int rcnt;
};

struct hashtable
{
	int size;
	int cnt;
	struct hashbucket **buk;

	unsigned int (*hashfunc)(const void *);
	int (*cmpfunc)(const void *, const void *);
	void (*keyfreefunc)(void *);
	void (*datfreefunc)(void *);
};

struct hashtable *newhashtable(int size, 
				unsigned int (*hashfunc)(const void *),
				int (*cmpfunc)(const void *, const void *), 
				void (*keyfreefunc)(void *), 
				void (*datafreefunc)(void *));

void insertpairhashtable(struct hashtable *htbl, void *key, void *dat);
void removepairhashtable(struct hashtable *htbl, void *key);
void *getdathashtable(struct hashtable *htbl, void *key);
void *getdataddrefhashtable(struct hashtable *htbl, void *key);
void removerefhashtable(struct hashtable *htbl, void *key);
void freehashtable(void *ht);

unsigned int stringhash(const void *str);
int stringcmp(const void *str1, const void *str2);

#endif /* __HASHLIB_H__ */


