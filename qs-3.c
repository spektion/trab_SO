#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct arr_num
{
        int count;
        int *array;
};
 
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

/*
void quick_sort_m (int *a, int n) {
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
    quick_sort_m(a, i);
    quick_sort_m(a + i, n - i);
}
*/

void* quick_sort_m (void* parameters)
{
	struct arr_num* arr = (struct arr_num*) parameters;
	int i,j,p,t;
	pthread_t thread;

    if (arr->count < 2)
		return NULL;
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

    //quick_sort_m(arr);
	pthread_create (&thread, NULL, &quick_sort_m,&arr);
	pthread_join (thread, NULL);
	arr->array=arr->array+i;
	arr->count=arr->count-i;
	pthread_create (&thread, NULL, &quick_sort_m,&arr);
	pthread_join (thread, NULL);
	
}

int run_s(int num){

    int a[num],i,n;

    n = sizeof a / sizeof a[0];
	for (i=0;i<num;i++)
		a[i]=rand() % num + 1;
	for (i = 0; i < n; i++)
		printf("%d%s", a[i], i == n - 1 ? "\n" : " ");
	printf("\n----------- sorted -----------\n");
	quick_sort(a, n);
	for (i = 0; i < n; i++)
		printf("%d%s", a[i], i == n - 1 ? "\n" : " ");
	return 0;
}

int run_m(int num){

	struct arr_num an;
    int r,j;
	pthread_t thread;

	an.count=num;
	an.array= malloc(sizeof(int) * an.count);
	for (r=0;r<an.count;r++)
	{
		an.array[r]=rand() % num - 1;
		printf(" -- %d - %d -- \n",r,an.array[r]);
	}
	pthread_create (&thread, NULL, &quick_sort_m,&an);
	pthread_join (thread, NULL);

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
