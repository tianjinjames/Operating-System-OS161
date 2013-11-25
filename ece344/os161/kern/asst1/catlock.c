/*
 * catlock.c
 *
 * 30-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: Please use LOCKS/CV'S to solve the cat syncronization problem in 
 * this file.
 */


/*
 * 
 * Includes
 *
 */

#include <types.h>
#include <lib.h>
#include <test.h>
#include <thread.h>
#include <curthread.h>
#include <synch.h>


/*
 * 
 * Constants
 *
 */

/*
 * Number of food bowls.
 */

#define NFOODBOWLS 2

/*
 * Number of cats.
 */

#define NCATS 6

/*
 * Number of mice.
 */

#define NMICE 2

struct cv *c;
struct lock *l;
int bowl[2] = {0,0};
char CorM[2] = {'N','N'};
//&l = lock_create("newlock");
//&cv = cv_create("newcv");
int lockee;

/*
 * 
 * Function Definitions
 * 
 */

/* who should be "cat" or "mouse" */
static void
lock_eat(const char *who, int num, int bowl, int iteration)
{
        kprintf("%s: %d starts eating: bowl %d, iteration %d\n", who, num, 
                bowl, iteration);
        clocksleep(1);
        kprintf("%s: %d ends eating: bowl %d, iteration %d\n", who, num, 
                bowl, iteration);
}

/*
 * catlock()
 *
 * Arguments:
 *      void * unusedpointer: currently unused.
 *      unsigned long catnumber: holds the cat identifier from 0 to NCATS -
 *      1.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Write and comment this function using locks/cv's.
 *
 */

static
void
catlock(void * unusedpointer, 
        unsigned long catnumber)
{
        /*
         * Avoid unused variable warnings.
         */
	//kprintf("BOWL %d,%d CorM %c,%c\n\n\n\n", bowl[0], bowl[1], CorM[0], CorM[1]);
while(curthread->iteration < 4)
{
	lock_acquire(l);
	while(((bowl[0] == 1)&&(bowl[1] == 1))||(CorM[1] == 'M')||(CorM[0] == 'M'))
	{
		cv_wait(c, l);
		
	}

	if(bowl[0] == 0)
	{
		//if(bowl[1] == 1)
		//{
		//	lockee = 1;
		//	lock_acquire(l);
		//}
		bowl[0] = 1;
		CorM[0] = 'C';
		lock_eat("cat", catnumber, 1, curthread->iteration);
		curthread->iteration++;
		bowl[0] = 0;		
		CorM[0] = 'N';	
		//if(lockee == 1)
		//{
		//	lockee = 0;
		//	lock_release(l); 
		//}
		cv_signal(c, l); 
		 
	}
	
	else if(bowl[1] == 0)
	{
		//if(bowl[0] == 1)
		//{
		//	lockee = 1;
		//	lock_acquire(l);
		//}
		bowl[1] = 1;
		CorM[1] = 'C';
		lock_eat("cat", catnumber, 2, curthread->iteration);
		curthread->iteration++;
		bowl[1] = 0;		
		CorM[1] = 'N';	
		//if(lockee == 1)
		//{
		//	lockee = 0;
		//	lock_release(l); 
		//}
		cv_signal(c, l);
		
	}
	lock_release(l);
}
	//CK = kmalloc(sizeof(int));      */                   
        //(void) unusedpointer;
        //(void) catnumber;
}
	

/*
 * mouselock()
 *
 * Arguments:
 *      void * unusedpointer: currently unused.
 *      unsigned long mousenumber: holds the mouse identifier from 0 to 
 *              NMICE - 1.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Write and comment this function using locks/cv's.
 *
 */

static
void
mouselock(void * unusedpointer,
          unsigned long mousenumber)
{
        /*
         * Avoid unused variable warnings.
         */
while(curthread->iteration < 4)
{
	lock_acquire(l);
	while(((bowl[0] == 1)&&(bowl[1] == 1))||(CorM[1] == 'C')||(CorM[0] == 'C'))
	{
		cv_wait(c, l);
	}

	if(bowl[0] == 0)
	{
		//if(bowl[1] == 1)
		//{
		//	lockee = 1;
		//	lock_acquire(l);
		//}
		bowl[0] = 1;
		CorM[0] = 'M';
		lock_eat("mouse", mousenumber, 1, curthread->iteration);
		curthread->iteration++;
		bowl[0] = 0;		
		CorM[0] = 'N';	
		//if(lockee == 1)
		//{
		//	lockee = 0;
		//	lock_release(l); 
		//}
		cv_signal(c, l);  
	}

	else if(bowl[1] == 0)
	{
		//if(bowl[0] == 1)
		//{
		//	lockee = 1;
		//	lock_acquire(l);
		//}
		bowl[1] = 1;
		CorM[1] = 'M';
		lock_eat("mouse", mousenumber, 2, curthread->iteration);
		curthread->iteration++;
		bowl[1] = 0;		
		CorM[1] = 'N';	
		//if(lockee == 1)
		//{
		//	lockee = 0;
		//	lock_release(l); 
		//}
		cv_signal(c, l);
	}
	lock_release(l);
}	
	
	//assert(CK);
	//lock_release(l); */
        
        //(void) unusedpointer;
        //(void) mousenumber;
}


/*
 * catmouselock()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Notes:
 *      Driver code to start up catlock() and mouselock() threads.  Change
 *      this code as necessary for your solution.
 */

int
catmouselock(int nargs,
             char ** args)
{
        int index, error;
   
        /*
         * Avoid unused variable warnings.
         */
	c = cv_create("newcv");
	l = lock_create("newlock");

        (void) nargs;
        (void) args;
   
        /*
         * Start NCATS catlock() threads.
         */

        for (index = 0; index < NCATS; index++) {
           
                error = thread_fork("catlock thread", 
                                    NULL, 
                                    index, 
                                    catlock, 
                                    NULL
                                    );
                
                /*
                 * panic() on error.
                 */

                if (error) {
                 
                        panic("catlock: thread_fork failed: %s\n", 
                              strerror(error)
                              );
                }
        }

        /*
         * Start NMICE mouselock() threads.
         */

        for (index = 0; index < NMICE; index++) {
   
                error = thread_fork("mouselock thread", 
                                    NULL, 
                                    index, 
                                    mouselock, 
                                    NULL
                                    );
      
                /*
                 * panic() on error.
                 */

                if (error) {
         
                        panic("mouselock: thread_fork failed: %s\n", 
                              strerror(error)
                              );
                }
        }

        return 0;
}

/*
 * End of catlock.c
 */
