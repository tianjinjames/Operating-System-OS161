#include <types.h>
#include <lib.h>
#include <threadpid.h>
#include <thread.h>

int chd;

struct threadpid *pid_init(pid_t pidnumb)
{
	if(search_pid == 0)
	{
		return -1;
	}
	struct threadpid *pid = kmalloc(sizeof(struct threadpid));
	pid -> pidnum = pidnumb;
	pid -> next = NULL;
	pid -> child = NULL;
	return pid;
}

int add_threadpid(pid_t pidnumb, struct thread* td)
{
	if(search_pid(pidnumb) == 0)
	{
		return -1;
	}
	struct threadpid *temp;
	struct threadpid *temp2;
	temp = head;
	while(temp -> next != NULL)
	{
		//temp -> prev = temp;
		temp = temp -> next;
	}
	temp -> next = kmalloc(sizeof(struct threadpid));
	temp2 = temp;
	temp = temp -> next;
	temp -> prev = temp2;
	temp -> pidnum = pidnumb;
	temp -> next = NULL;
	temp -> child = NULL;
	temp -> thethread = td;
	temp -> waiting = 0;
	temp -> waited = 0;
	temp -> ecode = 0;
	return 0;
}

int add_childpid(pid_t parent, pid_t pidnumb, struct thread* td)
{
	if(search_pid(parent) == -1)
	{
		return -1;
	}
	if(search_pid(pidnumb) == 0)
	{
		return -1;
	}
	struct threadpid *temp;
	struct threadpid *temp2;
	temp = get_threadpid(parent);
	if(temp -> child == NULL)
	{//printf("add 6\n\n\n\n");
		temp -> child = kmalloc(sizeof(struct threadpid));
		temp2 = temp;
		temp = temp -> child;
		temp -> prev = temp2;
		temp -> pidnum = pidnumb;
		temp -> next = NULL;
		temp -> child = NULL;
		temp -> thethread = td;
		temp -> waiting = 0;
		temp -> waited = 0;
		temp -> ecode = 0;
	}
	else if(temp -> child != NULL)
	{//printf("ADDFOUR \n\n\n");
		//temp -> child -> prev = temp;
		temp = temp -> child;
		while(temp -> next != NULL)
		{
			//temp -> prev = temp;
			temp = temp -> next;
		}
		temp -> next = kmalloc(sizeof(struct threadpid));
		temp2 = temp;
		temp = temp -> next;
		temp -> prev = temp2;
		temp -> pidnum = pidnumb;
		temp -> next = NULL;
		temp -> child = NULL;
		temp -> thethread = td;
		temp -> waiting = 0;
		temp -> waited = 0;
		temp -> ecode = 0;
	}
	return 0;
}

int search_pid(pid_t pidnumb)
{
	struct threadpid *temp;
	temp = head;
	while(temp -> next != NULL)
	{//kprintf("search 6.2\n\n\n\n");
		if(temp -> pidnum == pidnumb)
		{kprintf("search 6.2\n\n\n\n");
			return 0;
		}
		if(temp -> child != NULL)
		{//kprintf("search 6.3\n\n\n\n");
			if(search_child(pidnumb, temp -> child)==0)
			{
				return 0;
			}
		}
		temp = temp -> next;
	}
	if(temp -> pidnum == pidnumb)
	{//kprintf("search 6.4\n\n\n\n");
		return 0;
	}
	else if(temp -> child != NULL)
	{//kprintf("search 6.5\n\n\n\n");
		if(search_child(pidnumb, temp -> child)==0)
		{
			return 0;
		}
	}
	
		return -1;
}

int search_child(pid_t pidnumb, struct threadpid *cur)
{
	while(cur -> next != NULL)
	{//printf("search 6.1\n\n\n\n");
		if(cur -> pidnum == pidnumb)
		{//printf("search 6\n\n\n\n");
			return 0;
		}
		if(cur -> child != NULL)
		{
			if(search_child(pidnumb, cur -> child) == 0)
			{
				return 0;
			}
		}
		cur = cur -> next;
	}
	if(cur -> pidnum == pidnumb)
	{//printf("search 6.9\n\n\n\n");
		return 0;
	}
	else if(cur -> child != NULL)
	{
		if(search_child(pidnumb, cur -> child) == 0)
		{
			return 0;
		}
	}
	
		return -1;
}

struct threadpid *get_threadpid(pid_t pidnumb)
{
	if(search_pid(pidnumb) != 0)
	{
		//printf("ERROR\n");
		return NULL;
	}
	struct threadpid *temp;
	temp = head;
	while(temp -> next != NULL)
	{
		if(temp -> pidnum == pidnumb)
		{
			chd = 0;
			return temp;
		}
		if(temp -> child != NULL)
		{
			if(search_child(pidnumb, temp -> child)==0)
			{
				return get_child(pidnumb, temp -> child);
			}
		}
		temp = temp -> next;
	}
	if(temp -> pidnum == pidnumb)
	{
		chd = 0;
		return temp;
	}
	else if(temp -> child != NULL)
	{
		if(search_child(pidnumb, temp -> child)==0)
		{//printf("SEARCH 6\n\n\n\n\n");
			return get_child(pidnumb, temp -> child);
		}
	}
}

