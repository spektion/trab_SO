#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
 
int NUM_THREADS; 
long int chunk_end;
long int chunk_init;
pthread_mutex_t lock_x;
 
void swapM(long int* a, long int* b) //Funcao de troca de valores em memoria
{
    long int aux;
    aux = *a;
    *a = *b;
    *b = aux;
}
 
long int divideM(long int *vec, int left, int right) //Funcao de subdivisao do array a ordenar
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

void quickSortM(long int *vec, int left, int right) //funcao de ordenacao via quicksort
{
    int r;
 
    if (right > left)
    {
        r = divideM(vec, left, right);
        quickSortM(vec, left, r - 1);
        quickSortM(vec, r + 1, right);
    }
}

void *th_qs(void *vect) //funcao das threads
{
	long int *vec = (long int *) vect;
	
	quickSortM(vec,chunk_init, chunk_end);

	pthread_exit(0);
}

int run_t(char **fname) //funcao principal com suporte de threads
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
    while(fread(&num,sizeof(long int),1,ptr_myfile))						//Verifica tamanho do ficheiro
		size++;
    size--;
    long int arr[size];														//cria array com tamanho necessario
    fseek(ptr_myfile, sizeof(long int), SEEK_SET );
    for(i=0;i<size;i++)														//carrega array do ficheiro 
		fread(&arr[i],sizeof(long int),1,ptr_myfile);
    fclose(ptr_myfile);

	gettimeofday(&time, NULL);												//inicia contagem de tempo
	double dTime1 = time.tv_sec+(time.tv_usec/1000000.0);

	pthread_mutex_init(&lock_x, NULL);										//inicia mutex
	for ( i = 1; i < NUM_THREADS; i++)										//evoca threads e subdivide array
	{
		chunk_init = chunk_end;
		chunk_end = (size/NUM_THREADS)*i;
		pthread_create(&tid[i], NULL, &th_qs, (void*)arr);
	}
	for ( i = 1; i < NUM_THREADS; i++)										//termina threads
		pthread_join(tid[i], &status);
	chunk_init=0;
	chunk_end = size;
	pthread_create(&tid[NUM_THREADS], NULL, &th_qs, (void*)arr);			// evoca ultima ordenacao total
	pthread_join(tid[NUM_THREADS], &status);

	gettimeofday(&time, NULL);												//termina contagem de tempo
	double dTime2 = time.tv_sec+(time.tv_usec/1000000.0);

	strcpy(str, *fname);													//cria ficheiro output
	strcat(str, ".out");
	ptr_myfile=fopen(str,"wb");
    if (!ptr_myfile)
    {
		printf("Unable to open file!");
        return 1;
    }
    for ( i=0; i < size; i++)												//escreve array no ficheiro output
        fwrite(&arr[i], sizeof(long int), 1, ptr_myfile);                                                                                                                                       
    fclose(ptr_myfile);

	printf("\n\t - Quicksearch -\n");										// escreve stdout resultados
	printf("\n %.6lf seconds elapsed \n", dTime2 - dTime1);
	printf("\n Output file = %s \n\n",str);

	return 0;
}

int run_p(char **fname) //funcao principal com suporte a multiprocessos
{

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
    while(fread(&num,sizeof(long int),1,ptr_myfile))						//Verifica tamanho do ficheiro
		size++;
    size--;
    long int arr[size];														//cria array com tamanho necessario
    fseek(ptr_myfile, sizeof(long int), SEEK_SET );
    for(i=0;i<size;i++)														//carrega array do ficheiro 
		fread(&arr[i],sizeof(long int),1,ptr_myfile);
    fclose(ptr_myfile);

	gettimeofday(&time, NULL);												//inicia contagem de tempo
	double dTime1 = time.tv_sec+(time.tv_usec/1000000.0);

	for ( i = 1; i < NUM_THREADS; i++)										// evoca processos e subdivide array
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
	quickSortM(arr,0, size);												// evoca ultima ordenacao total

	gettimeofday(&time, NULL);												// termina contagem de tempo
	double dTime2 = time.tv_sec+(time.tv_usec/1000000.0);

	strcpy(str, *fname);													//cria ficheiro output 
	strcat(str, ".out");
	ptr_myfile=fopen(str,"wb");
    if (!ptr_myfile)
    {
		printf("Unable to open file!");
        return 1;
    }
    for ( i=0; i < size; i++)												
        fwrite(&arr[i], sizeof(long int), 1, ptr_myfile);                   //escreve array no ficheiro output                                                                          
    fclose(ptr_myfile);

	printf("\n\t - Quicksearch -\n");										// escreve stdout resultados
	printf("\n %.6lf seconds elapsed \n", dTime2 - dTime1);
	printf("\n Output file = %s \n\n",str);

	return 0;
}

void help() //funcao com output help
{
	printf("\nUsage: fqs [OPTION]... [FILE]...[NUM]...\nSort with quicksort algoritm a file with random long ints, OPTION, FILE and NUM are mandatory, OPTION must be [-t/-p], NUM is int to indicate the number of threads/processes, a default run displays this help and exit.\n\t-t       run with thread support\n\t-p      run with multiprocess support\n\t\t-h     display this help and exit\n\t\t-v  output version information and exit\n\n");
}

void vers() //funcao com output version
{
	printf("\nqs 1.45\nCopyright (C) 2015 Free Software Foundation, Inc.\nLicense GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\nThis is free software: you are free to change and redistribute it.\nThere is NO WARRANTY, to the extent permitted by law.\n\nWritten by Gonçalo Faria, Luis Franco, and Vitor Filipe \n\n");
}

int main (int argc, char *argv[]) //funcao main com gestao de parametros enviados pelo utilizador
{

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
