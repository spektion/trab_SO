#include <stdio.h> 
#include <stdlib.h> 
#include <sys/ipc.h> 
#include <sys/msg.h>
main() 
{
	int id = msgget ( 1000, IPC_CREAT | 0666 );
	if ( id < 0 ) 
	{
		printf("Erro na criacao da mailbox \n" );
		exit(1);
	}
	printf ("Criada mailbox com id %d\n", id); 
}
