/*
Algoritmos de gerenciamento de mem�ria
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Quantidade de frames na mem�ria
#define MEM_SIZE        10
// Constante true
#define true            1
// Constante false
#define false           0
// Tamanho m�ximo da string
#define T_STR           100

// Define se o carga dos processos ser� pelo arquivo de entrada
#define LERARQUIVO      true

/*
Se a pol�tica de pagina��o for global, ser� considerado todo o espa�o de endere�amento
de mem�ria para uma substitui��o de p�gina. Entretanto, se a pol�tica for local, apenas o
espa�o de endere�amento do processo ser� considerado para pagina��o.
*/
#define LOCAL           true
#define GLOBAL          true
// Se a pol�tica for local, POLITICA_PAG deve ser LOCAL, caso contr�rio, GLOBAL
#define POLITITA_PAG    LOCAL

// Quantidade frames de mem�ria
#define FRAMES          5

/*
Tipos de Dados
*/
typedef unsigned int u_int;   // Unsigned int
typedef char string[T_STR];   // Defini��o do tipo string
typedef u_int boolean;        // Defini��o do tipo boolean

typedef struct page_frame {   // Define a estrutura de um frame de mem�ria
  u_int n_frame;              // Endere�o do frame de mem�ria
  clock_t tempo_carga;        // Tempo em que a p�gina foi carregada na mem�ria
  clock_t tempo_acesso;       // Tempo em que a p�gina foi acessada pela �ltima vez
  struct page_frame *next;    // Pr�ximo frame
} t_page_frame;

typedef struct process {      // Define a estrutura do processo
  u_int pid;                  // Id do processo
  u_int ciclos_execucao;      // Quantidade de ciclos a ser executado
  u_int ciclos_executados;    // Quantidade de ciclos excutados
  u_int t_pages;              // Quantidade de p�ginas pertencentes ao processo
  t_page_frame *paginas;      // Lista de p�ginas (j� est� na sequencia de execu��o)
  struct process *next;       // Apontador para o pr�ximo processo
} t_process;

typedef struct proc_list {    // Define a estrutura da lista de processos a ser executada
  t_process *head_proc;       // Processo que esta no in�cio da lista
  t_process *tail_proc;       // Processo que esta no final da lista
  u_int n_of_process;         // Numero de processos
} t_proc_list;

typedef struct FIFO {
  t_page_frame m_frames[FRAMES];
  u_int index_frame;
  u_int page_victim;
} t_FIFO;

/* Trabalho 2 */

/*
    A marcação da página vítima é feita por base em um contador de acessos baseado nos bit de leitura
(R) e no bit de modificação (M), ou seja, quanto menos a página for acessada maior será a chance
de ser substituída.
    O paginador irá incrementar um contador marcando quão velha é a página (contador de idade).
    Quando o sistema necessita substituir páginas, a escolha se dará baseado no contador de idade, quanto mais velha
maior a chance de ser substituída.
*/
typedef struct LRU {
    t_page_frame m_frames[FRAMES];
    clock_t age_counter;
    u_int index_frame;
    u_int page_victim;
}t_LRU;

/*
O algoritmo da segunda chance é uma melhoria do algoritmo FIFO.
Permite alterar a posição de uma página muito antiga na lista que tenha sido referenciada (usada).
Se a página escolhida tiver o valor do bit R em 1, muda-se o valor para zero e então escolhe-se outra página.
Se a página escolhida tiver o valor do bit R em zero, efetua a substituição da página.
*/
typedef struct SC{ //Second Chance
    t_page_frame m_frames[FRAMES];
    u_int page_victim;
    u_int index_frame;
}t_SC;

/*
Prot�tipo das fun��es auxiliares
*/
int menu();
t_proc_list criaListaProcessos();
FILE *abrirArquivoEntrada(char *nomeArq);
t_page_frame *criaPageFrame(int numero);
t_process *criaProcesso();
void carregarProcessos(t_proc_list *l_process, FILE *entrada);
void lerArquivoEntrada(t_proc_list *l_process, char *nomeArq);
void mostrarDadosFrame(t_page_frame *pf); //frames sao paginas
void mostrarDadosPageFrame(t_page_frame *pf);
void mostrarDadosProcesso(t_process *proc);
void listarProcessos(t_proc_list l_process);
void liberarMemoria(t_proc_list *l_process);
void inicializarFrameFIFO(t_FIFO *fifo);
void executaPaginacaoFIFO(t_proc_list *l_process);

