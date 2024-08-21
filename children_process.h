#ifndef CHILDREN_PROCESS_H
#define CHILDREN_PROCESS_H

#include "imports.h"
#include "sleep_funcion.h"
#include "message.h"

#define PID_VIRTUAL_FILHO 0
#define QUANTIDADE_ITERACOES 30
#define QUANTIDADE_THREADS 2

typedef struct _lista_encadeada_threads {

    unsigned short numero_serie;
    pthread_t thread_id;
    struct _lista_encadeada_threads* prox_lista_threads;

} lista_encadeada_threads;

typedef struct _info_filho {

    unsigned short numero_serie;
    pid_t pid_real;
    lista_encadeada_threads* lista_threads; //lista encadeada de threads

} info_filho;

typedef struct _dados_thread {

    unsigned short numero_serie_threads;
    int quantidade_iteracoes;
    int* memoria_compartilhada;
    sem_t* semaforo;
    info_filho* dados_filho;
    dados_mensagem mensagem;
    int id_mensagem;

} dados_thread;

//Mostra os dados pedidos no terminal
void display_children_data(info_filho *dados_filho, int *memoria_compartilhada, int iter);
void display_thread_data(info_filho *dados_filho, int *memoria_compartilhada, unsigned short numero_serie, int iter, int quantidade_iteracoes);

//Cria um filho de acesso direto
pid_t direct_access_children(info_filho *dados_filho, unsigned short *numero_serie);

//Faz a execução do filho de acesso direto
void direct_access_children_execution(info_filho *dados_filho, int *memoria_compartilhada, sem_t *semaforo, dados_mensagem mensagem_pai, int id_mensagem_pai);

//Cria um filho de acesso por thread
pid_t thread_access_children(info_filho *dados_filho, unsigned short *numero_serie, unsigned short* numero_serie_threads, int *memoria_compartilhada, 
                                sem_t *semaforo, dados_mensagem mensagem_pai, int id_mensagem_pai);

//Faz a execução do filho de acesso via thread
void thread_access_children_execution(info_filho *dados_filho, unsigned short *numero_serie_threads, int *memoria_compartilhada, sem_t *semaforo);

//Cria as threads
void create_thread(info_filho *dados_filho, unsigned short *numero_serie, int *memoria_compartilhada, sem_t *semaforo, 
                    int iteracoes_por_thread, dados_mensagem mensagem, int id_mensagem);

//Sequência de instruções que serão executadas pelas threads
void thread_execution(dados_thread *dados);


#endif