#include "shadeutil.h"
#include "gl.h"
#include "glext.h"
#include "SDL_image.h"
#include "SDL_surface.h"
#include "SDL_render.h"
#include "SDL_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int glGetUniformLocation(unsigned int, const char *);
void glUniform1f(int, float);
void glUniform1i(int, int);
void glUniform2f(int, float, float);
void glUniform3f(int, float, float, float);
void glUniform4f(int, float, float, float, float);
void glUniformMatrix2fv(int, unsigned int, GLboolean, const void *);
void glUniformMatrix3fv(int, unsigned int, GLboolean, const void *);
void glUniformMatrix4fv(int, unsigned int, GLboolean, const void *);
void glGenBuffers(unsigned int, unsigned int *);
void glBindBuffer(GLenum, unsigned int);
void glBufferData(GLenum, unsigned int, const void *, GLenum);
int glGetAttribLocation(unsigned int, const char *);
void glEnableVertexAttribArray(unsigned int);
void glBufferSubData(GLenum, unsigned int, unsigned int, const void *);
void glVertexAttribPointer(unsigned int, int, GLenum, GLboolean, unsigned int, const void *);
void glDeleteBuffers(unsigned int, unsigned int *);

#define GLTEXTURE(x) (GL_TEXTURE0 + x)

/*	special snowflake
 *		Texture
 *
 *	ATTRIB	
 *		VertPos
 *		TexCoord
 *		Normal
 *
 *	VALUE
 *		GlobalTime
 *		Int
 *		Float
 *		Vec(2, 3, 4)
 *		Mat(2, 3, 4)
*/

#define ATTRIBMASK 0xff
#define VALUEMASK 0xff00
#define TEXTURE 0x10000

#define VertPosTYPE 0x01
#define TexCoordTYPE 0x02
#define NormalTYPE 0x03

#define GlobalTimeTYPE 0x0100
#define IntTYPE 0x0200
#define FloatTYPE 0x0300
#define Vec2TYPE 0x0400
#define Vec3TYPE 0x0500
#define Vec4TYPE 0x0600
#define Mat2TYPE 0x0800
#define Mat3TYPE 0x0900
#define Mat4TYPE 0x0a00
#define ProjMatTYPE 0x1000

#define TextureTYPE TEXTURE

#define strtype(x) if(!strcmp(str, #x)) return x ## TYPE;

struct cmbh
{
	unsigned int vers;
	unsigned int vcnt;
	unsigned int fcnt;
};

struct alist
{
	unsigned int type;
	char *name;
	void *dat;
	struct alist *next;
};

struct tlist
{
	unsigned int bind;
	unsigned int glid;
	char *file;
	struct tlist *next;
};

struct ulist
{
	unsigned int type;
	char *name;
	void *dat;
	struct ulist *next;
};

static struct
{
	unsigned int vcnt;
	unsigned int ecnt;
	
	unsigned int *ent;

	struct alist *al;

	struct tlist *tl;
	
	struct ulist *ul;
} datdef = {0};

void updateshader(unsigned int gid)
{
	struct ulist *uni = datdef.ul;
	float fval;
	unsigned int ind;
	while(uni)
	{
		if(uni->type == GlobalTimeTYPE)
		{
			fval = clock() - *((clock_t *) uni->dat);
			fval /= CLOCKS_PER_SEC;
			ind = glGetUniformLocation(gid, uni->name);
			glUniform1f(ind, fval);
		}
		uni = uni->next;
	}
}

void rendershader()
{
	glDrawElements(GL_TRIANGLES, datdef.ecnt, GL_UNSIGNED_INT, 0);
	glFinish();
}

static unsigned int bufs[2];

void freealist(struct alist *dl)
{
	if(!dl)
		return;
	freealist(dl->next);
	free(dl->name);
	free(dl->dat);
	free(dl);
}

void freetlist(struct tlist *dl)
{
	if(!dl)
		return;
	freetlist(dl->next);
	free(dl->file);
	free(dl);
}

void freeulist(struct ulist *dl)
{
	if(!dl)
		return;
	freeulist(dl->next);
	free(dl->name);
	free(dl->dat);
	free(dl);
}

void freedesc()
{
	glDeleteBuffers(2, bufs);
	freealist(datdef.al);
	freetlist(datdef.tl);
	freeulist(datdef.ul);
	datdef.al = 0;
	datdef.tl = 0;
	datdef.ul = 0;
}

