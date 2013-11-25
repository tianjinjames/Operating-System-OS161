/* 
 * stoplight.c
 *
 * 31-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: You can use any synchronization primitives available to solve
 * the stoplight problem in this file.
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
#include <synch.h>  /* Semaphore */
#include <curthread.h>
#include <machine/spl.h>
/*
 *
 * Constants
 *
 */
int carpassed = 0;
//DEFINE CAR DESTINATION
#define STRAIGHT 2
#define RIGHT 1
#define LEFT 0

//DEFINE WHERE CAR APPROACH
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3


/*
 * Number of cars created.
 */

#define NCARS 20

	static struct semaphore* NWlock;
	static struct semaphore* NElock;
	static struct semaphore* SWlock;
	static struct semaphore* SElock;
	static struct semaphore* NSlock;
	static struct semaphore* WElock;
	static struct semaphore* LTlock;
	static struct semaphore* STlock;
	static struct semaphore* RDlock;

/*
 *
 * Function Definitions
 *
 */

static const char *directions[] = { "N", "E", "S", "W" };
int spl;
static const char *msgs[] = {
        "approaching:",
        "region1:    ",
        "region2:    ",
        "region3:    ",
        "leaving:    "
};

/* use these constants for the first parameter of message */
enum { APPROACHING, REGION1, REGION2, REGION3, LEAVING };

static void
message(int msg_nr, int carnumber, int cardirection, int destdirection)
{
        kprintf("%s car = %2d, direction = %s, destination = %s\n",
                msgs[msg_nr], carnumber,
                directions[cardirection], directions[destdirection]);
}

// Car struct
struct car{
  int cardirection;
  int carturn;
  int carnumber;
};



/*
 * gostraight()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement passing straight through the
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
gostraight(unsigned long cardirection,
           unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */
        
        (void) cardirection;
        (void) carnumber;
	switch(cardirection){
		case NORTH:
//	     spl = splhigh();
			P(LTlock);			
			P(STlock);
			P(NSlock);
			P(SWlock);
			P(NWlock);
             message(APPROACHING, carnumber, cardirection, SOUTH);
             message(REGION1, carnumber, cardirection, SOUTH);
             message(REGION2, carnumber, cardirection, SOUTH);
             message(LEAVING, carnumber, cardirection, SOUTH);
             		V(NWlock);
			V(SWlock);
			V(NSlock);
			V(LTlock);	
			V(STlock);		
//	     splx(spl);
			break;

		case SOUTH:
//	     spl = splhigh();
			P(LTlock);			
			P(STlock);
			P(NSlock);
			P(NElock);
			P(SElock);
             message(APPROACHING, carnumber, cardirection, NORTH);
             message(REGION1, carnumber, cardirection, NORTH);
             message(REGION2, carnumber, cardirection, NORTH);
             message(LEAVING, carnumber, cardirection, NORTH);
             		V(SElock);
			V(NElock);
			V(NSlock);
			V(LTlock);
			V(STlock);			
//	     splx(spl);
			break;

		case EAST:
//	     spl = splhigh();
			P(LTlock);			
			P(STlock);
			P(WElock);
			P(NWlock);
			P(NElock);
             message(APPROACHING, carnumber, cardirection, WEST);
             message(REGION1, carnumber, cardirection, WEST);
             message(REGION2, carnumber, cardirection, WEST);
             message(LEAVING, carnumber, cardirection, WEST);
             		V(NElock);
			V(NWlock);
			V(WElock);
			V(LTlock);	
			V(STlock);		
//	     splx(spl);
			break;

		case WEST:
//	     spl = splhigh();
			P(LTlock);			
			P(STlock);
			P(WElock);
			P(SElock);
			P(SWlock);
             message(APPROACHING, carnumber, cardirection, EAST);
             message(REGION1, carnumber, cardirection, EAST);
             message(REGION2, carnumber, cardirection, EAST);
             message(LEAVING, carnumber, cardirection, EAST);
             		V(SWlock);
			V(SElock);
			V(WElock);
			V(LTlock);
			V(STlock);			
//	     splx(spl);
             		break;
        }
}


