/*
problemas fumantes exer 23
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int sleep_agente, sleep_fumante;

typedef void* THREAD;
//sem_t INSUMOS[3] = {0,0,0}; // pos 0 papel; pos 1 fosforo; pos 2 fumo
char* msgs[3] = {"PAPEL","FOSFORO","FUMO"};

sem_t mutex_insumos;

int INSUMOS[3] = {0,0,0};

THREAD agente();
THREAD fumante(void* id);
//void inicializa_sem();

void mostraEstoque();

int main(int argc, char** argv){
	int i;
	pthread_t tids[4]; //1 agente e 3 fumantes;


	if(argc != 3)
	{
        printf("%s sleep_agente sleep_fumante\n", argv[0]);
        exit(-1);
	}

	sleep_agente = atoi(argv[1]);
	sleep_fumante = atoi(argv[2]);

    //Inicializa semaforo
	sem_init(&mutex_insumos, 0, 1);

	srand(time(NULL));

	//cria fumantes
	for(i = 0; i < 3; i++)
		pthread_create(&tids[i], NULL, fumante, (void*)i);

	//cria agente
	pthread_create(&tids[i], NULL, agente, NULL);

	pthread_join(&tids[i], NULL);

	return 0;
}


THREAD agente(){

	int id_insumo_1,id_insumo_2;

	while(1){
		id_insumo_1 = rand() % 3;


        sem_wait(&mutex_insumos);
        INSUMOS[id_insumo_1] += 1;


		while((id_insumo_2 = rand() % 3) == id_insumo_1);
		INSUMOS[id_insumo_2] += 2;
		sem_post(&mutex_insumos);


		printf("Agente produziu %s e %s  \n", msgs[id_insumo_1], msgs[id_insumo_2]);


        mostraEstoque();

		sleep(sleep_agente);


	}


}



THREAD fumante(void* id){
	int quem_sou_eu = (int)id;

	//printf("quem sou eu %d \n", quem_sou_eu);

	while(1){
		switch (quem_sou_eu){
			case 0: 	//tenho papel e necessito fosforo e fumo
				sem_wait(&mutex_insumos);

				if(INSUMOS[1]> 0 && INSUMOS[2] > 0)
				{
                    INSUMOS[1] -= 1;
                    INSUMOS[2] -= 1;
                    printf("Fumante %s fumando ...\n", msgs[quem_sou_eu]);
				}
				sem_wait(&mutex_insumos); // tenta pegar fosforo

				//printf("Fumante PAPEL fumando...\n");
				//sleep(2);
				break;

			case 1:		//tenho fosfoto e necessito papel e fumo
                sem_wait(&mutex_insumos);

				if(INSUMOS[0] > 0 && INSUMOS[2] > 0)
				{
                    INSUMOS[0] -= 1;
                    INSUMOS[2] -= 1;
                    printf("Fumante %s fumando ...\n", msgs[quem_sou_eu]);
				}
				sem_wait(&mutex_insumos); // tenta pegar fosforo

				//printf("Fumante PAPEL fumando...\n");
				//sleep(2);
				break;

			case 2:		//tenho fumo e necessito fosforo e papel
						sem_wait(&mutex_insumos);

				if(INSUMOS[0] > 0 && INSUMOS[1] > 0)
				{
                    INSUMOS[0] -= 1;
                    INSUMOS[1] -= 1;
                    printf("Fumante %s fumando ...\n", msgs[quem_sou_eu]);
				}
				sem_wait(&mutex_insumos); // tenta pegar fosforo

				//printf("Fumante PAPEL fumando...\n");
				//sleep(2);
				break;

		//fumante vai dormir, esta cansado
		sleep(sleep_fumante);
	}
}}

void mostraEstoque()
{
     printf("..Estoque de insumos: \n");
     printf("--> Papel...: %d\n", INSUMOS[0]);
     printf("--> Fosforo.: %d\n", INSUMOS[1]);
     printf("--> Fumo....: %d\n", INSUMOS[3]);
}


/*void inicializa_sem(){

	for(int i = 0; i < 3; i++)
		sem_init(&INSUMOS[i],0,0);

}*/
