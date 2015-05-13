#include <stdio.h>

int main()
{
	long int num;
	int counter;
	FILE *ptr_myfile;

	ptr_myfile=fopen("test.bin","rb");
	if (!ptr_myfile)
	{
		printf("Unable to open file!");
		return 1;
	}
	for ( counter=0; counter <= 50000; counter++)
	{
		fread(&num,	sizeof(long int), 1, ptr_myfile);
		printf("%d - %lu\n",counter,num);
	}
	fclose(ptr_myfile);
	return 0;
}
