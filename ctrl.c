#include "ctrl.h"
#include "pcivrm.h"
#include "hashlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static htable_t *tbl = 0;

const char *prompt = "gaze> ";

void ctrl_quit()
{
	puts("aight peace nerd");
	exit(0);
}

void draw_error()
{
	puts("uncrecognized command");
}

void draw_devices()
{
	int i;
	char *buff[4];
	buff[0] = alloca(16);
	buff[1] = alloca(16);
	buff[2] = alloca(16);
	buff[3] = alloca(16);
	getdevnames(buff, 4, 16);
	for(i = 0; i < 4; i++)
	{
		printf("[%d]: %s\n", i, buff[i]);
	}
}

void (*fnctbl[3])() = { draw_error, ctrl_quit, draw_devices };

void draw_header()
{
	puts("wuchu wanna do fam");
}

void draw_prompt()
{
	printf("%s", prompt);
}

void get_command()
{
	int i;
	char cmd[512];
	fgets(cmd, 512, stdin);
	i = strlen(cmd);
	cmd[i-1] = '\0';
	fnctbl[htablesearch(cmd, tbl)]();
}

void create_table()
{
	tbl = htablecreate(128);
	htableinsert("q", 1, tbl);
	htableinsert("lsd", 2, tbl);
}

void begin_ctrl()
{
	create_table();
	draw_header();
	while(1) 
	{
		draw_prompt();
		get_command();
	}
}

