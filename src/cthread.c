/* AUTORES
 Fernando L. Spaniol
 Matheus C. Bica
 Marcelo H. Torres
 */

#define _XOPEN_SOURCE 600 // Devido ao erro de compilacao isto teve que ser adicionado


#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/support.h"
#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>

/* Definir os estados que threads podem ter */
#define CRIACAO 0
#define APTO 1
#define EXECUCAO 2
#define BLOQUEADO 3
#define TERMINO 4


/* VARIAVEIS GLOBAIS */
int tid = 1; // Indica quantas threads já foram criadas e quais indices podem ser usadas
int main_criada = 0; //Flag que guarda a informacao de que a main ja foi criada
int filas_inicializadas = 0;
ucontext_t* exit_context = NULL;

/* THREADS MAIN E EXECUTANDO */
TCB_t* mainThread = NULL; // thread main
TCB_t* exeThread = NULL; // thread sendo executada


/* Criacao das filas */
FILA2 fila_aptos; // Fila das threads que estao aptas
FILA2 fila_bloqueados; // Fila das threads que estao bloqueadas
FILA2 fila_esperando; // Fila com as threads que estão em estado waiting

/* Funções auxiliares */

quemEspera* alguemEsperando(TCB_t* thread); // Função que retorna caso alguém esteja esperando
void dispatcher(); // dispatcher


/* DISPATCHER */
void dispatcher(){
    
    TCB_t *proximaThread = NULL;
    
    FirstFila2(&fila_aptos);
    
    proximaThread = GetAtIteratorFila2(&fila_aptos);
    
    DeleteAtIteratorFila2(&fila_aptos);
    
    setcontext(&proximaThread->context);
    
    
    //int aleatorio = random2();
}



/* Aloca memoria para as filas */

void inicializaFilas(){
    
    CreateFila2(&fila_aptos);
    CreateFila2(&fila_bloqueados);
    CreateFila2(&fila_esperando);
    
    filas_inicializadas = 1;
}

int criarMainThread(){
    
    mainThread = (TCB_t*) malloc(sizeof(TCB_t));
    if (mainThread == NULL)
        return -1;
    
    mainThread->state = EXECUCAO;
    mainThread->tid = 0;
    mainThread->ticket = 0;
    
    getcontext(&mainThread->context);
    exeThread = mainThread;
    
    main_criada = 1;
    return 0;
    
}

void terminarThread(){
    
    quemEspera* quemEspera = alguemEsperando(exeThread);
    
    if (quemEspera != NULL){
        
        DeleteAtIteratorFila2(&fila_esperando);
        quemEspera->esperando->state = APTO;
        AppendFila2(&fila_aptos, quemEspera->esperando);
        
        
    }
    
    free(exeThread->context.uc_stack.ss_sp); // remove as threads
    free(exeThread);
    exeThread = NULL;
    dispatcher();   // Chama o dispatcher novamente
}

/*
	CCREATE
    Cria uma nova thread
*/
int ccreate(void* (*start)(void*), void *arg)
{
    
    if (main_criada == 0){
        criarMainThread();
        
    }
    
    
    if (filas_inicializadas == 0){
        inicializaFilas();
    }
    
    
    TCB_t *newThread = (TCB_t*) malloc(sizeof(TCB_t));
    if (newThread == NULL)
        return -1;
    
    newThread->tid = tid++;
    newThread->state = APTO;
    newThread->ticket = (Random2() % 256); // Valor dummie
    
    //printf("%i", newThread->ticket);
    
    if (exit_context == NULL)
    {
        exit_context = (ucontext_t*) malloc(sizeof(ucontext_t));
        if (exit_context== NULL) return -1; //erro no malloc
        
        exit_context->uc_link = NULL;
        exit_context->uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
        exit_context->uc_stack.ss_size = SIGSTKSZ;
        
        getcontext(exit_context);
        makecontext(exit_context, (void (*)(void)) terminarThread, 0, NULL);
    }
    
    getcontext(&newThread->context);

    newThread->context.uc_link = exit_context;
    newThread->context.uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
    if(newThread->context.uc_stack.ss_sp == NULL)
        return -1; //erro no malloc
    newThread->context.uc_stack.ss_size = SIGSTKSZ;
    
    makecontext(&newThread->context, (void(*)(void))start, 1, arg);
    
    AppendFila2(&fila_aptos, newThread);
    
    return newThread->tid;
}

/*
	CYIELD
	Liberação voluntária da CPU
*/
int cyield(void)
{
    return -1;
}

/*
	CJOIN
	Cthread chamante aguarda o final de outra thread
*/
int cjoin(int tid)
{
	return -1;
}

/*
	CSEM_INIT
	Inicializa o semáforo
*/
int csem_init(csem_t *sem, int count)
{
	return -1;
}

/*
	CWAIT
	Requisita um recurso, se não tiver disponível, fica bloqueado
*/
int cwait(csem_t *sem)
{
	return -1;
}

/*
	CSGINAL
	Sinaliza a liberação de um recurso
*/
int csignal(csem_t *sem)
{
	return -1;
}

/*
    CIDENTIFY
    Identifica os membros do grupo
*/
int cidentify (char *name, int size){
    return -1;
}


/*
 alguemEsperando
 Verifica se tem alguma thread esperando a thread passada 
 */
quemEspera* alguemEsperando(TCB_t* thread){
    FirstFila2(&fila_esperando);
    quemEspera* aux;
    aux = GetAtIteratorFila2(&fila_esperando);
    while(aux != NULL){
        if (aux->sendoEsperado == thread)
            return aux;
        NextFila2(&fila_esperando);
        aux = GetAtIteratorFila2(&fila_esperando);
    }
    return NULL;
}


























