#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
 
#define NUM_THREADS 5 
 
typedef struct _thread_data_t {
  int left,right,thnum,size;
  long *arr;
} thread_data_t;
 
/* shared data between threads 
long arr_x;
pthread_mutex_t lock_x;
*/
sem_t sem;

void swapM(long int* a, long int* b)
{
    long int aux;
    aux = *a;
    *a = *b;
    *b = aux;
}
 
void * divideM(void * param)
{
	thread_data_t *an = (thread_data_t *)param; 
    int i, j;
 
    i = an->left;
    for (j = an->left + 1; j <= an->right; ++j)
    {
        if (an->arr[j] < an->arr[an->left])
        {
            ++i;
//			pthread_mutex_lock(&lock_x);
			sem_wait(&sem);
            swapM(&an->arr[i], &an->arr[j]);
			sem_post(&sem);
//			pthread_mutex_unlock(&lock_x);
        }
    }
//	pthread_mutex_lock(&lock_x);
	sem_wait(&sem);
    swapM(&an->arr[an->left], &an->arr[i]);
	sem_post(&sem);
//	pthread_mutex_unlock(&lock_x);

//	return i;
	pthread_exit((void *)&i);
}

void quickSortM(long int *vec, int left, int right)
{
    int r;
	pthread_t tid;
	struct _thread_data_t an;
	
 

    if (right > left)
    {
		*an.arr=*vec;
		an.left=left;
		an.right=right;
//        r = divideM(vec, left, right);
		pthread_create(&tid, NULL, divideM, (void *) &an);
		pthread_join(tid,(void *) &r);
        quickSortM(vec, left, r - 1);
        quickSortM(vec, r + 1, right);
    }
}

int run_m(char **fname){

    int i,size=0;
    long int num;
    FILE *ptr_myfile;
	struct _thread_data_t an; 

	sem_init(&sem, 0, 0);

    ptr_myfile=fopen(*fname,"rb");
    if (!ptr_myfile)
    {
        printf("Unable to open file!");
        return 1;
    }
    while(fread(&num,sizeof(long int),1,ptr_myfile))
		size++;
    size--;
    an.size=size;
    an.arr= malloc(sizeof(long int) * an.size);
    fseek(ptr_myfile, sizeof(long int), SEEK_SET );
    for(i=0;i<size;i++)
		fread(&an.arr[i],sizeof(long int),1,ptr_myfile);
    fclose(ptr_myfile);

    printf("\n----------- sorted -----------\n");
	an.left=0;
	an.right=an.size;
	an.thnum=1;
    quickSortM(an.arr,an.left,an.right);
    for(i=0;i<=size;i++)
		printf("%d - %lu\n",i,an.arr[i]);
    printf("\n\t\t---> 2do\n");
    return 0;
}

void swap(long int* a, long int* b)
{
    long int aux;
    aux = *a;
    *a = *b;
    *b = aux;
}
 
int divide(long int *vec, int left, int right)
{
    int i, j;
 
    i = left;
    for (j = left + 1; j <= right; ++j)
    {
        if (vec[j] < vec[left])
        {
            ++i;
            swap(&vec[i], &vec[j]);
        }
    }
    swap(&vec[left], &vec[i]);

	return i;
}


void quickSort(long int *vec, int left, int right)
{
    int r;
 
    if (right > left)
    {
        r = divide(vec, left, right);
        quickSort(vec, left, r - 1);
        quickSort(vec, r + 1, right);
    }
}
 
int run_s(char **fname){

    int i;
    long int num,size=0;
    FILE *ptr_myfile;

    ptr_myfile=fopen(*fname,"rb");
    if (!ptr_myfile)
    {
		printf("Unable to open file!");
        return 1;
    }
    while(fread(&num,sizeof(long int),1,ptr_myfile))
		size++;
    size--;
    long int arr[size];
    fseek(ptr_myfile, sizeof(long int), SEEK_SET );
    for(i=0;i<size;i++)
		fread(&arr[i],sizeof(long int),1,ptr_myfile);
    fclose(ptr_myfile);

    printf("\n----------- sorted -----------\n");
    quickSort(arr,0, size);
    for(i=0;i<size;i++)
        printf("%d - %lu\n",i,arr[i]);
    return 0;
}

/* 
void *thr_func(void *arg) {
  thread_data_t *data = (thread_data_t *)arg;
 
  printf("hello from thr_func, thread id: %d\n", data->tid);
  // get mutex before modifying and printing shared_x 
  pthread_mutex_lock(&lock_x);
    shared_x += data->stuff;
    printf("x = %f\n", shared_x);
  pthread_mutex_unlock(&lock_x);
 
  pthread_exit(NULL);
}
 
int main(int argc, char **argv) {
  pthread_t thr[NUM_THREADS];
  int i, rc;
  // create a thread_data_t argument array
  thread_data_t thr_data[NUM_THREADS];
 
  // initialize shared data 
  shared_x = 0;
 
  // initialize pthread mutex protecting "shared_x"
  pthread_mutex_init(&lock_x, NULL);
 
  // create threads 
  for (i = 0; i < NUM_THREADS; ++i) {
    thr_data[i].tid = i;
    thr_data[i].stuff = (i + 1) * NUM_THREADS;
    if ((rc = pthread_create(&thr[i], NULL, thr_func, &thr_data[i]))) {
      fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
      return EXIT_FAILURE;
    }
  }
  // block until all threads complete 
  for (i = 0; i < NUM_THREADS; ++i) {
    pthread_join(thr[i], NULL);
  }
 
  return EXIT_SUCCESS;
}
*/

void help(){
	printf("\nUsage: fqs [OPTION]... [FILE]...\nSort with quicksort algoritm a file with random long ints, OPTION and FILE are mandatory, OPTION must be [-s/-m], a default run displays this help and exit.\n\nMandatory arguments to long options are mandatory for short options too.\n\t-m       run with multiprocess support\n\t-s      run without multiprocess support\n\t\t-h     display this help and exit\n\t\t-v  output version information and exit\n\n");
}

void vers(){
	printf("\nqs 1.03\nCopyright (C) 2014 Free Software Foundation, Inc.\nLicense GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\nThis is free software: you are free to change and redistribute it.\nThere is NO WARRANTY, to the extent permitted by law.\n\nWritten by Gonçalo Faria, Luis Franco, and Vitor Filipe \n\n");
}

int main (int argc, char *argv[]) {

	int rtn,total,opt;

    switch(argc){
		case 2:
			opt = getopt(argc, argv, "v");
            if(opt == 'v')
            {
                vers();
                rtn=0;
            }
            else 
            {
                help();
				rtn=1;
            }
        break;
        case 3:
			opt = getopt(argc, argv, "s:m:");
            if(opt == 's')
                rtn=run_s(&argv[2]);
            else if (opt == 'm')
                rtn=run_m(&argv[2]);
            else
            {
                help();
                rtn=1;
            }
        break;
        default:
            help();
            rtn=1;
        break;
    }
    return rtn;
}
