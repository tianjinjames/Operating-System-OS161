#include <types.h>
#include <kern/errno.h>
#include <kern/unistd.h>
#include <vnode.h>
#include <vfs.h>
#include <uio.h>
#include <lib.h>
#include <machine/spl.h>
#include <machine/trapframe.h>
#include <thread.h>
#include <curthread.h>
#include <addrspace.h>
#include <machine/pcb.h>
#include <machine/spl.h>
#include <kern/callno.h>
#include <syscall.h>
#include <threadpid.h>
#include <synch.h>

int count = 0;
int mall = 0;

/*
 * System call handler.
 *
 * A pointer to the trapframe created during exception entry (in
 * exception.S) is passed in.
 *
 * The calling conventions for syscalls are as follows: Like ordinary
 * function calls, the first 4 32-bit arguments are passed in the 4
 * argument registers a0-a3. In addition, the system call number is
 * passed in the v0 register.
 *
 * On successful return, the return value is passed back in the v0
 * register, like an ordinary function call, and the a3 register is
 * also set to 0 to indicate success.
 *
 * On an error return, the error code is passed back in the v0
 * register, and the a3 register is set to 1 to indicate failure.
 * (Userlevel code takes care of storing the error code in errno and
 * returning the value -1 from the actual userlevel syscall function.
 * See src/lib/libc/syscalls.S and related files.)
 *
 * Upon syscall return the program counter stored in the trapframe
 * must be incremented by one instruction; otherwise the exception
 * return code will restart the "syscall" instruction and the system
 * call will repeat forever.
 *
 * Since none of the OS/161 system calls have more than 4 arguments,
 * there should be no need to fetch additional arguments from the
 * user-level stack.
 *
 * Watch out: if you make system calls that have 64-bit quantities as
 * arguments, they will get passed in pairs of registers, and not
 * necessarily in the way you expect. We recommend you don't do it.
 * (In fact, we recommend you don't use 64-bit quantities at all. See
 * arch/mips/include/types.h.)
 */

int
sys_write(int ignore, void* buf, size_t size)
{
	char* buffer = kmalloc((size+1)*sizeof(char));
	int i;
	copyin(buffer, buf, size);
	buffer = buf;
	buffer[size] = '\0';
	kprintf("%s",buffer);
	count = count + 4;
	return count;
}

int sys_read(int fd, void *buf, size_t length, int* retval)
{
  struct uio iobuf;
  if(sizeof(buf) != 0)
	kprintf("%d!\n", sizeof(buf));
  if (!buf || buf == NULL) {
    kprintf("is buf valid?\n");
    *retval = -1;	//error happens here
    return EFAULT;
  }

   *retval = -1;

  struct vnode *temp;
  char* terminal = kstrdup("con:");
  int error = vfs_open(terminal, O_RDONLY, &temp);	//read-only mode
  kfree(terminal);
/*
 *--------------------------------------------------
 *|	PENDING ERROR CHECKING HERE(file status)   |
 *|	need to free terminal as well	   	   |
 *--------------------------------------------------
 */
  if (error != 0 ) {	
    *retval = -1;	//error happens here
    return EBADF;	
  }

//	Initialize uio stuff

  iobuf.uio_iovec.iov_un.un_ubase = buf;
  iobuf.uio_iovec.iov_len = length;
//  kprintf("iovlen is %d\n", iobuf.uio_iovec.iov_len);
//  iobuf.uio_iov = &iov;
//  iobuf.uio_iovcnt = 1;
  iobuf.uio_offset = 0; // initialize to 0 offset
//  kprintf("offset is %d\n", iobuf.uio_offset);
  iobuf.uio_resid = length;
//  kprintf("resid is %d\n", iobuf.uio_resid);
  iobuf.uio_segflg = UIO_USERSPACE;
  iobuf.uio_rw = UIO_READ;
  iobuf.uio_space = curthread->t_vmspace;	

  // Read buffer here
  error = VOP_READ(temp, &iobuf);
  vfs_close(temp);

 /*
 *--------------------------------------------------
 *|	PENDING ERROR CHECKING HERE(file status)   |   	  
 *--------------------------------------------------
 */
  // Error during reading buffer
  if (error != 0)
  {
	*retval = -1;	//error happens here
	return error;
  }
  *retval = length - iobuf.uio_resid;
  return 0;
}

