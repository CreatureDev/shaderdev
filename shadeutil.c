#include "SDL_opengl.h"
#include "shadeutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

int SH_init()
{
	sclk = clock();
	return 0;
}

int SH_parsedescline(void **ret, const char *str)
{
	char *ln, *tmp;
	struct shval *ret;
	if(!strlen(str))
		return -1;
	ln = (char *) malloc(strlen(str) + 1);
	strcpy(ln, str);
	ret = (struct shval *) malloc(sizeof(struct shval));
	tmp = strtok(ln, " ");
	if(!tmp)
	{
		free(ret);
		return -1;
	}
	if(!(ret->type = SH_valtype(tmp)))
	{
		free(ret);
		return -1;
	}
	
	free(ln);
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
		if(strlen(buff) < 10)
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
	
	
}

static void SH_freeshval(struct shval *ded)
{
	if(ded->next)
		SH_freeshval(ded->next);
	if(ded->dat)
		free(ded->dat);
	free(ded);

}

int SH_freedesc(void *shd)
{
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

