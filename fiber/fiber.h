#ifndef FIBER_H
#define FIBER_H

#ifndef  _GNU_SOURCE
#define _GNU_SOURCE
#endif

#define DEFAULT_FIBERS (5)
#define DEFAULT_STACK (8192)

#include <sched.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct
{
	pid_t pid;
	void *(*func)(void *);
	void *arg;
	void *stk;
} Fiber;

typedef struct
{
	Fiber *lst;
	pid_t pid;
	int len;
	int cap;
} Proc;

int wait_fibers(Proc *p);
int create_fiber(Proc *p, void *(*func)(void *), void *item);
Proc *new_proc(int n);
void destroy_proc(Proc *p);

Proc*
new_proc(int n)
{
	Proc *p = malloc(sizeof(Proc));
	if (p == 0) {
		return 0;
	}
	if ((p->lst = calloc(n, sizeof(Fiber))) == 0) {
		free(p);
		return 0;
	}
	p->len = 0;
	p->cap = n;
	p->pid = getpid();
	return p;
}

void
destroy_proc(Proc *p)
{
	wait_fibers(p);
	free(p->lst);
	free(p);
}

int
start_fiber(void *_f)
{
	Fiber *f = _f;
	f->func(f->arg);
	return 0;
}

int
create_fiber(Proc *p, void *(*func)(void *), void *item)
{
	if (p->len >= p->cap) {
		return -1;
	}
	Fiber *f = &p->lst[p->len];
	f->arg = item;
	f->func = func;
	if ((f->stk = calloc(sizeof(char), DEFAULT_STACK)) == 0) {
		return -1;
	}
	if ((f->pid = clone(start_fiber, f->stk + DEFAULT_STACK,
			SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM,
			f)) == -1) {
		free(f->stk);
		return -1;
	}
	p->len++;
	return 0;
}

int
wait_fibers(Proc *p)
{
	if (p->pid != getpid()) {
		return -1;
	}
	int n = p->len;
	for (int i = 0; i < n; ++i) {
		pid_t pid = wait(0);
		if (pid == -1) {
			return -1;
		}
		for (int j = 0; j < p->cap; ++j) {
			if (p->lst[j].pid != pid) {
				continue;
			}
			p->len--;
			free(p->lst[j].stk);
			p->lst[j] = (Fiber){0,};
		}
	}
	return 0;
}

#endif