int sys_fork(struct trapframe* parent_tf, int *retval)
{
	
	struct thread* child_thread = (struct thread*) kmalloc(sizeof(struct thread));
// Copy parent's trapframe to an new allocated trapframe
	struct trapframe* child_tf = (struct trapframe*) kmalloc(sizeof(struct trapframe));
  	if(child_tf==NULL)
    		return ENOMEM;	//out of memory error
	*child_tf = *parent_tf; 

// Copy current adddress space from t_vmspace to child_as
	struct addrspace *child_as ;
  	int error = as_copy(curthread->t_vmspace, &child_as);
  	if(error!=0)
    		return error;
	as_activate(curthread->t_vmspace);

// call thread fork to create child fork
	int kl=splhigh();
  	thread_fork( curthread->t_name, child_tf, (unsigned long) child_as, (void (*) (void *, unsigned long)) md_forkentry, &child_thread);
	splx(kl);

//	set return value child thread pid
	if(curthread != NULL)
	{
		*retval = (get_td(child_thread)) -> pidnum ;
	}
	return 0;
}

int execv(char* program, char** args)
{
/* 
	tf_a0: file path
	tf_a1: pointer vector 
*/
// 1.Counting the total number of strings in args
	int args_counter = 0;

//	args_counter = 0;
//	kprintf("args_counter is %d.\n", args_counter);
//	kprintf("args is %s\n",&(args[0]));
//	while((args[args_counter]) != NULL)
//	{
//		args_counter ++;
//	}
//	kprintf("args_counter is %d.\n", args_counter);
  // program name is in first arcument


	while(*(args+args_counter) != NULL)
	{
		args_counter ++;
	}

//	kprintf("args_counter in execv is %d\n", args_counter);

	char** temp_args = kmalloc( (args_counter+1) * sizeof( char* ) );
        int i;
        for (i = 0; i < args_counter; i++) {
                *(temp_args+i) = kstrdup( *(args+i) );
        }
        *(temp_args+i) = NULL;

/*
// 2.Counting the size of each string including null terminator
	int char_counter[args_counter];
	int total_char = 0;
	int temp = 0;
	while (temp != args_counter)
	{	
		char_counter[temp] = strlen(*args[temp])+1;	//plus the null terminator
		kprintf("Strlength is %d.\n", strlen(args[temp]));
		total_char = total_char + strlen(args[temp]) ;
		temp++;
	}

// 3.Put args on heap using kmalloc + NULL terminated
	char** args_list = (char*) kmalloc((args_counter)*sizeof(char));
	*args_list = *args;	//store the content in args into args_list
*/
/*-----------------------------------Copy from runprogram.c---------------------------------------*/
	struct vnode *v;
	vaddr_t entrypoint, stackptr;
	int result;

	/* Open the file. */
	result = vfs_open(program, O_RDONLY, &v);
	if (result) {
		return result;
	}

  	/* We should NOT be a new thread. */
  	assert(curthread->t_vmspace != NULL);
  	as_destroy(curthread->t_vmspace);

	/* Create a new address space. */
	curthread->t_vmspace = as_create();
	if (curthread->t_vmspace==NULL) {
		vfs_close(v);
		return ENOMEM;
	}

	/* Activate it. */
	as_activate(curthread->t_vmspace);

	/* Load the executable. */
	result = load_elf(v, &entrypoint);
	if (result) {
		/* thread_exit destroys curthread->t_vmspace */
		vfs_close(v);
		return result;
	}

	/* Done with the file now. */
	vfs_close(v);

	/* Define the user stack in the address space */
	result = as_define_stack(curthread->t_vmspace, &stackptr);
	if (result) {
		/* thread_exit destroys curthread->t_vmspace */
		return result;
	}

/*-----------------------------------End copy from runprogram.c---------------------------------------*/

// 2.Counting the size of each string including null terminator
//   Counting the total number of char will be used

        int char_counter[args_counter];
        int total_char = 0;
        int temp = 0;
        int offset = 0;

        vaddr_t argv_ptr[args_counter+1];

        while (temp != args_counter)
        {       
                char_counter[temp] = strlen(temp_args[temp])+1; //plus the null terminator
                total_char = total_char + char_counter[temp] ;
                temp++;
        }
        

// 3. Calculate offset in stackptr and copy out args
        temp = 0;

        while(temp != args_counter)
        {
                offset = offset + char_counter[temp];
                argv_ptr[temp] = stackptr - offset; //starting position of argv in stack
                copyout(temp_args[temp], argv_ptr[temp], char_counter[temp]); // copy into user stack
                temp ++;
        }
        total_char = total_char + sizeof(vaddr_t)*(args_counter+1);     //total space including argv list
        argv_ptr[args_counter] = 0;

// 4. Allcate space for argv on stack
        
        stackptr = stackptr - total_char - (stackptr - total_char)%4; //byte allocation
        copyout(argv_ptr, stackptr, sizeof(vaddr_t)*(args_counter+1));  

/*
// 4.User stackptr is activated, store args from heap to stack.
// 4.1 Allcoate space on stack before copying
//    copyout(temp[i], (userptr_t) argv_ptr[i], len); // copy into user stack
	vaddr_t temp_stackptr[args_counter+1];
	temp_stackptr[0] = stackptr - total_char;
	temp_stackptr[args_counter+1] = NULL;
	size_t char_counter2;
	char_counter2 = (size_t) total_char;

	temp = 0;
	while(temp != args_counter)
	{
		copyout(*args[temp], temp_stackptr[temp], char_counter[temp]);
		temp_stackptr[temp] = temp_stackptr[temp] + char_counter[temp];
		temp ++;
	}

	char_counter2 = char_counter2 + sizeof(vaddr_t) * (args_counter + 1);
 	stackptr = stackptr - char_counter2 - ((stackptr - char_counter2) % 8);
  	copyout(temp_stackptr, (userptr_t) stackptr, sizeof(vaddr_t) * (args_counter+1));
*/
	/* Warp to user mode. */
	md_usermode(args_counter /*argc*/, (userptr_t) stackptr /*userspace addr of argv*/,
		    stackptr, entrypoint);
	
	/* md_usermode does not return */
	panic("md_usermode returned\n");
	return EINVAL;
}

