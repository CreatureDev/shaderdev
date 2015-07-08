#include "hashlib.h"
#include <stdlib.h>
#include <string.h>

struct hashtable *newhashtable(int s, unsigned int (*hf)(const void *), 
	int (*kcmp)(const void *, const void *), void (*kff)(void *), void (*dff)(void *))
{
	if(!(hf && kcmp))
		return 0;
	struct hashtable *ret;
	ret = (struct hashtable *) malloc(sizeof(struct hashtable));
	ret->size = 1;
	while(ret->size < s)
		ret->size <<= 1;
	s = ret->size;
	ret->buk = (struct hashbucket **) malloc(sizeof(struct hashbucket *) * s);
	for(s = 0; s < ret->size; s++)
		ret->buk[s] = 0;
	ret->hashfunc = hf;
	ret->cmpfunc = kcmp;
	ret->keyfreefunc = kff;
	ret->datfreefunc = dff;
	ret->cnt = 0;
	return ret;
}

void insertpairhashtable(struct hashtable *ht, void *key, void *dat)
{
	struct hashbucket *hb;
	unsigned int hval;
	if(!ht)
		return;
	hval = ht->hashfunc(key);
	hb = ht->buk[hval % ht->size];
	if(!hb)
	{
		hb = (struct hashbucket *) malloc(sizeof(struct hashbucket));
		hb->key = key;
		hb->dat = dat;
		hb->hash = hval;
		hb->rcnt = 1;
		hb->next = 0;
		ht->buk[hval % ht->size] = hb;
		ht->cnt++;
		return;
	}
	while(1)
	{
		if(hb->hash == hval && ht->cmpfunc(key, hb->key))
			return;
		if(!hb->next)
			break;
		hb = hb->next;
	}
	hb->next = (struct hashbucket *) malloc(sizeof(struct hashbucket));
	hb = hb->next;
	hb->key = key;
	hb->hash = hval;
	hb->dat = dat;
	hb->rcnt = 1;
	hb->next = 0;
	ht->cnt++;
}

void *getdathashtable(struct hashtable *ht, void *key)
{
	struct hashbucket *hb;
	unsigned int hval;
	if(!ht)
		return 0;
	hval = ht->hashfunc(key);
	hb = ht->buk[hval % ht->size];
	while(hb)
	{
		if(hb->hash == hval && ht->cmpfunc(key, hb->key))
			return hb->dat;
		hb = hb->next;
	}
	return 0;
}

void *getdataddrefhashtable(struct hashtable *ht, void *key)
{
	struct hashbucket *hb;
	unsigned int hval;
	if(!ht)
		return 0;
	hval = ht->hashfunc(key);
	hb = ht->buk[hval % ht->size];
	while(hb)
	{
		if(hb->hash == hval && ht->cmpfunc(key, hb->key))
		{
			hb->rcnt++;
			return hb->dat;
		}
		hb = hb->next;
	}
	return 0;
}

void removerefhashtable(struct hashtable *ht, void *key)
{
	struct hashbucket *hb;
	unsigned int hval;
	if(!ht)
		return;
	hval = ht->hashfunc(key);
	hb = ht->buk[hval % ht->size];
	while(hb)
	{
		if(hb->hash == hval && ht->cmpfunc(key, hb->key))
		{	
			if(!(--hb->rcnt))
				removepairhashtable(ht, key);
			return;
		}
		hb = hb->next;
	}
	
}

static void freehashbucket(struct hashtable *ht, struct hashbucket *hb)
{
	if(ht->keyfreefunc)
		ht->keyfreefunc(hb->key);
	if(ht->datfreefunc)
		ht->datfreefunc(hb->dat);
	free(hb);
}

void removepairhashtable(struct hashtable *ht, void *key)
{
	struct hashbucket *hb, *prv;
	unsigned int hval;
	if(!ht)
		return;
	hval = ht->hashfunc(key);
	hb = ht->buk[hval % ht->size];
	if(!hb)
		return;
	if(hb->hash == hval && ht->cmpfunc(key, hb->key))
	{
		ht->buk[hval % ht->size] = hb->next;
		ht->cnt--;
		freehashbucket(ht, hb);
		return;
	}
	prv = hb;
	hb = hb->next;
	while(hb)
	{
		if(hb->hash == hval && ht->cmpfunc(key, hb->key))
		{
			prv->next = hb->next;
			ht->cnt--;
			freehashbucket(ht, hb);
			return;
		}
		prv = hb;
		hb = hb->next;
	}
}

static void freehashbucketlist(struct hashtable *ht, struct hashbucket *hb)
{
	if(hb->next)
		freehashbucketlist(ht, hb->next);
	if(ht->keyfreefunc)
		ht->keyfreefunc(hb->key);
	if(ht->datfreefunc)
		ht->datfreefunc(hb->dat);
	free(hb);
}

void freehashtable(void *htp)
{
	int i;
	struct hashtable *ht = (struct hashtable *) htp;
	if(!ht)
		return;
	
	for(i = 0; i < ht->size; i++)
		if(ht->buk[i])
			freehashbucketlist(ht, ht->buk[i]);
	
	free(ht->buk);
	free(ht);
}

unsigned int stringhash(const void *spt)
{
	unsigned int i;
	const char *str = (const char *) spt;
	for(i = 0; *str; ++str)
	{
		i *= 0x1000193;
		i ^= *str;
	}
	return i;
}

int stringcmp(const void *s1, const void *s2)
{
	return (strcmp((const char *) s1, (const char *) s2) == 0);
}

