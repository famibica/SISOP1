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
ucontext_t* contextoDispatcher = NULL;

/* THREADS MAIN E EXECUTANDO */
TCB_t* mainThread = NULL; // thread main
TCB_t* exeThread = NULL; // thread sendo executada


/* Criacao das filas */
static FILA2 fila_aptos; // Fila das threads que estao aptas
static FILA2 fila_bloqueados; // Fila das threads que estao bloqueadas
static FILA2 fila_esperando; // Fila com as threads que estão em estado waiting

/* Funções auxiliares */

quemEspera* alguemEsperando(TCB_t* thread); // Função que retorna caso alguém esteja esperando
void dispatcher(); // dispatcher, escolhe as threads futuras
int tidEsperado(int tid); // ve se o tid ja ta sendo esperado
TCB_t* acharTCB(int tid); // Encontra o TCB
int removerBloqueada(TCB_t* thread); // Remove da fila de bloqueados e bota na fila de Aptos
int diferenca(int x, int y); // Calcula o modulo da diferenca
TCB_t* acharProximaThread(int x); // Ve a thread com a menor diferenca de ticket com o int x
void removeFilaAptos(int tid); // Remove da fila de aptos e poe na bloqueada
int jaEsperada(int tid); // ve se o tid ja esta sendo esperado
int criarContextoDispatcher(); // Cria o contexto de dispatcher para nao chamar dispatcher()


/* DISPATCHER */
void dispatcher(){
    int aleatorio = (Random2() % 256);
    TCB_t *proximaThread = NULL;
    proximaThread = acharProximaThread(aleatorio);
    if(proximaThread == NULL)
        return;
    
	//printf("Thread escolhida: %d, ticket: %d e o numero avaliado: %d \n", proximaThread->tid, proximaThread->ticket, aleatorio);
    
    exeThread = proximaThread;
    proximaThread->state = EXECUCAO;
    
    setcontext(&proximaThread->context);
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
    mainThread->ticket = (Random2() % 256);
    
    getcontext(&mainThread->context);
    exeThread = mainThread;
    
    main_criada = 1;
    
    criarContextoDispatcher();
    return 0;
    
}

void terminarThread(){
    
    quemEspera* quemEspera = alguemEsperando(exeThread);
    
    if (quemEspera != NULL){
        removerBloqueada(quemEspera->esperando);
        DeleteAtIteratorFila2(&fila_esperando);
        quemEspera->esperando->state = APTO;
        if (quemEspera->esperando->tid == 0)
            AppendFila2(&fila_aptos, mainThread);
        else
            AppendFila2(&fila_aptos, quemEspera->esperando);
        
    }
    
    //free(exeThread->context.uc_stack.ss_sp); // remove as threads
    //free(exeThread); //Os frees causavam segmentation fault
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
    
    TCB_t *newThread = malloc(sizeof(TCB_t));
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
    
    //printf("Thread %d criada! Seu ticket eh: %d\n", newThread->tid, newThread->ticket);
    
    return newThread->tid;
}

/*
	CYIELD
	Liberação voluntária da CPU
*/
int cyield(void)
{
    if (main_criada == 0){
        criarMainThread();
    }
    
    
    if (filas_inicializadas == 0){
        inicializaFilas();
    }
    
    //printf("Eu sou a thread %d e estou realizandou um yield \n", exeThread->tid);
    
    TCB_t* rodando;
    rodando = exeThread;
    rodando->state = APTO;
    
    AppendFila2(&fila_aptos,rodando);
    
    swapcontext(&exeThread->context, contextoDispatcher);
    
    return 0;
}

/*
	CJOIN
	Cthread chamante aguarda o final de outra thread
*/
int cjoin(int tid)
{
    if (main_criada == 0){
        criarMainThread();
    }
    
    
    if (filas_inicializadas == 0){
        inicializaFilas();
    }
    
    if (jaEsperada(tid) == 1){
        //printf("Thread %d ja sendo esperada\n", tid);
        return -1;
    }
    
    
    TCB_t* procurado = acharTCB(tid);
    if (procurado == NULL)
        return -1;
    
    //printf("Sou a thread %d esperando a thread %d\n", exeThread->tid, tid);
    
    // A thread vai para a fila de bloqueadas
    
    TCB_t* vaiEsperar;
    vaiEsperar = exeThread;
    vaiEsperar->state = BLOQUEADO;
    AppendFila2(&fila_bloqueados,vaiEsperar);
    
    // A estrutura vai para a fila de esperando
    
    quemEspera* aux = (quemEspera*) malloc(sizeof(quemEspera));
    if (aux == NULL)
        return -1;
    
    aux->esperando = vaiEsperar;
    aux->sendoEsperado = procurado;
    aux->tidEsperada = tid;
    AppendFila2(&fila_esperando,aux);
    
    swapcontext(&exeThread->context, contextoDispatcher);
    
    return 0;
}

