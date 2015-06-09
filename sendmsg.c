#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
typedef struct 
{ 
	long int tipo;
	char texto[250]; 
}MsgStruct;
main() 
{
	int msg_id;
	int status; 
	MsgStruct msg;
	// ligar a mailbox
	msg_id = msgget ( 1000, 0600 | IPC_CREAT ); 
	if ( msg_id < 0 ) 
	{
		printf ("Erro na ligacao com a mailbox\n" ); 
		exit(1);
	}
	// enviar mensagem
	msg.tipo = 1;
	strcpy( msg.texto, "Ola");
	status = msgsnd( msg_id, &msg, sizeof(msg.texto), 0); 
	if ( status < 0 ) 
	{
		printf ("Erro no envio da msg para a mailbox\n" ); 
		exit(1);
	}
	printf ("Mensagem enviada!\n"); 
}
