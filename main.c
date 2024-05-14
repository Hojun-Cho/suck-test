#include <assert.h>
#include "fiber.h"
#include "lock.h"

void *
func(void *_)
{
	Lock *l = _;
	for (int i = 0; i < 20; i++)
	{
		lock(l);
		write(1, "good bye\n", 10);
		unlock(l);
	}
	return 0;
}

int
main()
{
	int cnt = 5;
	Proc *p;
	Lock l = (Lock){0,};
	int	arr[cnt];

	assert ((p = new_proc(cnt)) != 0);
	for (int i = 0; i < cnt; ++i) {
		arr[i] = i;
		assert (create_fiber(p, func, &l) == 0);
	}
	wait_fibers(p);
	destroy_proc(p);
}
