#include <stdio.h>

int main()
{
	long int num;
	int counter;
	FILE *ptr_myfile;

	ptr_myfile=fopen("test.bin","wb");
	if (!ptr_myfile)
	{
		printf("Unable to open file!");
		return 1;
	}
	for ( counter=0; counter <= 50000; counter++)
	{
		num=rand();
		fwrite(&num, sizeof(long int), 1, ptr_myfile);
	}
	fclose(ptr_myfile);
	return 0;
}
