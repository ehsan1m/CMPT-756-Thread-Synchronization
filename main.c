#include "main.h"
#include <unistd.h> /*AM: this is for the purposes of parsing command line arguments through getopt*/


// void *fnC()
// {
//     int i;
//     for(i=0;i<1000000;i++)
//     {   
//         c++;
//     }   
// }


pthread_mutex_t count_mutex;
pthread_spinlock_t count_spin;
my_spinlock_t count_myspin;


//int pshared;
//int ret;
/* initialize a spin lock */
//ret = pthread_spin_init(&count_spin, pshared); 


void *pthreadMutexTest()
{
	
    int i;
	int j;
	int k;
	
	int localCount = 0;
	
    for(i=0;i<numItterations;i++)
    {
		
		for(j=0;j<workOutsideCS;j++)/*How much work is done outside the CriticalSection*/
		{
			localCount++;
		}
		
		pthread_mutex_lock(&count_mutex);
		for(k=0;k<workInsideCS;k++)/*How much work is done inside the CriticalSection*/
		{
			c++;
		}
		pthread_mutex_unlock(&count_mutex);    
	
    }   


}

/*AM start*/
void *pthreadSpinTest()
{
    int i;
	int j;
	int k;
	int localCount = 0;
	
    for(i=0;i<numItterations;i++)
    {

		for(j=0;j<workOutsideCS;j++)//How much work is done outside the CriticalSection
		{
			localCount++;
		}
		
		pthread_spin_lock(&count_spin);
		for(k=0;k<workInsideCS;k++)//How much work is done inside the CriticalSection
		{
			c++;
		}
		pthread_spin_unlock(&count_spin);    
	
    }   


}

void *pthreadMySpinTASTest()
{
    int i;
	int j;
	int k;
	int localCount = 0;
	
    for(i=0;i<numItterations;i++)
    {

		for(j=0;j<workOutsideCS;j++)//How much work is done outside the CriticalSection
		{
			localCount++;
		}
		
		my_spinlock_lockTAS(&count_myspin);
		for(k=0;k<workInsideCS;k++)//How much work is done inside the CriticalSection
		{
			c++;
		}
		my_spinlock_unlock(&count_myspin);    
	
    }   


}
void *pthreadMySpinTTASTest()
{
    int i;
	int j;
	int k;
	int localCount = 0;
	
    for(i=0;i<numItterations;i++)
    {

		for(j=0;j<workOutsideCS;j++)//How much work is done outside the CriticalSection
		{
			localCount++;
		}
		
		my_spinlock_lockTTAS(&count_myspin);
		for(k=0;k<workInsideCS;k++)//How much work is done inside the CriticalSection
		{
			c++;
		}
		my_spinlock_unlock(&count_myspin);    
	
    }   


}
/*AM end*/

