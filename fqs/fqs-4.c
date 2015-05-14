#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
struct arr_num
{
        int count;
        int *array;
};
*/

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
 
/*

void* quick_sort_m (void* parameters)
{
	struct arr_num* arr = (struct arr_num*) parameters;
	int i,j,p,t;
	pthread_t thread;

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

    //quick_sort_m(arr);
	pthread_create (&thread, NULL, &quick_sort_m,&arr);
	pthread_join (thread, NULL);
	arr->array=arr->array+i;
	arr->count=arr->count-i;
	pthread_create (&thread, NULL, &quick_sort_m,&arr);
	pthread_join (thread, NULL);
	
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

*/
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
	printf(" numero de registos %lu\n",size-1);
	size--;
	long int arr[size];
	fseek(ptr_myfile, sizeof(long int), SEEK_SET );
	for(i=0;i<size;i++)
		fread(&arr[i],sizeof(long int),1,ptr_myfile);
	fclose(ptr_myfile);
	
	printf("\n----------- sorted -----------\n");
	quickSort(arr,0, size);
	for(i=0;i<=size;i++)
		printf("%d - %lu\n",i,arr[i]);
	return 0;
}

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
			{
				rtn=run_s(&argv[2]);
			}
			else if (opt == 'm')
			{
				printf("2do\n");
//				rtn=run_m(&argv[2]);
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
