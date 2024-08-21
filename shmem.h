#ifndef SHMEM_H
#define SHMEM_H

#include "imports.h"

#define TAMANHO_SHMEM 2*(sizeof(int))
#define DIRETORIO_ATUAL "."
#define CARACTERE_ARBITRARIO 'n' // n de nathan
#define VERIFICACAO_ERRO_SHMEM -1
#define PRIMEIRA_VARIAVEL 120
#define SEGUNDA_VARIAVEL 0
#define NENHUMA_FLAG_ESPECIAL 0

#define OPCOES_CRIACAO_SHMEM (IPC_CREAT | 0666) //indica que a área de memória compartilhada deve ser criada se não existir 
//e que as permissões de leitura e escrita devem ser concedidas ao processo pai. ^^^ 
// 0666 = representação octal das permissões

#define OPCOES_EXCLUSAO_SHMEM IPC_RMID

key_t generate_unique_key(); 

void create_shared_memory(key_t* chave, int* id_memoria_compartilhada, int** memoria_compartilhada);

void store_init_variables(int* memoria_compartilhada);

void free_shared_memory(int* id_memoria_compartilhada);

#endif