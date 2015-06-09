#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
typedef struct 
{ 
	long tipo;
	char texto[250]; 
} MsgStruct;
main() 
{
	int msg_id;
	int status; 
	MsgStruct msg;
	// ligar a mailbox 
	msg_id = msgget ( 1000, 0666 | IPC_CREAT ); 
	if ( msg_id < 0 ) 
	{
		printf ("Erro na ligacao com a mailbox\n" ); 
		exit(1);
	}
	// receber uma mensagem (bloqueia se nao houver)
	status = msgrcv( msg_id, &msg, sizeof(msg.texto), 1, 0); 
	if ( status < 0 ) 
	{
		printf ("Erro na leitura da mailbox\n" ); 
		exit(1);
	}
	printf("MENSAGEM: %s\n", msg.texto); 
}
