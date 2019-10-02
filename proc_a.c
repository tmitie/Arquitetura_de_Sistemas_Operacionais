#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>

int main(int argc, char** argv){

    int chave, id_mem, *ptr_sh, i;

    if (argc < 2){
        printf("%s chave\n", argv[0]);
        exit(0);
    }

    chave = atoi(argv[1]);

    //criar a area de memoria compartilhada

    id_mem = shmget(chave, sizeof(int)*10, 0777|IPC_CREAT);

    printf("O ID da area : %d\n", id_mem);

    //Associar a área de memoria ao processo

    ptr_sh = (int*)shmat(id_mem, NULL, 0);

    //Escrever na area compartilhada

    for(i = 0; i < 10; i++){
        *(ptr_sh++) = 1;
    }

    return 0;

}
