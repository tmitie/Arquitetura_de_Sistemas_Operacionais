#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main()
{
        
        char msg[20];
        int des_servidor, des_cliente;        
        
        //Abre o pipe do servidor
        des_servidor = open("pipe.servidor", O_RDONLY);//read only
        
        //Abre o pipe do cliente
        des_cliente = ("pipe.cliente", O_WRONLY);//write only
        
       
        read(des_servidor,msg,sizeof(msg));
        printf("O Servidor me disse: %s\n", msg);
	write(des_cliente, "Obrigada Servidor\n\0",strlen("Obrigada Servidor\n")+1);
        

        return 0;
        
}

