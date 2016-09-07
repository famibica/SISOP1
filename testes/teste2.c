#include <stdio.h>
#include <stdlib.h>
#include "../include/fila2.h"
#include "../include/cthread.h"

void* VerificaTres(void *arg);

void* VerificaDois(void *arg) {

	int valor=0;
	int t2=0;
	valor = (*(int*)arg);

	if((valor%2)==0){
		printf("O numero %d eh divisivel por 2\n", valor);
	}
	else{
		printf("O numero %d nao eh divisivel por 2\n", valor);
	}

	//A thread1 cria uma segunda thread a libera a CPU
	//Como a main está travando esperando a thread 1  acabar, a execucao passa para a thread 2 criada
	t2 = ccreate(VerificaTres, (void*)&valor); // criacao da thread 2
	cyield();

	//Nesse ponto toda a thread 2 foi executada e a execucao voltou para a thread 1
	if((valor%2)==0){
		printf("A divisao de %d por 2 resulta em %d\n", valor, valor/2);
	}
	else{
		printf("A divisao de %d por 2 resulta em %d e resto 1\n", valor, valor/2);
	}

	return (void*)NULL;
}

void* VerificaTres(void *arg) {

	int valor=0;
	valor = (*(int*)arg);

	if((valor%3)==0){
		printf("O numero %d eh divisivel por 3\n", valor);
	}
	else{
		printf("O numero %d nao eh divisivel por 3\n", valor);
	}

	if((valor%3)==0){
		printf("A divisao de %d por 3 resulta em %d\n", valor, valor/3);
	}
	else{
		printf("A divisao de %d por 3 resulta em %d e resto %d\n", valor, valor/3, (valor%3));
	}

	return (void*)NULL;
}

int main(int argc, char *argv[]){

	int t1 = 0;
	int valor = atoi(argv[0]);

	t1 = ccreate(VerificaDois, (void*)&valor); //criacao da thread 1

	//A main ira parar de executar ate que a thread 1 termine sua execucao
	cjoin(t1);

	printf("Programa encerrado com sucesso!\n");

	return 0;
}
