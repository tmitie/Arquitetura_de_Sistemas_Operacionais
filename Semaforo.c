#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<windows.h>
#define MAX_MSG 10

typedef unsigned int u_int;
typedef void* THREAD;

typedef struct B{
    u_int msg[MAX_MSG];
    u_int msgSize;
    u_int posRead;
    u_int posWrite;
}buffer_t;

sem_t bufferControl;
buffer_t MSGS;

THREAD thWrite();
THREAD thRead();

int main(){

    //Criando threads para leitor e escritor
    pthread_t tid_Reader, tid_Writer;

    //Inicializar semaforo
    /*  PARAMETROS = int sem_init(sem_t *sem, int pshared, unsigned int value);
    If the pshared argument has a non-zero value, then the semaphore is shared between processes; in this case, any process that can access
    the  semaphore sem can use sem for performing sem_wait(), sem_trywait(), sem_post(), and sem_destroy() operations.
    */
    sem_init(&bufferControl, 0, 1);

    //Inicializando como o rand deve se basear
    srand(time(NULL));

    //Inicializar buffer de mensagens
    MSGS.posRead = 0;
    MSGS.posWrite = 0;
    MSGS.msgSize = 0;

    //Criar Threads
    pthread_create(&tid_Writer,NULL,thWrite,NULL);//Escritor
    pthread_create(&tid_Reader,NULL,thRead,NULL);//Leitor

    //Começar a escrever
    pthread_join(tid_Writer,NULL);

    return 0;
}

THREAD thWrite(){

    u_int data;

    while(1){//Não vai funcionar eternamente por causa do semaforo

        sem_wait(&bufferControl);

        data = rand() % 223;

        if(MSGS.msgSize < MAX_MSG){
            printf("\n Escrevendo na posicao: %d",MSGS.posWrite);
            printf("\n Valor: %d\n", data);
            MSGS.msg[MSGS.posWrite] = data;
            MSGS.msgSize ++;
            MSGS.posWrite = (MSGS.posWrite + 1) % MAX_MSG;
        }

        sem_post(&bufferControl);
        usleep(200);
    }

}

THREAD thRead(){

    u_int data; //Como só vai ler, não é necessario adicionar entrada

    while(1){

        sem_wait(&bufferControl);

        if(MSGS.msgSize > 0){
            data = MSGS.msg[MSGS.posRead]; //Como os valores já foram escritos no thWrite, só atribuo eles para data. Não precisa ser armazenado
            printf("\n Lendo da Posição: %d",MSGS.posRead);
            printf("\n Valor: %d\n", data);
            MSGS.msgSize --;
            MSGS.posRead = (MSGS.posRead + 1) % MAX_MSG;
        }

        sem_post(&bufferControl);
        usleep(200);
    }
}




























