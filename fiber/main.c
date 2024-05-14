#include <stdio.h>
#include <assert.h>
#include "fiber.h"

void *func(void *_)
{
	char buf[3];
	int *x = _;

	buf[0] =*x + '0';
	buf[1] = '\n';
	buf[2] = 0;
	write(1, buf, sizeof(buf));
	return 0;
}

int main()
{
	Proc *p;
	int	arr[DEFAULT_FIBERS];

	assert ((p = new_proc(DEFAULT_FIBERS)) != 0);
	for (int i = 0; i < DEFAULT_FIBERS; ++i) {
		arr[i] = i;
		assert (create_fiber(p, func, &arr[i]) == 0);
	}
	wait_fibers(p);
}