void bindshaderdat(unsigned int gid, void *rnd)
{
	struct alist *atr;
	struct tlist *tex;
	struct ulist *uni;
	unsigned long ind, off, len;
	SDL_Surface *img;
	SDL_Texture *txt;
	float *fp, fval;
	int ival;
	atr = datdef.al;
	glGenBuffers(2, bufs);
	glBindBuffer(GL_ARRAY_BUFFER, bufs[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * datdef.vcnt, 0, GL_DYNAMIC_DRAW);
	while(atr)
	{
		ind = glGetAttribLocation(gid, atr->name);
		glEnableVertexAttribArray(ind);
		switch(atr->type)
		{
		case TexCoordTYPE:
			off = sizeof(float) * 3 * datdef.vcnt;
			len = sizeof(float) * 2 * datdef.vcnt;
			glBufferSubData(GL_ARRAY_BUFFER, off, len, atr->dat);
			glVertexAttribPointer(ind, 2, GL_FLOAT, 0, 0, (void *) off);
			break;
		case VertPosTYPE:
			off = 0;
			len = sizeof(float) * 3 * datdef.vcnt;
			glBufferSubData(GL_ARRAY_BUFFER, off, len, atr->dat);
			glVertexAttribPointer(ind, 3, GL_FLOAT, 0, 0, (void *) off);
			break;
		case NormalTYPE:
			off = sizeof(float) * 5 * datdef.vcnt;
			len = sizeof(float) * 3 * datdef.vcnt;
			glBufferSubData(GL_ARRAY_BUFFER, off, len, atr->dat);
			glVertexAttribPointer(ind, 3, GL_FLOAT, 0, 0, (void *) off);
			break;
		}
		atr = atr->next;
	}
	
	tex = datdef.tl;
	while(tex)
	{
		glActiveTexture(GLTEXTURE(tex->bind));
		img = IMG_Load(tex->file);
		if(!img)
		{
			printf("failed to load texture %s\n", tex->file);
			exit(-1);
		}
		txt = SDL_CreateTextureFromSurface(rnd, img);
		if(SDL_GL_BindTexture(txt, 0, 0))
		{
			printf("filed to bind texture %s to opengl\n", tex->file);
			printf("%s\n", SDL_GetError());
			exit(-1);
		}
		SDL_FreeSurface(img);
		tex = tex->next;
	}

	uni = datdef.ul;
	while(uni)
	{
		ind = glGetUniformLocation(gid, uni->name);
		switch(uni->type)
		{
		case GlobalTimeTYPE:
			fval = clock() - *((clock_t *) uni->dat);
			fval /= CLOCKS_PER_SEC;
			glUniform1f(ind, fval);
			break;
		case IntTYPE:
			ival = *((int *) uni->dat);
			glUniform1i(ind, ival);
			break;
		case FloatTYPE:
			fval = *((float *) uni->dat);
			glUniform1f(ind, fval);
			break;
		case Vec2TYPE:
			fp = (float *) uni->dat;
			glUniform2f(ind, fp[0], fp[1]);
			break;
		case Vec3TYPE:
			fp = (float *) uni->dat;
			glUniform3f(ind, fp[0], fp[1], fp[2]);
			break;
		case Vec4TYPE:
			fp = (float *) uni->dat;
			glUniform4f(ind, fp[0], fp[1], fp[2], fp[3]);
			break;
		case Mat2TYPE:
			fp = (float *) uni->dat;
			glUniformMatrix2fv(ind, 1, 0, fp);
			break;
		case Mat3TYPE:
			fp = (float *) uni->dat;
			glUniformMatrix3fv(ind, 1, 0, fp);
			break;
		case Mat4TYPE:
		case ProjMatTYPE:
			fp = (float *) uni->dat;
			glUniformMatrix4fv(ind, 1, 0, fp);
			break;
		}
		uni = uni->next;
	}
	
	len = sizeof(unsigned int) * datdef.ecnt;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, len, datdef.ent, GL_DYNAMIC_DRAW);
}

static void delnewline(char *str)
{
	if(!str)
		return;
	int i = strlen(str) - 1;
	if(str[i] == '\n')
		str[i] = '\0';
}