//TRABALHO
void inicializarFrameLRU(t_LRU *lru);//Trabalho
void executaPaginacaoLRU(t_proc_list *l_process);//Trabalho
void inicializarFrameSC(t_SC *sc);//Trabalho
void executaPaginacaoSegundaChance(t_proc_list *l_process);//Trabalho

boolean verificaPaginaAlocada(t_page_frame *pf, t_FIFO fifo);

boolean verificaPaginaAlocada_LRU(t_page_frame *pf, t_LRU lru);//Trabalho
boolean verificaPaginaAlocada_SC(t_page_frame *pf, t_SC sc);//Trabalho

/*
Prot�tipo dos algoritmos de pagina��o
*/
int FIFO_page_replacement(t_process *process);

int LRU_page_replacement(t_process *process);//Trabalho
int Second_Chance_page_replacement(t_process *process);//Trabalho


/*
Fun��o principal
*/
int main()
{
  // Lista de processos que ser�o carregados na mem�ria
  t_proc_list l_process = criaListaProcessos();
  #if LERARQUIVO
    string nomeArq;
    printf("Nome do arquivo de entrada? ");
    fflush(stdin);
    gets(nomeArq);
    lerArquivoEntrada(&l_process, nomeArq);
  #endif

  while (true) {

    switch (menu()) {

      case 1:
              break;

      case 2: if (l_process.head_proc)
                listarProcessos(l_process);
              else
                printf("\nA lista de processos esta vazia\n\n");
              break;

      case 3: if (l_process.head_proc)
                executaPaginacaoFIFO(&l_process);
              else
                printf("\nA lista de processos esta vazia\n\n");
              break;

      case 4: if (l_process.head_proc)
                executaPaginacaoLRU(&l_process);
              else
                printf("\nA lista de processos esta vazia\n\n");
              break;

      case 5: if (l_process.head_proc)
                executaPaginacaoSegundaChance(&l_process);
              else
                printf("\nA lista de processos esta vazia\n\n");
              break;

      case 6: if (l_process.head_proc)
                liberarMemoria(&l_process);
              else
                printf("\nA lista de processos esta vazia\n\n");
              break;

      case 0: liberarMemoria(&l_process);
              exit(0);
    }

  }

  return 0;
}

/*
Implementa��o das fun��es auxiliares
*/
int menu()
{
  int opc;

  printf("1.Inserir processos manualmente\n");
  printf("2.Mostrar processos carregados\n");
  printf("3.Executar simulacao com algoritmo FIFO\n");
  printf("4.Executar simulacao com algoritmo LRU\n");
  printf("5.Executar simulacao com algoritmo Segunda Chance\n");
  printf("6.Liberar memoria (zerar lista de processos)\n");
  printf("0.Sair do programa\n");
  printf("?: ");
  scanf("%d", &opc);

  return opc;
}

FILE *abrirArquivoEntrada(char *nomeArq)
{
  FILE *fp;

  if (!(fp = fopen(nomeArq, "r")))
    return NULL;

  return fp;
}

t_proc_list criaListaProcessos()
{
  t_proc_list list;

  list.head_proc     = NULL;
  list.tail_proc     = NULL; //tail rabo
  list.n_of_process  = 0;

  return list;
}

t_page_frame *criaPageFrame(int numero)
{
  // Aloca mem�ria para a pagina/frame
  t_page_frame *pf = (t_page_frame*)malloc(sizeof(t_page_frame));

  pf->n_frame      = numero;
  pf->tempo_carga  = 0;
  pf->tempo_acesso = 0;
  pf->next         = NULL;

  return pf;
}

t_process *criaProcesso()
{
  int i;
  // Aloca memoria para o processo
  t_process *proc = (t_process*)malloc(sizeof(t_process));

  //proc->ciclos_executados = rand() % proc->ciclos_execucao;//
  proc->ciclos_executados = 0;
  proc->t_pages           = 0;
  proc->next              = NULL;
  proc->paginas           = NULL;

  return proc;
}

