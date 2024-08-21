#include "imports.h"
#include "shmem.h"
#include "parent_process.h"
#include "semaphore.h"

#define INICIO_NUMERO_SERIE 1

int main() {

    //Número de série que será usado para identificar os processos filhos e threads
    unsigned short numero_serie = INICIO_NUMERO_SERIE;
    unsigned short numero_serie_threads = INICIO_NUMERO_SERIE;

    //Variáveis utilizadas para a criação da memória compartilhada
    key_t chave = generate_unique_key();
    int id_memoria_compartilhada;
    int *memoria_compartilhada;

    //Struct que vai conter todas as informações de um processo filho
    info_filho dados_filho;

    //Inicialização do ponteiro para criação do semáforo
    sem_t *semaforo;

    srand(time(NULL)); //Cria a semente que será ulitizada para gerar números aleatórios com base no tempo atual do sistema

    create_semaphore(&semaforo); //Cria o semáforo                                               

    create_shared_memory(&chave, &id_memoria_compartilhada, &memoria_compartilhada); //Cria a área de memória compartilhada

    main_execution(&dados_filho, &numero_serie, &numero_serie_threads, memoria_compartilhada, semaforo); //Execução principal

    free_shared_memory(&id_memoria_compartilhada); //Limpa a área de memória compartilhada

    sem_close(semaforo); //Fecha o semáforo

    sem_unlink("/semaforo"); //Remove o semáforo
    
    return EXIT_SUCCESS;
}