struct threadpid *get_child(pid_t pidnumb, struct threadpid *cur)
{
	//int cnt = 0;
	chd = 1;
	while(cur -> next != NULL)
	{
		if(cur -> pidnum == pidnumb)
		{//printf("666666\n\n\n\n");
		//	if(cnt == 0)
		//		chd = 1;
		//	if(cnt == 1)
		//		chd = 0;
			return  cur;
		}
		if(cur -> child != NULL)
		{
			if(search_child(pidnumb, cur -> child) == 0)
			{
				return get_child(pidnumb, cur -> child);
			}
		}
		cur = cur -> next;
		//cnt = 1;
	}
	if(cur -> pidnum == pidnumb)
	{
	//	if(cnt == 0)
	//		chd = 1;
		return cur;
	}
	else if(cur -> child != NULL)
	{
		if(search_child(pidnumb, cur -> child) == 0)
		{
			return get_child(pidnumb, cur -> child);
		}
	}
}

int delete_pid(pid_t pidnumb)
{
	if(search_pid(pidnumb) != 0)
		return -1;
	struct threadpid *tempo;
	tempo = get_threadpid(pidnumb);
	//if(chd == 1)
	//{
		if(tempo -> prev == NULL)
		{
			head = tempo -> next;
			kfree(tempo);
			return 0;
		}
		if(tempo -> prev -> child -> pidnum == pidnumb)
		{
			kprintf("LOOP1\n");
			if(tempo -> next != NULL)
			{//printf("LOOP1\n");
				tempo -> prev -> child = tempo -> next;
			}
			//if(tempo -> child != NULL)		
			//	tempo -> child = 
			if(tempo -> next == NULL)
			{
				tempo -> prev -> child = NULL;
			}
			kfree(tempo);
			return 0;
		}
		if(tempo -> prev -> next -> pidnum == pidnumb)
		{kprintf("LOOP2\n");
			if(tempo -> next != NULL)
			{//printf("LOOP1\n");
				tempo -> prev -> next = tempo -> next;
			}
			//if(tempo -> child != NULL)		
			//	tempo -> child = 
			if(tempo -> next == NULL)
			{
				tempo -> prev -> next = NULL;
			}
			kfree(tempo);
			return 0;
		}
	/*}
	if(chd == 0)
	{
		//printf("LOOP2\n");
		if(tempo -> next != NULL)
		{
			tempo -> prev -> next = tempo -> next;
		}
		if(tempo -> next == NULL)
		{		
			tempo -> prev -> next = NULL;
		}
		kfree(tempo);
		return 0;
	}*/
	return -1;
}

struct threadpid *get_td(struct thread *td)
{
	struct threadpid *temp;
	temp = head;
	while(temp -> next != NULL)
	{
		if(temp -> thethread == td)
		{
			return temp;
		}
		if(temp -> child != NULL)
		{
			
			return get_child_td(temp -> child, td);
			
		}
		temp = temp -> next;
	}
	if(temp -> thethread == td)
	{
		return temp;
	}
	else if(temp -> child != NULL)
	{
		return get_child_td(temp -> child, td);
	}
	return -1;
}

struct threadpid *get_child_td(struct threadpid *cur, struct thread *td)
{
	//int cnt = 0;
	//chd = 0;
	while(cur -> next != NULL)
	{
		if(cur -> thethread == td)
		{
			return  cur;
		}
		if(cur -> child != NULL)
		{
			if(get_child_td(cur -> child, td) != -1)
				return get_child_td(cur -> child, td);
	
		}
		cur = cur -> next;
		//cnt = 1;
	}
	if(cur -> thethread == td)
	{
		return cur;
	}
	else if(cur -> child != NULL)
	{
		if(get_child_td(cur -> child, td) != -1)
				return get_child_td(cur -> child, td);
	}
	return -1;
}

void print_pid()
{
	struct threadpid *temp;
	temp = head;
	while(temp -> next != NULL)
	{
		kprintf("pid: %d -> ", temp -> pidnum);
		if(temp -> child != NULL)
		{
			print_child(temp -> child);
		}
		temp = temp -> next;
	}
	
		kprintf("pid: %d -> ", temp -> pidnum);
	if(temp -> child != NULL)
	{
		print_child(temp -> child);
	}
}

void print_child(struct threadpid *cur)
{
	while(cur -> next != NULL)
	{
		kprintf("pid_child: %d -> ", cur -> pidnum);
		if(cur -> child != NULL)
		{
			print_child(cur -> child);
		}
		cur = cur -> next;
	}
	
	kprintf("pid_child: %d -> ", cur -> pidnum);
	if(cur -> child != NULL)
	{
		print_child(cur -> child);
	}
}
