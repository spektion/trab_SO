#include <pthread.h>
#include <stdio.h>
#include <math.h>
/*parameters to print function*/
struct char_print_parms
{
	double count;
	double array[99];
};
void* char_print(void* parameters)
{
	/*cast the cookie pointer to the right type. */
	struct char_print_parms* p = (struct char_print_parms*) parameters;
	int i;
	for(i = 1;i <= p->count; i++)
	{
		p->array[i]=sqrt((double)i);
//		printf("+++++a raiz de %d e' %f\n",i,sqrt(i));
	}
	pthread_exit((void*)p->array);
	return NULL;
}
int main()
{
	pthread_t thread1_id;
	struct char_print_parms thread1_args;
	long marray[99];
	int i;

	/*create a new thread to print 30000 x's */
	thread1_args.count = 99;
	pthread_create (&thread1_id, NULL, &char_print,&thread1_args);

	
	/*make sure the first thread has finished*/
	pthread_join (thread1_id,(void*)&marray);
	printf("calculo de raizes\n");
	for(i = 1;i <= 99; i++)                                                                              
		printf("a raiz de %d e' %f\n",i,marray[i]);

	/*safe return*/
	return 0;
}
