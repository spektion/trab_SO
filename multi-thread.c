#include <pthread.h>
#include <stdio.h>
#define NUM_THREADS 2
pthread_t tid[NUM_THREADS]; /* array of thread IDs */
void * adder(void * bignum)
{
	int i;
	printf("I am a new thread!\n");
	for(i=0;i<100000;i++) {
		bignum++; /* BUG HERE: THIS IS NOT IN A MUTEX AND IS
	INCORRECT!! */
	}
	pthread_exit(&bignum);
}
main( int argc, char *argv[] )
{
	int i, ret, bignum = 0;
	void *status;
	for (i=0; i<NUM_THREADS; i++) {
		pthread_create(&tid[i], NULL, &adder, &bignum);
	}
	for ( i = 0; i < NUM_THREADS; i++)
		pthread_join(tid[i], &status);
	bignum=*(int *)status;
	printf("main() reporting that all %d threads have terminated\n", i);
	printf("I am main! bignum=%d\n", bignum);
} /* main */