static void loadmodel(const char *mnam)
{
	FILE *fl;
	unsigned int i;
	struct cmbh hed;
	struct alist *atr;
	i = strlen(mnam);
	if(i < 5 || strcmp(".cmb", &mnam[i-4]))
	{
		printf("can only parse models in the cmb format!\n");
		exit(-1);
	}
	fl = fopen(mnam, "r");
	if(!fl)
	{
		printf("failed to open model file %s\n", mnam);
		exit(-1);
	}
	fread(&hed, sizeof(struct cmbh), 1, fl);
	if(hed.vers != 1)
	{
		printf("unknown cmb version %u\n", hed.vers);
		exit(-1);
	}
	datdef.vcnt = hed.vcnt;
	datdef.ecnt = hed.fcnt * 3;
	
	datdef.ent = (unsigned int *) malloc(sizeof(unsigned int) * datdef.ecnt);
	
	atr = datdef.al;
	while(atr)
	{
		switch(atr->type)
		{
		case VertPosTYPE:
			fseek(fl, sizeof(struct cmbh), SEEK_SET);
			i = 3 * datdef.vcnt;
			atr->dat = malloc(sizeof(float) * i);
			fread(atr->dat, sizeof(float), i, fl);
			break;
		case TexCoordTYPE:
			i = 3 * datdef.vcnt * sizeof(float);
			i += sizeof(struct cmbh);
			fseek(fl, i, SEEK_SET);
			i = 2 * datdef.vcnt;
			atr->dat = malloc(sizeof(float) * i);
			fread(atr->dat, sizeof(float), i, fl);
			break;
		case NormalTYPE:
			i = 5 * datdef.vcnt * sizeof(float);
			i += sizeof(struct cmbh);
			fseek(fl, i, SEEK_SET);
			i = 3 * datdef.vcnt;
			atr->dat = malloc(sizeof(float) * i);
			fread(atr->dat, sizeof(float), i, fl);
			break;
		}
		atr = atr->next;
	}
	i = 8 * datdef.vcnt * sizeof(float);
	i += sizeof(struct cmbh);
	fseek(fl, i, SEEK_SET);
	i = datdef.ecnt;
	fread(datdef.ent, sizeof(unsigned int), i, fl);
	fclose(fl);
}

static void askattribdata()
{
	char buff[1024];
	unsigned int i, j, k;
	struct alist *atr;
	float *fv;
	printf("how many vertecies are there?\n");
	fgets(buff, 1024, stdin);
	datdef.vcnt = atoi(buff);
	atr = datdef.al;
	while(atr)
	{
		switch(atr->type)
		{
		case VertPosTYPE:
			fv = (float *) malloc(sizeof(float) * 3 * datdef.vcnt);
			for(i = 0; i < datdef.vcnt; i++)
			{
				j = i * 3;
				printf("what is the value of VertPos %d?\n", i);
				fgets(buff, 1024, stdin);
				sscanf(buff, "%f %f %f", &fv[j], &fv[j+1], &fv[j+2]);
			}
			atr->dat = fv;
			break;
		case TexCoordTYPE:
			fv = (float *) malloc(sizeof(float) * 2 * datdef.vcnt);
			for(i = 0; i < datdef.vcnt; i++)
			{
				j = i * 2;
				printf("what is the value of TexCoord %d?\n", i);
				fgets(buff, 1024, stdin);
				sscanf(buff, "%f %f", &fv[j], &fv[j+1]);
			}
			atr->dat = fv;
			break;
		case NormalTYPE:
			fv = (float *) malloc(sizeof(float) * 3 * datdef.vcnt);
			for(i = 0; i < datdef.vcnt; i++)
			{
				j = i * 3;
				printf("what is the value of Normal %d?\n", i);
				fgets(buff, 1024, stdin);
				sscanf(buff, "%f %f %f", &fv[j], &fv[j+1], &fv[j+2]);
			}
			atr->dat = fv;
			break;
		}
		atr = atr->next;
	}
	printf("how many triangle faces are there?\n");
	fgets(buff, 1024, stdin);
	k = atoi(buff);
	datdef.ecnt = k * 3;
	datdef.ent = (unsigned int *) malloc(sizeof(unsigned int) * datdef.ecnt);
	for(i = 0; i < k; i++)
	{
		j = i * 3;
		printf("what is the definition of triangle %d?\n", i);
		fgets(buff, 1024, stdin);
		sscanf(buff, "%u %u %u", &datdef.ent[j], &datdef.ent[j+1], &datdef.ent[j+2]);
	}
}

static void asktexturedata()
{
	char buff[1024];
	unsigned int i;
	struct tlist *tex;
	tex = datdef.tl;
	while(tex)
	{
		printf("What texture should be bound to position %u?\n", tex->bind);
		fgets(buff, 1024, stdin);
		delnewline(buff);
		i = strlen(buff);
		if(i < 5)
		{
			printf("invalid texture name %s\n", buff);
			exit(-1);
		}
		if(strcmp(".png", &buff[i-4]))
		{
			printf("textures must be in png format\n");
			exit(-1);
		}
		tex->file = (char *) malloc(i + 1);
		strcpy(tex->file, buff);
		tex = tex->next;
	}
}