void carregarProcessos(t_proc_list *l_process, FILE *entrada)
{
  t_process     *processo;
  t_page_frame  *pf, *aux;
  int            pagina;

  while (!feof(entrada)) {
    aux = NULL;
    processo = criaProcesso();
    fscanf(entrada, "%d", &processo->pid);//primeira linha do arquivo
    fscanf(entrada, "%d", &processo->ciclos_execucao);// segunda linha

    // Carrega p�ginas dos processos
    do {
      fscanf(entrada, "%d ", &pagina);// terceira linha do arq
      if (pagina) {  // Se p�gina for 0, indica fim de listagem de p�ginas
        pf = criaPageFrame(pagina);
        if (!processo->paginas) {
          processo->paginas = pf;
          aux = processo->paginas;
        }
        else {
          aux->next = pf;
          aux = pf;
        }
        processo->t_pages++;
      }
    } while (pagina);

    // Insere processo na fila de processos
    if (!l_process->head_proc) {
      l_process->head_proc = processo;
    }
    else {
      l_process->tail_proc->next = processo;
    }
    l_process->tail_proc = processo;
    l_process->n_of_process++;

  }
}

void lerArquivoEntrada(t_proc_list *l_process, char *nomeArq)
{
  FILE *fp = abrirArquivoEntrada(nomeArq);

  if (!fp) {
    fprintf(stderr, "Erro ao abrir arquivo %s\n", nomeArq);
  }
  else {
    carregarProcessos(l_process, fp);
    close(fp);
  }
}

void mostrarDadosFrame(t_page_frame *pf)
{
  printf("Numero de frame....: %d\n", pf->n_frame);
  printf("Tempo de carga.....: %u\n", pf->tempo_carga);
  printf("Tempo de acesso....: %u\n\n", pf->tempo_acesso);
}

void mostrarDadosPageFrame(t_page_frame *pf)
{

  while (pf) {
    mostrarDadosFrame(pf);
    pf = pf->next;
  }

}

void mostrarDadosProcesso(t_process *proc)
{

  printf("\n");
  printf("Identificacao do processo.....: %d\n", proc->pid);
  printf("Ciclos de execucao............: %d\n", proc->ciclos_execucao);
  printf("Ciclos de executados..........: %d\n", proc->ciclos_executados);
  printf("Quantidade de paginas.........: %d\n", proc->t_pages);
  printf("********** Paginas **********\n");
  mostrarDadosPageFrame(proc->paginas);
  printf("\n");

}

void listarProcessos(t_proc_list l_process)
{
  t_process *proc = l_process.head_proc;

  while (proc) {
    mostrarDadosProcesso(proc);
    proc = proc->next;
  }
}

void liberarMemoria(t_proc_list *l_process)
{
  t_page_frame *pf      = NULL;
  t_process    *proc    = NULL;

  // Enquanto existirem processos na lista de processos
  while (l_process->head_proc) {
    proc = l_process->head_proc;
    pf   = proc->paginas;

    // Libera mem�ria das p�ginas dos processos
    while (proc->paginas) {
      // Avan�a uma unidade da lista de p�ginas do processo
      proc->paginas = proc->paginas->next;
      // Libera mem�ria da lista de p�ginas do processo
      free(pf);
      // Corrigi ponteiro de p�ginas
      pf = proc->paginas;
    }

    // Avan�a uma unidade na lista de processos
    l_process->head_proc = l_process->head_proc->next;
    // Libera mem�ria do processo
    free(proc);
  }

  // Recria lista de processos
  *l_process = criaListaProcessos();
}

void inicializarFrameFIFO(t_FIFO *fifo)
{
  int i;

  // Zera todos os frames de mem�ria
  for (i = 0; i < FRAMES; i++)
    fifo->m_frames[i].n_frame = 0;

  // Inicializa �ndices
  fifo->index_frame = 0;
  fifo->page_victim = 0;

}

//TRABALHO
void inicializarFrameLRU(t_LRU *lru){
    int i;

    //zera todos os frames de memooria
    for (i = 0; i < FRAMES; i++){
      lru->m_frames[i].n_frame = 0;

    }
    //Inicializar indices
    lru->age_counter = 0;
    lru->page_victim = 0;
    lru->index_frame = 0;
}

//TRABALHO
void inicializarFrameSC(t_SC *sc){
  int i;

  for (i = 0; i < FRAMES; i++)
    sc->m_frames[i].n_frame = 0;

  sc->index_frame = 0;
  sc->page_victim = 1;
}

