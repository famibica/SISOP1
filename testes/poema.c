/*
	Imprime primeiro o poema 1, e, após, o poema 2.

	DESCRIÇÃO DA EXECUÇÃO:
		A thread main cria as threads imprimePoema1 e imprimePoema2
			e entra em loop de cyield() enquanto stop_yielding==0.
		A thread imprimePoema1 reserva o iterador, imprime seu poema,
			reseta o iterador e dá cjoin() na imprimePoema2.
		A imprimePoema2 imprime seu poema e reseta o iterador.
		Quando imprimePoema2 termina, a imprimePoema1 seta a flag
			stop_yielding e termina.
		A thread main termina o loop de cyield() e termina.
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cthread.h"
#include "../include/fila2.h"

#define SIZE 20;

int iterador_compartilhado = 0;
int stop_yielding = 0;
csem_t semaforo;

void* imprimePoema1(void *arg)
{
	char *poema[4];
	poema[0] = "O poeta é um fingidor";
	poema[1] = "Finge tão completamente";
	poema[2] = "Que chega a fingir que é dor";
	poema[3] = "A dor que deveras sente.";

	cwait(&semaforo);

	printf("\n");
	for(; iterador_compartilhado < 4; iterador_compartilhado++) 
	{
		puts(poema[iterador_compartilhado]);
		cyield();
	}
	printf("---------");
	iterador_compartilhado = 0;
	csignal(&semaforo);

	cjoin(*((int *)arg));

	stop_yielding = 1;

	return NULL;
}

void* imprimePoema2(void *arg)
{
	cyield();
	char *poema[4];
	poema[0] = "Todos estes que aí estão";
	poema[1] = "Atravancando o meu caminho,";
	poema[2] = "Eles passarão.";
	poema[3] = "Eu passarinho!";

	cwait(&semaforo);

	printf("\n");
	for(; iterador_compartilhado < 4; iterador_compartilhado++) 
	{
		puts(poema[iterador_compartilhado]);
		cyield();
	}
	printf("\n");
	iterador_compartilhado = 0;
	csignal(&semaforo);

	return NULL;
}

int main()
{	
	int id1;
	int x = 0;

	csem_init(&semaforo, 1);

	id1 = ccreate(imprimePoema2, (void*)&x);
	ccreate(imprimePoema1, (void*)&id1);
	

	while (stop_yielding != 1)
		cyield();

	return 0;
}