/*
	CSEM_INIT
	Inicializa o semáforo
*/
int csem_init(csem_t *sem, int count)
{
	
	//printf("Criando com CSEM_INIT() com count = %d \n", sem->count);
	if(main_criada == 0) //if criado para verificar se a main foi criada
        if(criarMainThread() == -1) return -1; //se não foi, retorna -1

	if (filas_inicializadas == 0){
		inicializaFilas(); //inicializa filas se não estão inicializadas
	}

	sem->count = count; //grava a quantidade de recursos que o semáforo vai ter
	sem->fila = NULL; //cria uma fila para este semáforo onde vão guardar as threads que estão na fila do mesmo
    //verificações se deu algum erro na criação da fila
    //if (sem->fila == NULL) return -1;  
    //if (CreateFila2(sem->fila) != 0) return -1;
    //printf("CSEM_INIT() - Criou CSEM_INIT corretamente \n");

    return 0;
}

/*
	CWAIT
	Requisita um recurso, se não tiver disponível, fica bloqueado
*/
int cwait(csem_t *sem)
{   
	//printf("Executando CWAIT() \n");
	sem->count--; //decrementa contador
	//printf("CWAIT() - Count agora e %d \n", sem->count);
    if (sem->count < 0) //tenho que confirmar só essa verificação mas acho que ta certo
    {
	//printf("CWAIT() - Selecionando a thread que estava executando \n");
        TCB_t* thread; //seleciona a thread que estava executando
        thread = exeThread;
        thread->state = BLOQUEADO;
	if(sem->fila == NULL)
	{
		sem->fila = malloc(sizeof(FILA2));
		CreateFila2(sem->fila);	
	}

	//printf("CWAIT() - Colocando na fila de bloqueados %d \n", thread->tid);	
        AppendFila2(&fila_bloqueados, (void *)thread); //Coloca a thread que estava executando na fila de bloqueados geral
       	//printf("CWAIT() - Removendo da fila de espera dentro do semaforo \n");
        AppendFila2(sem->fila,  (void *)thread); //Coloca a thread que estava executando na fila do semáforo

		swapcontext(&thread->context, contextoDispatcher);

        
    }
    
    return 0;
}

/*
	CSGINAL
	Sinaliza a liberação de um recurso
*/
int csignal(csem_t *sem)
{    
	//printf("Executando CSIGNAL() \n");
	sem->count++; //libera o recurso
	//printf("CSIGNAL() - Count agora e %d \n", sem->count);
	if (sem->count <= 0)
	{
	   	if(FirstFila2(sem->fila) == 0) //pega o primeiro da fila 'FIFO'
		{
			TCB_t* thread = (TCB_t*)GetAtIteratorFila2(sem->fila); //pega a thread da fila do semáforo
			//printf("CSIGNAL() - Removendo da fila de bloqueados a thread %d \n", thread->tid);

			//Remove da fila de bloqueados
			removerBloqueada(thread);
			thread->state = APTO;

			//Coloca de volta na fila de aptos
			//printf("CSIGNAL() - Colocando na fila de aptos \n");
			AppendFila2(&fila_aptos, (void *)thread);

			//Retira da fila do semáforo
			//printf("CSIGNAL() - Retirando do semaforo \n");
			DeleteAtIteratorFila2(sem->fila);
		}
		
		if (FirstFila2(sem->fila) != 0)
		{
		    free(sem->fila);
		    sem->fila = 0;
		}
	}
    
	return 0;
}

