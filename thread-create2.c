#include <pthread.h>
#include <stdio.h>
/*parameters to print function*/
struct char_print_parms
{
	/* the character to print */
	char character;
	/* the number of times to print it. */
	int count;
};
void* char_print(void* parameters)
{
	/*cast the cookie pointer to the right type. */
	struct char_print_parms* p = (struct char_print_parms*) parameters;
	int i;
	for(i = 0;i < p->count; i++)
		fputc(p->character,stderr);
	return NULL;
}
int main()
{
	pthread_t thread1_id;
	pthread_t thread2_id;
	struct char_print_parms thread1_args;
	struct char_print_parms thread2_args;

	/*create a new thread to print 30000 x's */
	thread1_args.character = 'x';
	thread1_args.count = 30;
	pthread_create (&thread1_id, NULL, &char_print,&thread1_args);

	/*create a new thread to print 20000 o's */                                                               
    thread2_args.character = 'o';                                                                             
    thread2_args.count = 20;                                                                               
    pthread_create (&thread2_id, NULL, &char_print,&thread2_args);
	
	/*make sure the first thread has finished*/
	pthread_join (thread1_id, NULL);
	pthread_join (thread2_id, NULL);
	/*safe return*/
	return 0;
}
