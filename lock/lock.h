#ifndef LOCK_H
#define LOCK_H

typedef struct Lock
{
	int val;
} Lock;

extern int tas(int *);

void
lock(Lock *lk)
{
	int i;

	while (!tas(&lk->val))
		;
}

int
canlock(Lock *lk)
{
	if (tas(&lk->val))
		return 0;
	return 1;
}

void
unlock(Lock *lk)
{
	lk->val = 0;
}

#endif