static float *makeortho(float *dim)
{
	float *ret = (float *) calloc(16, sizeof(float));
	ret[0] = 2 / (dim[1] - dim[0]);
	ret[5] = 2 / (dim[3] - dim[2]);
	ret[10] = -2 / (dim[5] - dim[4]);
	ret[12] = -(dim[1] + dim[0]) / (dim[1] - dim[0]);
	ret[13] = -(dim[3] + dim[2]) / (dim[3] - dim[2]);
	ret[14] = -(dim[5] + dim[4]) / (dim[5] - dim[4]);
	ret[15] = 1;
	return ret;
}

static float *makeproj(float *dim)
{
	float *ret = (float *) calloc(16, sizeof(float));
	ret[0] = (2 * dim[4]) / (dim[1] - dim[0]);
	ret[5] = (2 * dim[4]) / (dim[3] - dim[2]);
	ret[8] = (dim[1] + dim[0]) / (dim[1] - dim[0]);
	ret[9] = (dim[3] + dim[2]) / (dim[3] - dim[2]);
	ret[10] = -(dim[5] + dim[4]) / (dim[5] - dim[4]);
	ret[11] = -1;
	ret[14] = -(2 * dim[5] * dim[4]) / (dim[5] - dim[4]);
	return ret;
}

static void askuniformdata()
{
	char buff[1024];
	unsigned int i, j;
	clock_t *time;
	float *fval;
	int *ival;
	float pdim[6];
	struct ulist *uni;
	uni = datdef.ul;
	while(uni)
	{
		switch(uni->type)
		{
		case GlobalTimeTYPE:
			time = (clock_t *) malloc(sizeof(clock_t));
			*time = clock();
			uni->dat = time;
			break;
		case FloatTYPE:
			fval = (float *) malloc(sizeof(float));
			printf("what is the value of float uniform %s?\n", uni->name);
			fgets(buff, 1024, stdin);
			*fval = atof(buff);
			uni->dat = fval;
			break;
		case IntTYPE:
			ival = (int *) malloc(sizeof(int));
			printf("what is the value of int uniform %s?\n", uni->name);
			fgets(buff, 1024, stdin);
			*ival = atoi(buff);
			uni->dat = ival;
			break;
		case Vec2TYPE:
			fval = (float *) malloc(sizeof(float) * 2);
			printf("what is the value of vec2 uniform %s?\n", uni->name);
			fgets(buff, 1024, stdin);
			sscanf(buff, "%f %f", &fval[0], &fval[1]);
			uni->dat = fval;
			break;
		case Vec3TYPE:
			fval = (float *) malloc(sizeof(float) * 3);
			printf("what is the value of vec3 uniform %s?\n", uni->name);
			fgets(buff, 1024, stdin);
			sscanf(buff, "%f %f %f", &fval[0], &fval[1], &fval[2]);
			uni->dat = fval;
			break;
		case Vec4TYPE:
			fval = (float *) malloc(sizeof(float) * 4);
			printf("what is the value of vec4 uniform %s?\n", uni->name);
			fgets(buff, 1024, stdin);
			sscanf(buff, "%f %f %f %f", &fval[0], &fval[1], &fval[2], &fval[3]);
			uni->dat = fval;
			break;
		case Mat2TYPE:
			fval = (float *) malloc(sizeof(float) * 4);
			for(i = 0; i < 2; i++)
			{
				j = i * 2;
				printf("what is the value of column %d of Mat2 %s?\n", i, uni->name);
				fgets(buff, 1024, stdin);
				sscanf(buff, "%f %f", &fval[j], &fval[j+1]);
			}
			uni->dat = fval;
			break;
		case Mat3TYPE:
			fval = (float *) malloc(sizeof(float) * 9);
			for(i = 0; i < 3; i++)
			{
				j = i * 3;
				printf("what is the value of column %d of Mat3 %s?\n", i, uni->name);
				fgets(buff, 1024, stdin);
				sscanf(buff, "%f %f %f", &fval[j], &fval[j+1], &fval[j+2]);
			}
			uni->dat = fval;
			break;
		case Mat4TYPE:
			fval = (float *) malloc(sizeof(float) * 16);
			for(i = 0; i < 4; i++)
			{
				j = i * 4;
				printf("what is the value of column %d of Mat4 %s?\n", i, uni->name);
				fgets(buff, 1024, stdin);
				sscanf(buff, "%f %f %f %f", &fval[j], &fval[j+1], 
											&fval[j+2], &fval[j+3]);
			}
			uni->dat = fval;
			break;
		case ProjMatTYPE:
			printf("what are the dimensions for ProjMat %s?\n", uni->name);
			printf("l r b t n f\n");
			fgets(buff, 1024, stdin);
			sscanf(buff, "%f %f %f %f %f %f", &pdim[0], &pdim[1], &pdim[2],
											&pdim[3], &pdim[4], &pdim[5]);
			printf("is ProjMat %s (p)erspective or (o)rthographic?\n", uni->name);
			fgets(buff, 1024, stdin);
			if(buff[0] == 'o')
				fval = makeortho(pdim);
			else
				fval = makeproj(pdim);
			uni->dat = fval;
			break;
		}
		uni = uni->next;
	}
}