/*
 * turnleft()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a left turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnleft(unsigned long cardirection,
         unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */

        (void) cardirection;
        (void) carnumber;

	switch(cardirection){
		case NORTH:
//	     spl = splhigh();
			P(LTlock);			
			P(STlock);
			P(SElock);
			P(SWlock);
			P(NWlock);
             message(APPROACHING, carnumber, cardirection, EAST);
             message(REGION1, carnumber, cardirection, EAST);
             message(REGION2, carnumber, cardirection, EAST);
             message(REGION3, carnumber, cardirection, EAST);
             message(LEAVING, carnumber, cardirection, EAST);
			V(SElock);
			V(SWlock);
			V(NWlock);
			V(LTlock);
			V(STlock);			

//	     splx(spl);
			break;

		case SOUTH:
//	     spl = splhigh();
			P(LTlock);			
			P(STlock);
			P(NWlock);
			P(NElock);
			P(SElock);
             message(APPROACHING, carnumber, cardirection, WEST);
             message(REGION1, carnumber, cardirection, WEST);
             message(REGION2, carnumber, cardirection, WEST);
             message(REGION3, carnumber, cardirection, WEST);
             message(LEAVING, carnumber, cardirection, WEST);
			V(NWlock);
			V(NElock);
			V(SElock);
			V(LTlock);	
			V(STlock);		
//	     splx(spl);
			break;

		case EAST:
//	     spl = splhigh();
			P(LTlock);			
			P(STlock);
			P(SWlock);
			P(NWlock);
			P(NElock);
             message(APPROACHING, carnumber, cardirection, SOUTH);
             message(REGION1, carnumber, cardirection, SOUTH);
             message(REGION2, carnumber, cardirection, SOUTH);
             message(REGION3, carnumber, cardirection, SOUTH);
             message(LEAVING, carnumber, cardirection, SOUTH);
			V(SWlock);
			V(NWlock);
			V(NElock);
			V(LTlock);
			V(STlock);			
//	     splx(spl);
			break;

		case WEST:
//	     spl = splhigh();
			P(LTlock);			
			P(STlock);
			P(NElock);
			P(SElock);
			P(SWlock);
             message(APPROACHING, carnumber, cardirection, NORTH);
             message(REGION1, carnumber, cardirection, NORTH);
             message(REGION2, carnumber, cardirection, NORTH);
             message(REGION3, carnumber, cardirection, NORTH);
             message(LEAVING, carnumber, cardirection, NORTH);
			V(NElock);
			V(SElock);
			V(SWlock);
			V(LTlock);	
			V(STlock);		
//	     splx(spl);
             		break;
	}
}


/*
 * turnright()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a right turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnright(unsigned long cardirection,
          unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */

        (void) cardirection;
        (void) carnumber;
	
	switch(cardirection){
		case NORTH:
			P(NWlock);
             message(APPROACHING, carnumber, cardirection, WEST);
             message(REGION1, carnumber, cardirection, WEST);
             message(LEAVING, carnumber, cardirection, WEST);
			V(NWlock);
			break;

		case WEST:
			P(SWlock);
             message(APPROACHING, carnumber, cardirection, SOUTH);
             message(REGION1, carnumber, cardirection, SOUTH);
             message(LEAVING, carnumber, cardirection, SOUTH);
             		V(SWlock);
           		break;

		case SOUTH:
			P(SElock);
             message(APPROACHING, carnumber, cardirection, EAST);
             message(REGION1, carnumber, cardirection, EAST);
             message(LEAVING, carnumber, cardirection, EAST);
             		V(SElock);
			break;

		case EAST:
			P(NElock);
             message(APPROACHING, carnumber, cardirection, NORTH);
             message(REGION1, carnumber, cardirection, NORTH);
             message(LEAVING, carnumber, cardirection, NORTH);
             		V(NElock);
			break;
        }
}


/*
 * approachintersection()
 *
 * Arguments: 
 *      void * unusedpointer: currently unused.
 *      unsigned long carnumber: holds car id number.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Change this function as necessary to implement your solution. These
 *      threads are created by createcars().  Each one must choose a direction
 *      randomly, approach the intersection, choose a turn randomly, and then
 *      complete that turn.  The code to choose a direction randomly is
 *      provided, the rest is left to you to implement.  Making a turn
 *      or going straight should be done by calling one of the functions
 *      above.
 */
 
static
void
approachintersection(void * unusedpointer,
                     unsigned long carnumber)
{
        int cardirection;
	int carturn;
        /*
         * Avoid unused variable and function warnings.
         */

        (void) unusedpointer;
        (void) carnumber;
	(void) gostraight;
	(void) turnleft;
	(void) turnright;

        /*
         * cardirection is set randomly.
         */
	P(RDlock);
        cardirection = random() % 4;
 	carturn = random() % 3;

	kprintf("%d %d\n",  cardirection, carturn);

        switch(carturn){
        case RIGHT:
          turnright(cardirection, carnumber);
          break;
        case LEFT:
          turnleft(cardirection, carnumber);
          break;
        case STRAIGHT:
          gostraight(cardirection, carnumber);
          break;
        }
	V(RDlock);
}


/*
 * createcars()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Notes:
 *      Driver code to start up the approachintersection() threads.  You are
 *      free to modiy this code as necessary for your solution.
 */

int
createcars(int nargs,
           char ** args)
{
        int index, error;

        /*
         * Avoid unused variable warnings.
         */

        (void) nargs;
        (void) args;

	/*
         * Set Up Semaphore.
         */
	NWlock = sem_create("NWlock", 1);
	NElock = sem_create("NElock", 1);
	SWlock = sem_create("SWlock", 1);
	SElock = sem_create("SElock", 1);
	WElock = sem_create("WElock", 1);
	NSlock = sem_create("NSlock", 1);
	LTlock = sem_create("LTlock", 1);//leftturn
	STlock = sem_create("STlock", 1);//STRAIGHT
	RDlock = sem_create("RDlock", 1);//randlock

        for (index = 0; index < NCARS; index++) {

                error = thread_fork("approachintersection thread",
                                    NULL,
                                    index,
                                    approachintersection,
                                    NULL
                                    );

                /*
                 * panic() on error.
                 */

                if (error) {
                        
                        panic("approachintersection: thread_fork failed: %s\n",
                              strerror(error)
                              );
                }
	}
	
        return 0;
}
