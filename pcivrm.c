#include "pcivrm.h"
#include <CL/cl.h>
#include <stdio.h>
#include <sys/mman.h>

struct device_list
{
	cl_device_id did;
	cl_command_queue q;
	cl_command_queue devq;
	struct device_list *next;
};


static cl_context clc;
static struct device_list dev[4] = {{0}};

void getdevnames(char **dst, int cnt, int len)
{
	int i = 0;
	struct device_list *l = dev;
	while(l && i < cnt)
	{
		clGetDeviceInfo(l->did, CL_DEVICE_NAME, len, dst[i++], 0);
		l = l->next;
	}
}

static int claim_context(cl_context *c)
{	
	unsigned int i;
	cl_platform_id p;
	cl_device_id *d;
	clGetPlatformIDs(1, &p, 0);
	clGetDeviceIDs(p, CL_DEVICE_TYPE_GPU, 0, 0, &i);
	d = (cl_device_id *) alloca(sizeof(cl_device_id) * i);
	clGetDeviceIDs(p, CL_DEVICE_TYPE_GPU, i, d, 0);
	*c = clCreateContext(0, i, d, 0, 0, 0);
	return (*c == 0);
}

static int claim_devices(struct device_list *dl, int max, cl_context c)
{
	int i, j, k;
	cl_device_id *ids;
	const cl_queue_properties qp[3] = { CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE, 0 };

	const cl_queue_properties dp[3] = { CL_QUEUE_PROPERTIES,
		CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE |
		CL_QUEUE_PROFILING_ENABLE | CL_QUEUE_ON_DEVICE, 0};

	clGetContextInfo(c, CL_CONTEXT_NUM_DEVICES, sizeof(int), (void *) &i, 0);
	max = max > i ? i : max;
	ids = alloca(max * sizeof(cl_device_id));
	clGetContextInfo(c, CL_CONTEXT_DEVICES, sizeof(cl_device_id) * max, ids, 0);
	for(i = k = 0; i < max; i++)
	{
		dl[i].next = (max - i) == 1 ? 0 : &dl[i+1];
		dl[i].did = ids[i];
		dl[i].q = clCreateCommandQueueWithProperties(c, ids[i], qp, &j);
		k |= j;
		dl[i].devq = clCreateCommandQueueWithProperties(c, ids[i], dp, &j);
		k |= j;
	}
	return k;
}

static int build_manager(struct device_list *dl)
{

	return 0;	
}

int init_pcivrm()
{
	claim_context(&clc);
	claim_devices(dev, 4, clc);	
	build_manager(dev);
	return 0;
}