static void askshaderdat()
{
	char buff[1024];
	if(datdef.al)
	{
		printf("Load attribute data from a file? (y/n)\n");
		fgets(buff, 1024, stdin);
		if(buff[0] == 'y')
		{
			printf("what is the path to the model?\n");
			fgets(buff, 1024, stdin);
			delnewline(buff);
			loadmodel(buff);
		}
		else
			askattribdata();
	}
	if(datdef.tl)
		asktexturedata();
	if(datdef.ul)
		askuniformdata();
}

static unsigned int gettype(const char *str)
{
	strtype(VertPos)
	strtype(Normal)
	strtype(TexCoord)
	
	strtype(GlobalTime)
	strtype(Float)
	strtype(Int)
	strtype(Vec2)
	strtype(Vec3)
	strtype(Vec4)
	strtype(Mat2)
	strtype(Mat3)
	strtype(Mat4)
	strtype(ProjMat)

	strtype(Texture)

	return 0;
}

void getshaderdat(const char *fl)
{
	char buff[1024], tmp[1024], *t, *n;
	unsigned int i;
	FILE *dsc = fopen(fl, "r");
	struct alist **atl;
	struct tlist **tel;
	struct ulist **unl;
	
	if(!dsc)
	{
		printf("failed to open desc file\n");
		exit(-1);
	}

	while(fgets(buff, 1024, dsc))
	{
		delnewline(buff);
		i = strlen(buff) - 1;
		
		if(strlen(buff) < 4)
			continue;
		strcpy(tmp, buff);
		t = strtok(buff, " ");
		n = strtok(0, " ");
		if(!n || !strlen(t) || !strlen(n))
		{
			printf("ERROR: encountered line with improper format\n");
			printf("%s\n", tmp);
			exit(-1);
		}
		
		i = gettype(t);
		
		if(i & ATTRIBMASK)
			goto ADDATTRIB;

		if(i & VALUEMASK)
			goto ADDVALUE;
		
		if(i & TEXTURE)
			goto ADDTEXTURE;
		
		printf("Identifier %s has unknown type\n", t);
		exit(-1);

ADDATTRIB:
		atl = &datdef.al;
		while(*atl)
		{
			if((*atl)->type == i)
			{
				printf("two definitions for attribute %s\n", t);
				exit(-1);
			}
			if(!strcmp((*atl)->name, n))
			{
				printf("attribute name %s given twice\n", n);
				exit(-1);
			}
			atl = &(*atl)->next;
		}

		*atl = (struct alist *) malloc(sizeof(struct alist));
		(*atl)->type = i;
		(*atl)->name = (char *) malloc(strlen(n) + 1);
		strcpy((*atl)->name, n);
		(*atl)->dat = 0; 
		(*atl)->next = 0;
		goto GONEXT;


ADDVALUE:
		unl = &datdef.ul;
		while(*unl)
		{
			if(!strcmp((*unl)->name, n))
			{
				printf("uniform name %s given twice\n", n);
				exit(-1);
			}
			unl = &(*unl)->next;
		}

		*unl = (struct ulist *) malloc(sizeof(struct ulist));
		(*unl)->type = i;
		(*unl)->name = (char *) malloc(strlen(n) + 1);
		strcpy((*unl)->name, n);
		(*unl)->dat = 0;
		(*unl)->next = 0;
		goto GONEXT;

ADDTEXTURE:
		tel = &datdef.tl;
		i = atoi(n);
		while(*tel)
		{
			if((*tel)->bind == i)
			{
				printf("texture bind location %d given twice\n", i);
				exit(-1);
			}
			tel = &(*tel)->next;
		}
		
		*tel = (struct tlist *) malloc(sizeof(struct tlist));
		(*tel)->bind = i;
		(*tel)->glid = 0;
		(*tel)->file = 0;
		(*tel)->next = 0;
GONEXT:
		continue;
	}
	

	askshaderdat();
}

