/*
 * Sample/test code for running a user program.  You can use this for
 * reference when implementing the execv() system call. Remember though
 * that execv() needs to do more than this function does.
 */

#include <types.h>
#include <kern/unistd.h>
#include <kern/errno.h>
#include <lib.h>
#include <addrspace.h>
#include <thread.h>
#include <curthread.h>
#include <vm.h>
#include <vfs.h>
#include <test.h>

/*
 * Load program "progname" and start running it in usermode.
 * Does not return except on error.
 *
 * Calls vfs_open on progname and thus may destroy it.
 */

int
runprogram(char *progname, char** args, int args_counter)
{
/* 
	tf_a0: file path
	tf_a1: pointer vector 
*/
// 1.Counting the total number of strings in args
	// copy out args
	char** temp_args = kmalloc( (args_counter+1) * sizeof( char* ) );
	int i;
	for (i = 0; i < args_counter; i++) {
	 	*(temp_args+i) = kstrdup( *(args+i) );
//		kprintf("args[] is %s\n", args[i]);
	}
	*(temp_args+i) = NULL;


/*-----------------------------------Copy from runprogram.c---------------------------------------*/
	struct vnode *v;
	vaddr_t entrypoint, stackptr;
	int result;

	/* Open the file. */
	result = vfs_open(progname, O_RDONLY, &v);
	if (result) {
		return result;
	}

	/* We should be a new thread. */
	assert(curthread->t_vmspace == NULL);

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
		char_counter[temp] = strlen(temp_args[temp])+1;	//plus the null terminator
		total_char = total_char + char_counter[temp] ;
		temp++;
	}
	

// 3. Calculate offset in stackptr and copy out args
	temp = 0;
//	argv_ptr[0] = stackptr;

	while(temp != args_counter)
	{
		offset = offset + char_counter[temp];
		argv_ptr[temp] = stackptr - offset; //starting position of argv in stack
	  	copyout(temp_args[temp], argv_ptr[temp], char_counter[temp]); // copy into user stack
		temp ++;
	}
	total_char = total_char + sizeof(vaddr_t)*(args_counter+1);	//total space including argv list
	argv_ptr[args_counter] = 0;

// 4. Allcate space for argv on stack
	
	stackptr = stackptr - total_char - (stackptr - total_char)%4; //byte allocation
	copyout(argv_ptr, stackptr, sizeof(vaddr_t)*(args_counter+1));	

/****************************************************************************************/
	/* Warp to user mode. */
	md_usermode(args_counter /*argc*/, (userptr_t) stackptr /*userspace addr of argv*/,
		    stackptr, entrypoint);
	
	/* md_usermode does not return */
	panic("md_usermode returned\n");
	return EINVAL;
}