pid_t sys_getpid()
{
//	kprintf("getpid value is %d\n", curthread -> pidnum);
	return (get_td(curthread) -> pidnum);
}


struct cv *c;
struct lock *l;
struct cv *c2;
struct lock *l2;
struct cv *c3;
struct lock *l3;
//struct cv *c4;
//struct lock *l4;

int sys_waitpid(pid_t pid, int *status, int options, int* retval)
{
	if(options != 0){
   		return EINVAL;
  	}
	if(mall==0)
	{
		c = kmalloc(sizeof(struct cv));
		l = kmalloc(sizeof(struct lock));
		c2 = kmalloc(sizeof(struct cv));
		l2 = kmalloc(sizeof(struct lock));
		c3 = kmalloc(sizeof(struct cv));
		l3 = kmalloc(sizeof(struct lock));
		mall = 1;
	}

	lock_acquire(l2);
	struct threadpid *temp;
	temp = get_threadpid(get_td(curthread) -> pidnum);
	if((temp -> child) == NULL)
	{
		lock_release(l2);
		return EINVAL;
	}
	temp = temp -> child;
	while(temp -> next != NULL)
	{
		if((temp -> pidnum) == pid)
		{
			if(temp -> waiting == 5)
			{kprintf("**1**\n");
			
				*status = temp -> ecode;
				*retval = pid;
	
				temp -> waiting = 0;
				lock_release(l2);
				cv_signal(c3,l3);
			}  
			else
			{kprintf("**2**\n");
				temp -> waited = 1;
				lock_release(l2);
				cv_wait(c, l);
				lock_acquire(l2);
				*status = temp -> ecode;
				*retval = pid;
				temp -> waited = 0;
				lock_release(l2);
			}
			return 0;
		}
		temp = temp -> next;
	}
	if((temp -> pidnum) == pid)
	{
		if(temp -> waiting == 5)
		{	kprintf("**3**\n");
			*status = temp -> ecode;
			*retval = pid;

			temp -> waiting = 0;
			lock_release(l2);
			cv_signal(c3,l3);
		}  
		else
		{//kprintf("**4** %d\n", pid);
			temp -> waited = 1;//kprintf("**4.1**\n");
			lock_release(l2);//kprintf("**4.2**\n");
			cv_wait(c, l);//kprintf("**4.3**\n");
			lock_acquire(l2);//kprintf("**4.4**\n");
			*status = temp -> ecode;//kprintf("**4.5** %d\n", temp -> ecode);
			*retval = pid;//kprintf("**4.6**\n");
			temp -> waited = 0;
			lock_release(l2);//kprintf("**4.7**\n");
		}
		return 0;
	}
	else
	{
		lock_release(l2);
		return EINVAL;
	}	
	
	return 0;
}

