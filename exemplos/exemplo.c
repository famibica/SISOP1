
/*
 *	Programa de exemplo de uso da biblioteca cthread
 *
 *	Versão 1.0 - 14/04/2016
 *
 *	Sistemas Operacionais I - www.inf.ufrgs.br
 *
 */

#include "../include/support.h"
#include "../include/cthread.h"
#include <stdio.h>

int	id0, id1;

void* func0(void *arg) {
	printf("Eu sou a thread ID0 imprimindo %d\n", *((int *)arg));
    
    cjoin(id1);
    
    return 0;
}

void* func1(void *arg) {
	printf("Eu sou a thread ID1 imprimindo %d\n", *((int *)arg));
    
    return 0;
}

int main(int argc, char *argv[]) {
	int i;

	id0 = ccreate(func0, (void *)&i);
	id1 = ccreate(func1, (void *)&i);

	printf("Eu sou a main apos a criacao de ID0 e ID1\n");

	cjoin(id0);
	cjoin(id1);

	printf("Eu sou a main voltando para terminar o programa\n");
    
    return 0;
}

