#include "hashlib.h"

#define DEFLT_TBL_SIZE 32
#include <stdlib.h>
#include <string.h>



unsigned int hashstr(const char *str)
{
	unsigned int i;
	for(i = 0; *str; str++)
	{
		i *= 16777619;
		i ^= *str;
	}
	return i;
}

htable_t *htablecreate(unsigned int bcnt)
{
	htable_t *new;
	int i;
	new = (htable_t *) malloc(sizeof(htable_t));
	if(!bcnt)
		bcnt = DEFLT_TBL_SIZE;
	else
	{
		for(i = 1; i < bcnt; i = i << 1);
		bcnt = i;
	}
	new->bukts = (hbukt_t **) malloc(sizeof(hbukt_t *) * bcnt);
	new->bukcnt = bcnt;
	new->itmcnt = 0;
	for(i = 0; i < bcnt; i++)
		new->bukts[i] = (hbukt_t *) '\0';
	return new;
}

unsigned int htablesearch(const char *str, htable_t *tbl)
{
	unsigned int hv, bid;
	hbukt_t *buk;
	hv = hashstr(str);
	bid = hv & (tbl->bukcnt - 1);
	buk = tbl->bukts[bid];
	while(buk)
	{
		if(hv == buk->hval && !strcmp(buk->key, str))
			return buk->dat;
		buk = buk->next;
	}
	return 0;
}

void htableinsert(const char *str, unsigned int dat, htable_t *tbl)
{
	if(!tbl || htablesearch(str, tbl) || !dat)
		return;
	hbukt_t *buk;
	unsigned int hv, bid;
	buk = (hbukt_t *) malloc(sizeof(hbukt_t));
	buk->key = malloc(strlen(str) + 1);
	strcpy(buk->key, str);
	hv = hashstr(str);
	bid = hv & (tbl->bukcnt - 1);
	buk->hval = hv;
	buk->dat = dat;
	buk->next = tbl->bukts[bid];
	tbl->bukts[bid] = buk;
	tbl->itmcnt++;
}

unsigned int htableremove(const char *str, htable_t *tbl)
{
	hbukt_t *buk, *prv;
	unsigned int hv, bid;
	unsigned int ret;
	if(!(tbl && tbl->itmcnt))
		return 0;
	hv = hashstr(str);
	bid = hv & (tbl->bukcnt - 1);
	for(buk = tbl->bukts[bid]; buk; buk = buk->next)
	{
		if(hv == buk->hval && !strcmp(buk->key, str))
		{
			if(prv)
				prv->next = buk->next;
			else
				tbl->bukts[bid] = buk->next;
			tbl->itmcnt--;
			ret = buk->dat;
			free(buk->key);
			free(buk);
			return ret;
		}
		prv = buk;
	}
	return 0;
}

void htabledestroy(htable_t *tbl)
{
	int i;
	hbukt_t *buk, *nxt;
	for(i = 0; i < tbl->bukcnt; i++)
	{
		buk = tbl->bukts[i];
		while(buk)
		{
			nxt = buk->next;
			free(buk->key);
			free(buk);
			buk = nxt;
		}
	}
	free(tbl->bukts);
	free(tbl);
}

