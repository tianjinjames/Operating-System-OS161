#ifndef _THREADPID_H_
#define _THREADPID_H_

//#include <unistd.h>
#include <types.h>
#include <lib.h>

struct threadpid 
{
	pid_t pidnum;
	struct threadpid* next;
	struct threadpid* child;
	struct threadpid* prev;
	struct thread* thethread;
	int waiting;
	int waited;
	int ecode;
};

struct threadpid *head;

struct threadpid *pid_init(pid_t pidnumb);

int add_threadpid(pid_t pidnumb, struct thread* td);

//int add_threadpid(pid_t pidnumb);

int add_childpid(pid_t parent, pid_t pidnumb, struct thread* td);

//int add_childpid(pid_t parent, pid_t pidnumb);

int search_pid(pid_t pidnumb);

int search_child(pid_t pidnumb, struct threadpid *cur);

struct threadpid *get_threadpid(pid_t pidnumb);

struct threadpid *get_child(pid_t pidnumb, struct threadpid *cur);

struct threadpid *get_td(struct thread* td);

struct threadpid *get_child_td(struct threadpid *cur, struct thread *td);

int delete_pid(pid_t pidnumb);	

void print_pid();

void print_child(struct threadpid *cur);

#endif /* _THREADPID_H_ */
