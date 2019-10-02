#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	int descritor[2];
	char msg [20];


	//Cria Pipe(esse caso é unidirecional, ou só lê ou só escreve)
	/*
	        quando fecha o descritor, fecha só para o processo
	        0 o descritor é para a leitura de dados
	        0 escritor 1 e para escrita de dados
	*/
	
	pipe(descritor);
	
	if (fork() > 0){
	//processo pai
	        printf("\nEu sou o Processo Pai %ld\n", getpid());
	        write(descritor[1], msg, strlen(msg)+1);
	        sprintf(msg, "%ld", getpid());
	        read(descritor[0], msg, sizeof(msg)+1);
	        //printf(" O processo filho me enviou a mensagem: %s\n", msg);
	        close(descritor[1]);
	        close(descritor[0]);
	}
	else{
	        //Processo filho
	        //Fecha descritor de escrita
	        msg[0] = "\0";
	        write(descritor[0], msg,strlen(msg)+1);
	        sprintf(msg, "%ld", getpid());  
	        read(descritor[1], msg, sizeof(msg)+1);
	        printf(" O processo pai me enviou a mensagem: %s\n", msg);
	        close(descritor[0]);
	        close(descritor[1]);
	}

	return 0;
}
