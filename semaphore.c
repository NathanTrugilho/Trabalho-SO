#include "semaphore.h"

void create_semaphore(sem_t **semaforo) {

    //Abre o semáforo no endereço que foi criado na main e recebido por ela
    *semaforo = sem_open("/semaforo", O_CREAT | O_EXCL, 0644, VALOR_INICIAL_SEMAFORO); // 0644 são as permissões do semáforo
    
    if (*semaforo == SEM_FAILED) {
        perror("Erro ao criar o semáforo");
        sem_unlink("/semaforo"); //Tenta desanexar um possível semáforo já existente como uma medida de tratamento de erros
        exit(EXIT_FAILURE);
    }
}