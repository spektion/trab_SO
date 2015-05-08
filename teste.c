#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{

fork();
fork();
fork();
fork();
if(fork())
	printf("univ\n");

return 0;
}

