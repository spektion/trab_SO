#include <pthread.h>
#include <stdio.h>
#include <math.h>
/*parameters to print function*/
void* char_print(void* parameters)
{
	/*cast the cookie pointer to the right type. */
	long array[99];
	int i,count=99;
	for(i = 1;i <= count; i++)
	{
		array[i]=sqrt((double)i);
		printf("+++++a raiz de %d e' %f\n",i,sqrt(i));
	}
	pthread_exit((void*)array);
	return NULL;
}
int main()
{
	pthread_t thread1_id;
	long marray[99];
	int i;

	/*create a new thread to print 30000 x's */
	pthread_create (&thread1_id, NULL, &char_print,NULL);

	
	/*make sure the first thread has finished*/
	pthread_join (thread1_id,(void*)&marray);
	printf("calculo de raizes\n");
	for(i = 1;i <= 99; i++)                                                                              
//		printf("a raiz de %d e' %f\n",i,marray[i]);

	/*safe return*/
	return 0;
}
