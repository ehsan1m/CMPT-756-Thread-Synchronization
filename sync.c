/*
 * NAME, etc.
 *
 * sync.c
 *
 */

#define _REENTRANT

#include "sync.h"


/*
 * Spinlock routines
 */

int my_spinlock_init(my_spinlock_t *lock)
{
	lock->lock = 0;
	return 0;
}

int my_spinlock_destroy(my_spinlock_t *lock)
{
}

int my_spinlock_unlock(my_spinlock_t *lock)
{
	lock->lock = 0;
	return 0;
}

int my_spinlock_lockTAS(my_spinlock_t *lock)
{
	while(tas(lock));
	return 0;
}


int my_spinlock_lockTTAS(my_spinlock_t *lock)
{
	while(1) {
    	 while(lock->lock);
    	   if(!tas(lock)) {
    		  break;
    	   }
    }
}

int my_spinlock_trylock(my_spinlock_t *lock)
{
}


/*
 * Mutex routines
 */

int my_mutex_init(my_mutex_t *lock)
{
	lock->lock = 0;
	return 0;
}

int my_mutex_destroy(my_mutex_t *lock)
{
}

int my_mutex_unlock(my_mutex_t *lock)
{
	lock->lock = 0;
	return 0;
}

int my_mutex_lock(my_mutex_t *lock)
{

    useconds_t min_delay = 1;
    useconds_t max_delay = 1;
    useconds_t delay = min_delay;

    while(1) 
    {
    	while(lock->lock);
    	if(!tas(lock)) {
    		  break;
    	   }
	   usleep(min_delay);
	    // printf("Thread is sleeping\n");
	    if(delay < max_delay) {
	    	delay = 2*delay;
	    }

    }
   
    return 0;

}

int my_mutex_trylock(my_mutex_t *lock)
{
}

/*
 * Queue Lock
 */

int my_queuelock_init(my_queuelock_t *lock)
{
	lock->nowServing = 0;
	lock->nextTicket=0;
	return 0;
}

int my_queuelock_destroy(my_queuelock_t *lock)
{
}

int my_queuelock_unlock(my_queuelock_t *lock)
{
	(lock->nowServing)++;
	return 0;

}

int my_queuelock_lock(my_queuelock_t *lock)
{

	unsigned long my_ticket;
	my_ticket = faa(&(lock->nextTicket));
//	 printf("inside my_queuelock, my_ticket: %i\n",my_ticket );
    while(lock->nowServing != my_ticket) {}
   	return 0;

}

int my_queuelock_trylock(my_queuelock_t *lock)
{
}

