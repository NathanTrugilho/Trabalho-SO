#include "shmem.h"

key_t generate_unique_key() {
    
    //Cria a chave
    key_t chave = ftok(DIRETORIO_ATUAL, CARACTERE_ARBITRARIO);
   
    if (chave == VERIFICACAO_ERRO_SHMEM) {
        perror("Falha ao criar a chave unica!\n"); // Mostra qual foi o erro
        exit(EXIT_FAILURE);
    }
    return chave;
} 

//recebe o endereço de um ponteiro para o local onde a shmem ficará alocada
void create_shared_memory(key_t* chave, int* id_memoria_compartilhada, int** memoria_compartilhada) {

    //Cria o espaço de memória compartilhada
    *id_memoria_compartilhada = shmget(*chave, TAMANHO_SHMEM, OPCOES_CRIACAO_SHMEM);

    if (*id_memoria_compartilhada == VERIFICACAO_ERRO_SHMEM) {
        perror("Falha ao criar a area de memoria compartilhada!\n"); //mostra qual foi o erro
        exit(EXIT_FAILURE);
    }

    //'memoria_compartilhada' recebe o endereço da shmem
    *memoria_compartilhada = shmat(*id_memoria_compartilhada, NULL, NENHUMA_FLAG_ESPECIAL); //faço com que os filhos também possam acessar a shmem
    if (*memoria_compartilhada == (int *) VERIFICACAO_ERRO_SHMEM) {
        perror("Erro ao anexar a memória compartilhada!\n");
        exit(EXIT_FAILURE);
    }
    //Guarda os valores iniciais das variáveis na área de memória compartilhada
    store_init_variables(*memoria_compartilhada); 
}

void free_shared_memory(int* id_memoria_compartilhada) {

    // Desanexar a área de memória compartilhada
    if (shmdt(shmat(*id_memoria_compartilhada, NULL, NENHUMA_FLAG_ESPECIAL)) == VERIFICACAO_ERRO_SHMEM) {
        perror("Erro ao desanexar a area de memoria compartilhada!\n");
        exit(EXIT_FAILURE);
    }
    // Serve para garantir que nenhum processo esteja vinculado a area de memória antes desta ser excluída ^^^

    // Excluir a área de memória compartilhada
    if (shmctl(*id_memoria_compartilhada, IPC_RMID, NULL) == VERIFICACAO_ERRO_SHMEM) {
        perror("Erro ao excluir a area de memoria compartilhada!\n");
        exit(EXIT_FAILURE);
    }
}

//Guarda os valores iniciais das variáveis na área de memória compartilhada
void store_init_variables(int* memoria_compartilhada) {
    memoria_compartilhada[0] = PRIMEIRA_VARIAVEL;
    memoria_compartilhada[1] = SEGUNDA_VARIAVEL;
}