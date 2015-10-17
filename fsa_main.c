#include "pcivrm.h"

int main(int argc, char **argv)
{
	pcivrm_init();
	getdevinfo();
	return 0;
}

