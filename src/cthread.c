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


/* Criacao das filas */
FILA2 fila_aptos; // Fila das threads que estao aptas
FILA2 fila_bloqueados; // Fila das threads que estao bloqueadas

void inicializaFilas(){
    CreateFila2(&fila_aptos); // Aloca a memoria para as filas
    CreateFila2(&fila_bloqueados);
    filas_inicializadas = 1;
    
}

int criarMainThread(){
    
    return 1;
    
}

int terminateThread(){
    
    return 1;
    
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
    newThread->tid = tid++;
    newThread->state = APTO;
    newThread->ticket = Random2(); // Valor dummie
    
    //printf("%i", newThread->ticket);
    
    getcontext(&newThread->context);
    
    if (exit_context == NULL)
    {
        exit_context = (ucontext_t*) malloc(sizeof(ucontext_t));
        if (exit_context== NULL) return -1; //erro no malloc
        
        exit_context->uc_link = NULL;
        exit_context->uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
        exit_context->uc_stack.ss_size = SIGSTKSZ;
        
        getcontext(exit_context);
        makecontext(exit_context, (void (*)(void)) terminateThread, 0, NULL);
    }
    
    newThread->context.uc_link = exit_context;
    newThread->context.uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
    if(newThread->context.uc_stack.ss_sp == NULL) return -1; //erro no malloc
    newThread->context.uc_stack.ss_size = SIGSTKSZ;
    
    makecontext(&newThread->context, (void(*)(void))start, 1, arg);
    
    
    tid++;
    
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

