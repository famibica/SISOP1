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

/*
	CCREATE
    Cria uma nova thread
*/
int ccreate(void* (*start)(void*), void *arg)
{
    
    TCB_t *newThread = (TCB_t*) malloc(sizeof(TCB_t));
    newThread->tid = tid++;
    newThread->state = APTO;
    newThread->ticket = 1; // Valor dummie enquanto espera a funcao do cechin
    //newThread->context = NULL; // nao sei que merda eh essa
    
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

