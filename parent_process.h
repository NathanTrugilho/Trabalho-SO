#ifndef PARENT_H
#define PARENT_H

#include "imports.h"
#include "children_process.h"
#include "message.h"

#define QUANTIDADE_PROCESSOS_FILHOS_DIR 2 //Filhos que vão manipular diretamente
#define QUANTIDADE_PROCESSOS_FILHOS_THR 2 //Filhos que criarão threads
#define CHAVE_MENSAGEM_PROCESSO_PAI 1234
#define IDENTIFICADOR_PAI 1

//Cria os processos filhos
int create_children_processes(info_filho* dados_filho, unsigned short* numero_serie, unsigned short* numero_serie_threads, int* memoria_compartilhada, 
                                sem_t* semaforo, dados_mensagem mensagem_pai, int id_mensagem_pai);

void main_execution(info_filho* dados_filho, unsigned short* numero_serie, unsigned short* numero_serie_threads, int* memoria_compartilhada, sem_t* semaforo);

#endif