void executaPaginacaoFIFO(t_proc_list *l_process)
{
  FILE *f_saida;
  t_process *process = NULL;


  // Abre arquivo de sa�da
  if (!(f_saida = fopen("FIFO.txt", "w+"))) {
    printf("N�o foi possivel criar arquivo FIFO.txt\n");
    return;
  }

  // Cabe�alho do arquivo
  fprintf(f_saida,"*************************************************************\n");
  fprintf(f_saida,"    Relatorio de troca de paginas com o algoritmo FIFO\n");
  fprintf(f_saida,"*************************************************************\n\n");

  // Executa processos
  process = l_process->head_proc;
  while (process) {
    fprintf(f_saida, "Processo........................: %d\n", process->pid);
    fprintf(f_saida, "Quantidade de paginas trocadas..: %d\n\n", FIFO_page_replacement(process));

    // Seleciona outro processo
    process = process->next;
  }

  // Fecha o arquivo de sa�da
  fclose(f_saida);

}

//  TRABALHO
//Aqui os relatorios são gerados
void executaPaginacaoLRU(t_proc_list *l_process)
{
  FILE *f_saida;
  t_process *process = NULL;
  int i;

  // Abre arquivo de sa�da
  if (!(f_saida = fopen("LRU.txt", "w+"))) {
    printf("Nao foi possivel criar arquivo LRU.txt\n");
    return;
  }

  // Cabe�alho do arquivo
  fprintf(f_saida,"*************************************************************\n");
  fprintf(f_saida,"    Relatorio de troca de paginas com o algoritmo LRU\n");
  fprintf(f_saida,"*************************************************************\n\n");

    // Executa processos
  process = l_process->head_proc;

  while (process) {
    fprintf(f_saida, "Processo........................: %d\n", process->pid);

    fprintf(f_saida, "Paginas trocadas..: %d\n\n", LRU_page_replacement(process));



    // Seleciona outro processo
    process = process->next;
  }

  // Fecha o arquivo de sa�da
  fclose(f_saida);

}

// TRABALHO
void executaPaginacaoSegundaChance(t_proc_list *l_process)
{
  FILE *f_saida;
  t_process *process = NULL;


  // Abre arquivo de sa�da
  if (!(f_saida = fopen("SECOND_CHANCE.txt", "w+"))) {
    printf("N�o foi possivel criar arquivo SECOND_CHANCE.txt\n");
    return;
  }

  // Cabe�alho do arquivo
  fprintf(f_saida,"*************************************************************\n");
  fprintf(f_saida,"    Relatorio de troca de paginas com o algoritmo SECOND CHANCE\n");
  fprintf(f_saida,"*************************************************************\n\n");

  // Executa processos
  process = l_process->head_proc;
  while (process) {
    fprintf(f_saida, "Processo........................: %d\n", process->pid);
    fprintf(f_saida, "Quantidade de paginas trocadas..: %d\n\n", Second_Chance_page_replacement(process));

    // Seleciona outro processo
    process = process->next;
  }

  // Fecha o arquivo de sa�da
  fclose(f_saida);

}

boolean verificaPaginaAlocada(t_page_frame *pf, t_FIFO fifo)
{
  int i;

  for (i = 0; i < FRAMES; i++) {
    if (pf->n_frame == fifo.m_frames[i].n_frame)
      return true;
  }

  return false;
}
//TRABALHO
boolean verificaPaginaAlocada_LRU(t_page_frame *pf, t_LRU lru)
{
  int i;

  for (i = 0; i < FRAMES; i++) {
    //printf("%d",i);
    if (pf->n_frame == lru.m_frames[i].n_frame)
      return true;
  }

  return false;
}

//TRABALHO
boolean verificaPaginaAlocada_SC(t_page_frame *pf, t_SC sc)
{
  int i;

  for (i = 0; i < FRAMES; i++) {
    if (pf->n_frame == sc.m_frames[i].n_frame)
      return true;
  }

  return false;
}


