#include "pcivrm.h"
#include "ctrl.h"

int main(int argc, char **argv)
{
	init_pcivrm();
	begin_ctrl();
	return 0;
}