/*
    CIDENTIFY
    Identifica os membros do grupo
*/
int cidentify (char *name, int size){
    char participantes[] = "Nome: Fernando Luis Spaniol Cartao: 228343 \n Nome: Mateus Claudino Bica Cartao: 164383  \n Nome: Marcelo Haider Torres Cartao: 228416 \n";
    int tamanho = sizeof(participantes);
    int x = 0;
    
    if(size <= 0){
        //printf("Erro! Size eh negativo. \n");
        return -1;
    }
    
    while (x < size && x<tamanho){
        *name = participantes[x];
        name++;
        x++;
    }
    
    return 0;
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
/*
 tidEsperado
 Verifica se o tid passado como parametro já é esperado
 
*/

int tidEsperado(int tid){
    FirstFila2(&fila_esperando);
    quemEspera* aux;
    aux = GetAtIteratorFila2(&fila_esperando);
    while(aux != NULL){
        if (aux->tidEsperada == tid)
            return 1;
        NextFila2(&fila_esperando);
        aux = GetAtIteratorFila2(&fila_esperando);
    }
    return 0;
}

/*
 jaEsperada
 Retorna 1 se a thread já é esperada por outra thread, 0 se não
*/

int jaEsperada(int tid){
    FirstFila2(&fila_esperando);
    quemEspera* aux;
    aux = GetAtIteratorFila2(&fila_esperando);
    while(aux != NULL){
        if (aux->tidEsperada == tid)
            return 1;
        NextFila2(&fila_esperando);
        aux = GetAtIteratorFila2(&fila_esperando);
    }
    
    return 0;
}

/*
 acharTCB
 Acha qual o TCB a ser esperado
*/

TCB_t* acharTCB(int tid){
    FirstFila2(&fila_aptos);
    TCB_t* aux;
    aux = GetAtIteratorFila2(&fila_aptos);
    while(aux != NULL){
        if(aux->tid == tid)
            return aux;
        NextFila2(&fila_aptos);
        aux = GetAtIteratorFila2(&fila_aptos);
    }
    
    FirstFila2(&fila_bloqueados);
    TCB_t* aux2;
    aux2 = GetAtIteratorFila2(&fila_bloqueados);
    while(aux2 != NULL){
        if(aux2->tid == tid)
            return aux2;
        NextFila2(&fila_bloqueados);
        aux2 = GetAtIteratorFila2(&fila_bloqueados);
    }
    
    // Se chegar aqui, não existe
    return NULL;
}

/*
 removerBloqueada
 remove a estrutura quemEspera da fila de bloqueadas
 */

int removerBloqueada(TCB_t* thread){
    FirstFila2(&fila_bloqueados);
    TCB_t* aux;
    aux = GetAtIteratorFila2(&fila_bloqueados);
    while(aux != NULL){
        if(aux->tid == thread->tid){
            DeleteAtIteratorFila2(&fila_bloqueados); 
            return 0;
        }
        NextFila2(&fila_bloqueados);
        aux = GetAtIteratorFila2(&fila_bloqueados);
    }
    return -1;
}

/*
 acharProximaThread
 Recebe um valor e acha a thread com o valor de ticket mais proximo
*/

TCB_t* acharProximaThread(int x){
    int menor_diferenca = 256;
    TCB_t* aux = NULL;
    TCB_t* proximaThread = NULL;
    FirstFila2(&fila_aptos);
    aux = GetAtIteratorFila2(&fila_aptos);
    while (aux != NULL){
        if (diferenca(aux->ticket, x) < menor_diferenca){
            menor_diferenca = diferenca(aux->ticket, x);
            proximaThread = aux;
        }
        NextFila2(&fila_aptos);
        aux = GetAtIteratorFila2(&fila_aptos); // DeleteAtIteratorFila2???
    }
    
    removeFilaAptos(proximaThread->tid);
    
    return proximaThread;
}

/*
 diferenca
 Acha a diferenca entre dois inteiros
*/

int diferenca(int x, int y){
    int z;
    z = (x - y);
    if(z < 0)
        z = z * -1;
    return z;
}

/*
 removeFilaAptos
 Retira a fila que vai ser executada pelo dispatcher
*/

void removeFilaAptos(int tid){
    TCB_t* aux = NULL;
    
    FirstFila2(&fila_aptos);
    aux = GetAtIteratorFila2(&fila_aptos);
    
    while (aux != NULL){
        if (aux->tid == tid)
            DeleteAtIteratorFila2(&fila_aptos);
        NextFila2(&fila_aptos);
        aux = GetAtIteratorFila2(&fila_aptos);
    }
    
}

/*
 criarContextoDispatcher()
 cria o contexto do dispatcher para ser usado nas funções
*/

int criarContextoDispatcher()
{
    contextoDispatcher = (ucontext_t*) malloc(sizeof(ucontext_t));
    if (contextoDispatcher == NULL) return -1; //erro no malloc
    
    contextoDispatcher->uc_link = NULL;
    contextoDispatcher->uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
    contextoDispatcher->uc_stack.ss_size = SIGSTKSZ;
    
    getcontext(contextoDispatcher);
    makecontext(contextoDispatcher, (void (*)(void)) dispatcher, 0, NULL);
    return 0;
    
}




























