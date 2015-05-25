#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
 
#define NUM_THREADS 4 
 
void swapM(long int* a, long int* b)
{
    long int aux;
    aux = *a;
    *a = *b;
    *b = aux;
}
 
long int divideM(long int *vec, int left, int right)
{
    int i, j;
 
    i = left;
    for (j = left + 1; j <= right; ++j)
    {
        if (vec[j] < vec[left])
        {
            ++i;
            swapM(&vec[i], &vec[j]);
        }
    }
    swapM(&vec[left], &vec[i]);

	return i;
}

void quickSortM(long int *vec, int left, int right)
{
    int r;
 
    if (right > left)
    {
        r = divideM(vec, left, right);
        quickSortM(vec, left, r - 1);
        quickSortM(vec, r + 1, right);
    }
}

int run_m(char **fname){

    int i,j=1;
    long int num,size=0,qsize,cmlsize;
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
	qsize=size/4;
	cmlsize=qsize;
    long int arr1[qsize],arr2[qsize],arr3[qsize],arr4[qsize];
    fseek(ptr_myfile, sizeof(long int), SEEK_SET );
    for(i=0;i<size;i++){
		switch(j){
		case 1:
			fread(&arr1[i],sizeof(long int),1,ptr_myfile);
        break;
		case 2:
			fread(&arr2[i-qsize],sizeof(long int),1,ptr_myfile);	
        break;
		case 3:
			fread(&arr3[i-(2*qsize)],sizeof(long int),1,ptr_myfile);
        break;
		case 4:
			fread(&arr4[i-(3*qsize)],sizeof(long int),1,ptr_myfile);
        break;
		}		
		if(i==cmlsize)
		{
			cmlsize += qsize;
			j++;
		}
	}
    fclose(ptr_myfile);

    printf("\n----------- sorted -----------\n");
    quickSortM(arr1,0, qsize);
    quickSortM(arr2,0, qsize);
    quickSortM(arr3,0, qsize);
    quickSortM(arr4,0, qsize);
	
    for(i=0;i<qsize;i++)
	{
        printf("%d - %lu\n",i,arr1[i]);
        printf("%d - %lu\n",i,arr2[i]);
        printf("%d - %lu\n",i,arr3[i]);
        printf("%d - %lu\n",i,arr4[i]);

	}
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

void help(){
	printf("\nUsage: fqs [OPTION]... [FILE]...\nSort with quicksort algoritm a file with random long ints, OPTION and FILE are mandatory, OPTION must be [-s/-m], a default run displays this help and exit.\n\t-m       run with multiprocess support\n\t-s      run without multiprocess support\n\t\t-h     display this help and exit\n\t\t-v  output version information and exit\n\n");
}

void vers(){
	printf("\nqs 1.15\nCopyright (C) 2015 Free Software Foundation, Inc.\nLicense GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\nThis is free software: you are free to change and redistribute it.\nThere is NO WARRANTY, to the extent permitted by law.\n\nWritten by Gonçalo Faria, Luis Franco, and Vitor Filipe \n\n");
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
