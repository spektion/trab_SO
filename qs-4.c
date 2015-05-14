#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 25000

typedef struct arr_num
{
        int count;
        int *array;
}arr_num;

double shared_x;
pthread_mutex_t lock_x;

void swap(int* a, int* b)
{
    int aux;
    aux = *a;
    *a = *b;
    *b = aux;
}
 
int divide(int *vec, int left, int right)
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
 
void quickSort(int *vec, int left, int right)
{
    int r;
 
    if (right > left)
    {
        r = divide(vec, left, right);
        quickSort(vec, left, r - 1);
        quickSort(vec, r + 1, right);
    }
}

void quick_sort (int *a, int n) {
    int i, j, p, t;
    if (n < 2)
        return;
    p = a[n / 2];
    for (i = 0, j = n - 1;; i++, j--) {
        while (a[i] < p)
            i++;
        while (p < a[j])
            j--;
        if (i >= j)
            break;
        t = a[i];
        a[i] = a[j];
        a[j] = t;
    }
	
    quick_sort(a, i);
    quick_sort(a + i, n - i);
}

void* quick_sort_m (void* parameters)
{
	struct arr_num* arr = (struct arr_num*) parameters;
	int i,j,p,t,th_num;
	pthread_t thread[NUM_THREADS];

    if (arr->count < 2)
		return;
	p=arr->array[arr->count/2];
	for(i = 0,j=arr->count-1;; i++,j++)
	{
		while(arr->array[i]<p)
			i++;
		while (p<arr->array[j])
			j--;
        if (i >= j)
            break;
        t = arr->array[i];
        arr->array[i] = arr->array[j];
        arr->array[j] = t;
	}
	pthread_mutex_lock(&lock_x);
	th_num++;
    //quick_sort_m(arr);
	pthread_create (&thread[th_num], NULL, &quick_sort_m,&arr);
	pthread_join (thread[th_num], NULL);
	
	pthread_mutex_unlock(&lock_x);

	pthread_mutex_lock(&lock_x);

	th_num++;
	arr->array=arr->array+i;
	arr->count=arr->count-i;
	pthread_create (&thread[th_num], NULL, &quick_sort_m,&arr);
	pthread_join (thread[th_num], NULL);
	
	pthread_mutex_unlock(&lock_x);
}

int run_s(int num){

    int a[num],i,n;

    n = sizeof a / sizeof a[0];
	for (i=0;i<num;i++)
		a[i]=rand() % num + 1;
	for (i = 0; i < n; i++)
		printf("%d%s", a[i], i == n - 1 ? "\n" : " ");
	printf("\n----------- sorted -----------\n");
	//quickSort(a,0, n);
	quick_sort(a, n);
	for (i = 0; i <= n; i++)
		printf("%d%s", a[i], i == n  ? "\n" : " ");
	return 0;
}

int run_m(int num){

	struct arr_num an;
    int r,j;
	pthread_t thread[NUM_THREADS];

	pthread_mutex_init(&lock_x, NULL);

	an.count=num;
	an.array= malloc(sizeof(int) * an.count);
	for (r=0;r<an.count;r++)
	{
		an.array[r]=rand() % num - 1;
		printf(" -- %d - %d -- \n",r,an.array[r]);
	}
	pthread_create (&thread[0], NULL, &quick_sort_m,&an);
	pthread_join (thread[0], NULL);

	printf("\n----------- sorted -----------\n");

	for (j=0;j<an.count;j++)
		printf(" -- %d - %d --\n",j,an.array[j]);

	return 0;
}

void help(){
		printf("\nUsage: qs [OPTION]... [SIZE]...\nSort with quicksort algoritm a random array of SIZE, SIZE and OPTION are mandatory,SIZE must be a integer, OPTION must be [-s/-m], a default run displays this help and exit.\n\nMandatory arguments to long options are mandatory for short options too.\n\t-m       run with multiprocess support\n\t-s      run without multiprocess support\n\t\t-h     display this help and exit\n\t\t-v  output version information and exit\n\n");
}

void vers(){
printf("\nqs 1.02\nCopyright (C) 2014 Free Software Foundation, Inc.\nLicense GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\nThis is free software: you are free to change and redistribute it.\nThere is NO WARRANTY, to the extent permitted by law.\n\nWritten by Gonçalo Faria, Luis Franco, and Vitor Filipe \n\n");
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
			{
				total=atoi(argv[2]);
				rtn=run_s(total);
			}
			else if (opt == 'm')
			{
				printf("2do\n");
				total=atoi(argv[2]);
				rtn=run_m(total);
			}
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
