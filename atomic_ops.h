#ifndef __ATOMIC_OPS_H4__
#define __ATOMIC_OPS_H4__


static inline unsigned long tas(volatile unsigned long* ptr)//Test and Set
{
return __sync_lock_test_and_set(ptr, 1);
}


static inline unsigned long cas(volatile unsigned long* ptr, unsigned long old, unsigned long _new)//Compare and Swap
{
    return __sync_val_compare_and_swap(ptr, old, _new);

}


static inline unsigned long faa(volatile unsigned long* ptr )//Test and Set
{
	unsigned long var;
	// printf("got here\n");
	var = __sync_fetch_and_add(ptr, 1);
	// printf("inside atomic, value= %i\n", var);
	return var;
}


#endif // __ATOMIC_OPS_H__
