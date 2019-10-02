# Arquitetura_de_Sistemas_Operacionais

Em Gerenciamento_de_Memoria.c há a implementação dos principais algoritmos de gerenciamento de memória por paginação, 
bem como LRU, segunda chance e FIFO.


Em PIPE.c há a criação de um pipe de comunicação entre um processo pai e um processo filho por meio de um descritor.


Em fumantes.c há a resolução do seguinte problema:
Considere um sistema com três processos fumante e um processo agente. 
Cada fumante está continuamente enrolando um cigarro e fumando-o em seguida. Mas para enrolar e fumar um cigarro, 
o fumante precisa de três ingredientes: fumo, papel e fósforo. Um dos processos fumante tem papel, outro tem fumo 
e o terceiro tem os fósforos. O agente tem um suprimento infinito de todos os três materiais. O agente coloca 
dois dos ingredientes na mesa. O fumante que tem o outro ingrediente faz o cigarro e o fuma, sinalizando o agente 
na conclusão. O agente coloca outros dois dos ingredientes e o ciclo se repete. 
Escreva um programa que sincronize o agente e os fumantes. Desenvolva uma solução utilizando semáforos.


Em Semaforo.c há o problema típico o qual dois processos, um de leitura e outro de escrita, disputam por um recurso,
utilizar o buffer, e o semáforo é responsável por gerenciar o uso do recurso entre os processos.

...

P.S. Os outros códigos ainda estão sendo verificados.
