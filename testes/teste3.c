	#include <stdio.h>
#include <stdlib.h>
#include "../include/fila2.h"
#include "../include/cthread.h"

void* func1(void *arg) {
	
	printf("3 - A thread filha esta executando agora e ira tentar alocar o recurso\n");
	//A thread filha ira tentar utilizar o recurso
	//como a main o está utilizando, a thread filha sera bloqueada
	cwait(arg);
	
	printf("5 - Sou a thread filha e estou utilizando o recurso agora\n");
	csignal(arg);
	printf("6 - Sou a thread filha e nao estou mais utilizando o recurso\n");
	
	return (void*)NULL;
}

int main(int argc, char *argv[]){
	
	int t=0;
	int count = 1; //quantidade de recursos do semáforo disponíveis
	csem_t semaforo;
	
	//inicializacao do semaforo
	csem_init(&semaforo, count);
	
	printf("1- A main esta alocando o recurso\n");
	cwait(&semaforo);
	
	t = ccreate(func1, (void*)&semaforo);
	printf("2 - A main esta liberando a cpu, mas o recurso ainda esta alocada para ela\n");
	cyield();
	
	printf("4 - A execucao voltou para a main que ira desalocar o recurso e liberar a cpu\n");
	csignal(&semaforo);
	cyield();
	
	printf("7 - A execucao voltou para a main e o recurso nao esta sendo utilizado por ninguem\n");
	printf("8 - Programa encerrado com sucesso!\n");
	
	return 0;
}