/*
Implementa��o dos algoritmos de pagina��o
*/
int FIFO_page_replacement(t_process *process)
{
  t_FIFO fifo;
  inicializarFrameFIFO(&fifo);

  clock_t clock_start = clock();
  u_int troca_paginas = 0;

  while (process->paginas) {
    // Verifica se a p�gina j� n�o est� na mem�ria
    if (!verificaPaginaAlocada(process->paginas, fifo)) {
      // Carrega p�gina na mem�ria
      fifo.m_frames[fifo.index_frame] = *(process->paginas);
      fifo.m_frames[fifo.index_frame].tempo_carga = (clock() - clock_start) / CLOCKS_PER_SEC;
      fifo.m_frames[fifo.index_frame].tempo_acesso = (clock() - clock_start) / CLOCKS_PER_SEC;

      // Ajuste do �ndice de frames (fila)
      fifo.index_frame = (fifo.index_frame + 1) % FRAMES;

      // Verifica se a p�gina ser� substitu�da por outra
      if ((fifo.index_frame == fifo.page_victim) && process->paginas->next) {
        troca_paginas++;
        // Redefine p�gina v�tima (inicialmente � zero)
        fifo.page_victim = (fifo.page_victim + 1) % FRAMES;
      }
    }
    else {
      if (!process->paginas->next)
        troca_paginas--;
    }

    // Define outra p�gina que ser� carregada na mem�ria
    process->paginas = process->paginas->next;
  }

  // Retorna o n�mero de troca de p�ginas
  return troca_paginas;
}

int LRU_page_replacement(t_process *process)
{
  t_LRU lru;
  inicializarFrameLRU(&lru);

  clock_t clock_start = clock();
  u_int troca_paginas = 0;

  clock_t old = 0;


    while (process->paginas) {
    // Verifica se a p�gina j� n�o est� na mem�ria
    if (!verificaPaginaAlocada_LRU(process->paginas, lru)) {
      // Carrega p�gina na mem�ria
      lru.m_frames[lru.index_frame] = *(process->paginas);
      lru.m_frames[lru.index_frame].tempo_carga = (clock() - clock_start) / CLOCKS_PER_SEC;
      lru.m_frames[lru.index_frame].tempo_acesso = (clock() - clock_start) / CLOCKS_PER_SEC;

      lru.age_counter = (lru.m_frames[lru.index_frame].tempo_acesso - lru.m_frames[lru.index_frame].tempo_carga);

      //preciso de um ponteiro pra pagina atual
      //percorrer a lista e verificar maior clock()- (tempo de acesso lru.m_frames[lru.index_frame].tempo_acesso)
      //salvo em uma variavel, removo ela e faço isso de novo ate o fim do arquivo

      //ajuste do indice de frames
      lru.index_frame = (lru.index_frame + 1) % FRAMES;

      if((lru.m_frames[lru.index_frame].tempo_acesso - lru.m_frames[lru.index_frame].tempo_carga) > old){
          old = lru.age_counter;
          //redefine pagina vitima
          lru.page_victim = lru.index_frame;
          troca_paginas++;
      }
      }else {
          if (!process->paginas->next)
            troca_paginas--;
      }

    // Define outra p�gina que ser� carregada na mem�ria
    process->paginas = process->paginas->next;
  }
  return troca_paginas;
}

int Second_Chance_page_replacement(t_process *process)
{
  t_SC sc;
  inicializarFrameSC(&sc);

  clock_t clock_start = clock();
  u_int troca_paginas = 0;

    while (process->paginas) {

      // Verifica se a p�gina j� n�o est� na mem�ria
      if (!verificaPaginaAlocada_SC(process->paginas,sc)) {
        // Carrega p�gina na mem�ria
        sc.m_frames[sc.index_frame] = *(process->paginas);
        sc.m_frames[sc.index_frame].tempo_carga = (clock() - clock_start) / CLOCKS_PER_SEC;
        sc.m_frames[sc.index_frame].tempo_acesso = (clock() - clock_start) / CLOCKS_PER_SEC;


        // Ajuste do �ndice de frames (fila)
        sc.index_frame = (sc.index_frame + 1) % FRAMES;

        // Verifica se a p�gina ser� substitu�da por outra
          if((sc.page_victim == 1)){//se estiver em 1, troco por zero
            sc.page_victim = 0;
          }
          else{
                if ((sc.page_victim == 0) && process->paginas->next) {
                  troca_paginas++;
                  // Redefine p�gina v�tima (inicialmente � zero)
                  sc.page_victim = (sc.page_victim + 1) % FRAMES;
                }}
        }else {
              if (!process->paginas->next)
                  troca_paginas--;
              }
            // Define outra p�gina que ser� carregada na mem�ria
          process->paginas = process->paginas->next;
    }
  return troca_paginas;
}



