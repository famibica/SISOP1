#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/fila2.h"
#include "../include/cthread.h"

//Recebe um inteiro e imprime ele e os dez próximos números
void* ImprimeDez(void *arg) {

	int i=0;

	for(i=0; i<10; i++){

		printf("%d ", (*(int*)arg)+i);
	}

		printf("\n");

	return (void*)NULL;
}

void* SinalizaFim(void *arg){

	//Essa thread so vai começar executar após a thread1 acabar
	cjoin((*(int*)arg));

	printf("\nValores impressos, retornando para main");

	return (void*)NULL;
}


int main(int argc, char *argv[]){

	int t1, t2;
	int valor = atoi(argv[0]);

	t1 = ccreate(ImprimeDez, (void*)&valor); //criacao da thread 1
	t2 = ccreate(SinalizaFim, (void*)&t2);	//criacao da thread 2

	//A execução somente volta para main apos a thread2 acabar
	cjoin(t2);

	printf("\nPrograma encerrado com sucesso!");

	return 0;
}
