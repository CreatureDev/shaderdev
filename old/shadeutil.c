#include "SDL_opengl.h"
#include "shadeutil.h"
#include "hashlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TOK_CNT 12
#define DELIM "\t "

typedef int (*shvalfun)(void **ret, int argc, char *argv[]);

struct shdesc
{
	unsigned int pid;
	unsigned int *ddat;
	void (*drawfun)(unsigned int *);
	struct shval *val;
};

struct shval
{
	unsigned int type;
	unsigned int vid;
	void *dat;
	struct shval *next;
};

static clock_t sclk = 0;

static struct hashtable *shtbl = 0;

static char *shvalstr[5] = 
{
	"VertexPos",
	"GlobalTime",
	"Value",
	"Model",
	"Texture"
};

static shvalfun SH_modelshval(void **ret, int argc, char *argv[])
{

	return 0;
}

static shvalfun SH_valueshval(void **ret, int argc, char *argv[])
{

	return 0;
}

static shvalfun SH_vertposshval(void **ret, int argc, char *argv[])
{

	return 0;
}

static shvalfun SH_textureshval(void **ret, int argc, char *argv[])
{

	return 0;
}

static shvalfun SH_globaltimeshval(void **ret, int argc, char *argv[])
{

	return 0;
}

static void SH_populateshtbl()
{
	insertpairhashtable(shtbl, (void *) shvalstr[0], (void *) SH_vertposshval);
	insertpairhashtable(shtbl, (void *) shvalstr[1], (void *) SH_globaltimeshval);
	insertpairhashtable(shtbl, (void *) shvalstr[2], (void *) SH_valueshval);
	insertpairhashtable(shtbl, (void *) shvalstr[3], (void *) SH_modelshval);
	insertpairhashtable(shtbl, (void *) shvalstr[4], (void *) SH_textureshval);
}

int SH_init()
{
	if(shtbl)
		return 0;
	
	sclk = clock();
	
	shtbl = newhashtable(64, stringhash, stringcmp, 0, 0);
	if(!shtbl)
		return -1;
	
	SH_populateshtbl();
	
	return 0;
}

int SH_parsedescline(void **ret, const char *str)
{
	char *ln, *tok[MAX_TOK_CNT], *tmp;
	unsigned int tcnt = 0;
	int rval;
	shvalfun fnc;
	if(!strlen(str))
		return -1;
	ln = (char *) malloc(strlen(str) + 1);
	strcpy(ln, str);
	tmp = strtok(ln, DELIM);
	while(tmp && tcnt < MAX_TOK_CNT)
	{
		if(!strlen(tmp))
		{
			tmp = strtok(0, DELIM);
			continue;
		}
		tok[tcnt++] = tmp;
		tmp = strtok(0, DELIM);
	}
	if(tcnt < 2)
	{
		printf("incomplete descriptor line: %s\n", str);
		free(ln);
		return -1;
	}
	
	fnc = (shvalfun) getdathashtable(shtbl, tok[0]);
	if(!fnc)
	{
		printf("unknown descriptor type %s\n", tok[0]);
		free(ln);
		return -1;
	}

	rval = fnc(ret, tcnt, tok);
	free(ln);
	return rval;
}

int SH_parsedesc(void **ret, const char *fnam)
{
	int lcnt;
	ret = 0;
	struct shdesc *dsc;
	struct shval *shv;
	char buff[1024];
	FILE *fl = fopen(fnam, "r");
	if(!fl)
	{
		printf("failed to open shader descriptor file %s\n", fnam);
		return -1;
	}	
	
	dsc = (struct shdesc *) malloc(sizeof(struct shdesc));
	dsc->pid = 0;
	dsc->ddat = 0;
	dsc->drawfun = 0;
	dsc->val = 0;
	lcnt = 0;
	while(fgets(buff, 1024, fl))
	{
		lcnt++;
		if(strlen(buff) < 6)
			continue;
		if(SH_parsedescline((void **) &shv, buff))
		{
			printf("failed to parse %s line %d\n", fnam, lcnt);
			SH_freedesc((void *) dsc);
			return -1;
		}
	}
	

	fclose(fl);
	return 0;
}

int SH_printdesc(void *shd)
{
	struct shdesc *dsc = (struct shdesc *) shd;
	
	return 0;
}

static void SH_freeshval(struct shval *ded)
{
	if(ded->next)
		SH_freeshval(ded->next);
	if(ded->dat)
		free(ded->dat);
	free(ded);

}

int SH_freedesc(void *shdp)
{
	struct shdesc *shd = (struct shdesc *) shdp;
	if(!shd)
		return -1;
	if(shd->val)
		SH_freeshval(shd->val);
	shd->drawfun = 0;
	if(shd->ddat)
		free(shd->ddat);
	if(shd->pid)
		glDeleteProgram(shd->pid);
	free(shd);
	return 0;
}