int runTest(int testID)
{

/*You must create all data structures and threads for you experiments*/

if (testID == 0 || testID == 1 ) /*Pthread Mutex*/
{
	pthread_mutex_init(&count_mutex, 0);
	c=0;
	struct timespec start;
	struct timespec stop;
	unsigned long long result; //64 bit integer

	pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);	
	int i;
	int rt;

	clock_gettime(CLOCK_MONOTONIC, &start);
	for(i=0;i<numThreads;i++)
	{
	
	 if( rt=(pthread_create( threads+i, NULL, &pthreadMutexTest, NULL)) )
	{
		printf("Thread creation failed: %d\n", rt);
		return -1;	
	}
	
	}
	
	for(i=0;i<numThreads;i++) //Wait for all threads to finish
	{
		 pthread_join(threads[i], NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &stop);

	printf("Threaded Run Pthread (Spinlock) Total Count: %d\n", c);
	result=timespecDiff(&stop,&start);
	printf("Pthread Spinlock time(ms): %llu\n",result/1000000);

}

if(testID == 0 || testID == 2) /*Pthread Spinlock*/
{
/*Pthread Spinlock goes here*/
	pthread_spin_init(&count_spin, 0);
	c=0;
	struct timespec start;
	struct timespec stop;
	unsigned long long result; //64 bit integer

	pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);	
	int i;
	int rt;

	clock_gettime(CLOCK_MONOTONIC, &start);

	

	for(i=0;i<numThreads;i++)
	{
	
	 if( rt=(pthread_create( threads+i, NULL, &pthreadSpinTest, NULL)) )
	{
		printf("Thread creation failed: %d\n", rt);
		return -1;	
	}
	
	}
	
	for(i=0;i<numThreads;i++) //Wait for all threads to finish
	{
		 pthread_join(threads[i], NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &stop);

	printf("Threaded Run Pthread (Spin) Total Count: %d\n", c);
	result=timespecDiff(&stop,&start);
	printf("Pthread Spin time(ms): %llu\n",result/1000000);
}

if(testID == 0 || testID == 3) /*MySpinlockTAS*/
{
/* mySpinlock TAS goes here*/
	/*Pthread Spinlock goes here*/
	my_spinlock_init(&count_myspin);
	c=0;
	struct timespec start;
	struct timespec stop;
	unsigned long long result; //64 bit integer

	pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);	
	int i;
	int rt;

	clock_gettime(CLOCK_MONOTONIC, &start);

	

	for(i=0;i<numThreads;i++)
	{
	
	 if( rt=(pthread_create( threads+i, NULL, &pthreadMySpinTASTest, NULL)) )
	{
		printf("Thread creation failed: %d\n", rt);
		return -1;	
	}
	
	}
	
	for(i=0;i<numThreads;i++) //Wait for all threads to finish
	{
		 pthread_join(threads[i], NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &stop);

	printf("Threaded Run  MySpinlockTAS Total Count: %d\n", c);
	result=timespecDiff(&stop,&start);
	printf("MySpinlockTAS time(ms): %llu\n",result/1000000);
}

if(testID == 0 || testID == 4) /*MySpinlockTTAS*/
{
/* mySpinlock TAS goes here*/
	/*Pthread Spinlock goes here*/
	my_spinlock_init(&count_myspin);
	c=0;
	struct timespec start;
	struct timespec stop;
	unsigned long long result; //64 bit integer

	pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);	
	int i;
	int rt;

	clock_gettime(CLOCK_MONOTONIC, &start);

	

	for(i=0;i<numThreads;i++)
	{
	
	 if( rt=(pthread_create( threads+i, NULL, &pthreadMySpinTTASTest, NULL)) )
	{
		printf("Thread creation failed: %d\n", rt);
		return -1;	
	}
	
	}
	
	for(i=0;i<numThreads;i++) //Wait for all threads to finish
	{
		 pthread_join(threads[i], NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &stop);

	printf("Threaded Run MySpinlockTTAS Total Count: %d\n", c);
	result=timespecDiff(&stop,&start);
	printf("MySpinlockTTAS time(ms): %llu\n",result/1000000);
}

/*....you must implement the other tests....*/

	return 0;
}

int testAndSetExample()
{
volatile long test = 0; //Test is set to 0
printf("Test before atomic OP:%d\n",test);
tas(&test);
printf("Test after atomic OP:%d\n",test);
}

int processInput(int argc, char *argv[])
{

/*testid: 0=all, 1=pthreadMutex, 2=pthreadSpinlock, 3=mySpinLockTAS, 4=mySpinLockTTAS, 5=myMutexTAS, 6=myQueueLock*/
	/*You must write how to parse input from the command line here, your software should default to the values given below if no input is given*/
	
	numThreads=4;
	numItterations=1000000;
	testID=0;
	workOutsideCS=0;
	workInsideCS=1;
 

	// /* AM: if we have command line parameters start parsing*/
	if(argc > 1) {
    	int opt;

    	while ((opt = getopt(argc, argv, "tiocd")) != -1) {
        	switch (opt) {
        		case 't': numThreads = atoi(argv[optind]); break;
        		case 'i': numItterations = atoi(argv[optind]); break;
        		case 'o': workOutsideCS = atoi(argv[optind]); break;
        		case 'c': workInsideCS = atoi(argv[optind]); break;
        		case 'd': testID = atoi(argv[optind]); break;
       			default:
            		      
            		      printf("Usage of: %s -t #threads -i #Itterations -o #OperationsOutsideCS -c #OperationsInsideCS -d testid\n", argv[0]);
	                      printf("testid: 0=all, 1=pthreadMutex, 2=pthreadSpinlock, 3=mySpinLockTAS, 4=mySpinLockTTAS, 5=myMutexTAS, 6=myQueueLock, \n");	
           }              
        }

	}


	return 0;
}


int main(int argc, char *argv[])
{


	printf("Usage of: %s -t #threads -i #Itterations -o #OperationsOutsideCS -c #OperationsInsideCS -d testid\n", argv[0]);
	printf("testid: 0=all, 1=pthreadMutex, 2=pthreadSpinlock, 3=mySpinLockTAS, 4=mySpinLockTTAS, 5=myMutexTAS, 6=myQueueLock, \n");	
	
	//testAndSetExample(); //Uncomment this line to see how to use TAS
	
	processInput(argc,argv);
	runTest(testID);
	return 0;

}
