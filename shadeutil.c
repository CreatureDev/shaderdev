#include "shadeutil.h"
#include "SDL_opengl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	unsigned int vab;
	
	struct alist *al;

	struct tlist *tl;
	
	struct ulist *ul;
} datdef = {0};

static void loadmodel(const char *mnam)
{

}

static void askattribdata()
{

}

static void asktexturedata()
{

}

static void askuniformdata()
{

}

static void askshaderdat()
{
	char buff[1024];
	if(datdef.al)
	{
		printf("Load attribute data from a file? (y/n)\n")
		gets(buff);
		if(buff[0] == 'y')
		{
			printf("what is the path to the model?\n");
			gets(buff);
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
		i = strlen(buff) - 1;
		if(buff[i] == '\n')
			buff[i--] = '\0';
		
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

