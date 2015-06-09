#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
 
int NUM_THREADS; 
long int chunk_end;
long int chunk_init;
pthread_mutex_t lock_x;
 
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
			pthread_mutex_lock(&lock_x);
            swapM(&vec[i], &vec[j]);
			pthread_mutex_unlock(&lock_x);
        }
    }
	pthread_mutex_lock(&lock_x);
    swapM(&vec[left], &vec[i]);
	pthread_mutex_unlock(&lock_x);

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

void *th_qs(void *vect)
{
	long int *vec = (long int *) vect;
	
	quickSortM(vec,chunk_init, chunk_end);

	pthread_exit(0);
}

int run_t(char **fname)
{

    int i;
    long int num,size=0;
    FILE *ptr_myfile;
	pthread_t tid[NUM_THREADS];
	void *status;
	char str[30];
	struct timeval time;

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

	gettimeofday(&time, NULL);
	double dTime1 = time.tv_sec+(time.tv_usec/1000000.0);

	pthread_mutex_init(&lock_x, NULL);
	for ( i = 1; i < NUM_THREADS; i++)
	{
		chunk_init = chunk_end;
		chunk_end = (size/NUM_THREADS)*i;
		pthread_create(&tid[i], NULL, &th_qs, (void*)arr);
	}
	for ( i = 1; i < NUM_THREADS; i++)
		pthread_join(tid[i], &status);
	chunk_init=0;
	chunk_end = size;
	pthread_create(&tid[NUM_THREADS], NULL, &th_qs, (void*)arr);
	pthread_join(tid[NUM_THREADS], &status);

	gettimeofday(&time, NULL);
	double dTime2 = time.tv_sec+(time.tv_usec/1000000.0);

	strcpy(str, *fname);
	strcat(str, ".out");
	ptr_myfile=fopen(str,"wb");
    if (!ptr_myfile)
    {
		printf("Unable to open file!");
        return 1;
    }
    for ( i=0; i < size; i++)
        fwrite(&arr[i], sizeof(long int), 1, ptr_myfile);                                                                                                                                       
    fclose(ptr_myfile);

	printf("\n\t - Quicksearch -\n");
	printf("\n %.6lf seconds elapsed \n", dTime2 - dTime1);
	printf("\n Output file = %s \n\n",str);

	return 0;
}

int run_p(char **fname){

    int i,wpid,status;
    long int num,size=0;
    FILE *ptr_myfile;
	char str[30];
	struct timeval time;

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

	gettimeofday(&time, NULL);
	double dTime1 = time.tv_sec+(time.tv_usec/1000000.0);

	for ( i = 1; i < NUM_THREADS; i++)
	{
		chunk_init = chunk_end;
		chunk_end = (size/NUM_THREADS)*i;
		if ( i==2 || i== 4 || i==6)
			wpid = fork();
			if(wpid>0)
				break;
		quickSortM(arr,chunk_init, chunk_end);
		wait(&status);
	}
	quickSortM(arr,0, size);

	gettimeofday(&time, NULL);
	double dTime2 = time.tv_sec+(time.tv_usec/1000000.0);

	strcpy(str, *fname);
	strcat(str, ".out");
	ptr_myfile=fopen(str,"wb");
    if (!ptr_myfile)
    {
		printf("Unable to open file!");
        return 1;
    }
    for ( i=0; i < size; i++)
        fwrite(&arr[i], sizeof(long int), 1, ptr_myfile);                                                                                                                                       
    fclose(ptr_myfile);

	printf("\n\t - Quicksearch -\n");
	printf("\n %.6lf seconds elapsed \n", dTime2 - dTime1);
	printf("\n Output file = %s \n\n",str);

	return 0;
}

void help(){
	printf("\nUsage: fqs [OPTION]... [FILE]...\nSort with quicksort algoritm a file with random long ints, OPTION and FILE are mandatory, OPTION must be [-s/-m], a default run displays this help and exit.\n\t-t       run with thread support\n\t-p      run without multiprocess support\n\t\t-h     display this help and exit\n\t\t-v  output version information and exit\n\n");
}

void vers(){
	printf("\nqs 1.35\nCopyright (C) 2015 Free Software Foundation, Inc.\nLicense GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\nThis is free software: you are free to change and redistribute it.\nThere is NO WARRANTY, to the extent permitted by law.\n\nWritten by Gonçalo Faria, Luis Franco, and Vitor Filipe \n\n");
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
        case 4:
			NUM_THREADS=atoi(argv[3]);
			opt = getopt(argc, argv, "t:p:");
            if(opt == 'p')
                rtn=run_p(&argv[2]);
            else if (opt == 't')
                rtn=run_t(&argv[2]);
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
