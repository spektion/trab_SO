#include <stdio.h>

int main(int argc, char *argv[])
{
	long int num,size;
	int counter;
	FILE *ptr_myfile;

	ptr_myfile=fopen(argv[1],"rb");
	if (!ptr_myfile)
	{
		printf("Unable to open file!");
		return 1;
	}

	while(fread(&num,sizeof(long int),1,ptr_myfile))
		size++;
    size--;

	fseek(ptr_myfile, sizeof(long int), SEEK_SET );
	for ( counter=0; counter <= size; counter++)
	{
		fread(&num,	sizeof(long int), 1, ptr_myfile);
		printf("%d - %lu\n",counter,num);
	}
	fclose(ptr_myfile);
	return 0;
}