void sys__exit(int exitcode)
{
	if(mall==0)
	{
		c = kmalloc(sizeof(struct cv));
		l = kmalloc(sizeof(struct lock));
		c2 = kmalloc(sizeof(struct cv));
		l2 = kmalloc(sizeof(struct lock));
		c3 = kmalloc(sizeof(struct cv));
		l3 = kmalloc(sizeof(struct lock));
		mall = 1;	
	}
	lock_acquire(l2);
	if(get_td(curthread) -> waited == 1)
	{	//kprintf("**5** %d\n", get_td(curthread) -> pidnum);
		get_td(curthread) -> ecode = exitcode;	//kprintf("**5.1** %d\n", get_td(curthread) -> ecode);
		lock_release(l2);//kprintf("**5.2**\n");
		cv_signal(c, l); //kprintf("**5.3**\n");
		while(get_td(curthread) -> waited == 1)
		{
		}
		lock_acquire(l2);//kprintf("**5.4**\n");
		delete_pid(get_td(curthread) -> pidnum);//kprintf("**5.5**\n");
		lock_release(l2);//kprintf("**5.6**\n");
		thread_exit();
	}
	else
	{	//kprintf("**6**\n");
		get_td(curthread) -> waiting = 5;
		get_td(curthread) -> ecode = exitcode;
		lock_release(l2);
		cv_wait(c3,l3);
		lock_acquire(l2);	
		delete_pid(get_td(curthread) -> pidnum);
		lock_release(l2);
		thread_exit();	
	}
	
}



void
mips_syscall(struct trapframe *tf)
{
	int callno;
	int32_t retval;
	int err;

	assert(curspl==0);

	callno = tf->tf_v0;

	/*
	 * Initialize retval to 0. Many of the system calls don't
	 * really return a value, just 0 for success and -1 on
	 * error. Since retval is the value returned on success,
	 * initialize it to 0 by default; thus it's not necessary to
	 * deal with it except for calls that return other values, 
	 * like write.
	 */

	retval = 0;
	

	switch (callno) {
	    case SYS_reboot:
//		kprintf("11\n");
		err = sys_reboot(tf->tf_a0);
//		kprintf("1\n");
		break;
	    case SYS_write:
//		kprintf("12\n");
	  	err = sys_write(tf->tf_a0, tf->tf_a1, tf->tf_a2);
//		kprintf("--2\n");
		break;
	    case SYS_read:
//		kprintf("13\n");
	  	err = sys_read(tf->tf_a0, tf->tf_a1, tf->tf_a2, &retval);
//		kprintf("3\n");
		break;	
	    case SYS_fork:
//		kprintf("14\n");
		err = sys_fork(tf, &retval);
//		kprintf("4\n");
		break;
	    case SYS_execv:
//		kprintf("15\n");
		err = execv(tf->tf_a0,tf->tf_a1);//a0 is program path, a1 is **args
//		kprintf("5\n");
		break;
	    case SYS_getpid:
//		kprintf("16\n");
		retval = sys_getpid();
//		kprintf("6\n");
		break;
	    case SYS_waitpid:
//		kprintf("17\n");
		err = sys_waitpid((pid_t) tf->tf_a0, (int *) tf->tf_a1, (int) tf->tf_a2, &retval);
//		kprintf("7\n");
		break;
            case SYS__exit:
//		kprintf("18\n");
		sys__exit(tf->tf_a0);
//		kprintf("8\n");
		break;

	    /* Add stuff here */
 
	    default:
		kprintf("Unknown syscall %d\n", callno);
		err = ENOSYS;
		break;
	}


	if (err) {
		/*
		 * Return the error code. This gets converted at
		 * userlevel to a return value of -1 and the error
		 * code in errno.
		 */
//		kprintf("90 errcode is %d\n", err);
		tf->tf_v0 = err;
		tf->tf_a3 = 1;      /* signal an error */
	}
	else {
//		kprintf("91\n");
		/* Success. */
		tf->tf_v0 = retval;
		tf->tf_a3 = 0;      /* signal no error */
	}
	
	/*
	 * Now, advance the program counter, to avoid restarting
	 * the syscall over and over again.
	 */
//		kprintf("92\n");
	tf->tf_epc += 4;
//		kprintf("93\n");
	/* Make sure the syscall code didn't forget to lower spl */
	assert(curspl==0);
}


void
md_forkentry(struct trapframe *child_tf, unsigned long addrspace)
{
	/*
	 * This function is provided as a reminder. You need to write
	 * both it and the code that calls it.
	 *
 * Thus, you can trash it and do things another way if you prefer.
	 */
 	struct addrspace* child_as = (struct addrspace*) addrspace;
	struct trapframe tplocal;
	tplocal = *child_tf;
	child_tf = &tplocal;

	// modify V0 and a3 in tf to indicate success
	child_tf->tf_v0 = 0;	//success
	child_tf->tf_a3 = 0;	//success
	
	// Now, advance the program counter, to avoid restarting
	// the syscall over and over again.

	child_tf->tf_epc += 4;  //pc=pc+4

	// load and activate child address space
	curthread->t_vmspace = child_as;
	as_activate(curthread->t_vmspace);
	// return to usermode
	mips_usermode(&tplocal);
	(void)child_tf;
}
