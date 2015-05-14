#include <stdio.h>

int create(int size,char **fname)
{
	long int num;
	int counter;
	FILE *ptr_myfile;
	
	ptr_myfile=fopen(*fname,"wb");
	if (!ptr_myfile)
	{
		printf("Unable to open file!");
		return 1;
	}
	for ( counter=0; counter <= size; counter++)
	{
		num=rand();
		fwrite(&num, sizeof(long int), 1, ptr_myfile);
	}
	fclose(ptr_myfile);
	return 0;
}

void help(){
                printf("\nUsage: fgen [OPTION]... [SIZE]...[FILE]...\nGenerate a binary file with random numbers with the SIZE file will be named with FILE, OPTION,SIZE and FILE are mandatory,SIZE must be a integer, OPTION must be [-c], a default run displays this help and exit.\n\nMandatory arguments to long options are mandatory for short options too.\n\t-c       generates a file\n\t\t-h     display this help and exit\n\t\t-v  output version information and exit\n\n");
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
                case 4:
                        opt = getopt(argc, argv, "c:");
                        if(opt == 'c')
                        {
                                total=atoi(argv[2]);
                                rtn=create(total,&argv[3]);
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
