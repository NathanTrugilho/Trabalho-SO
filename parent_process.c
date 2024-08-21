#include "parent_process.h"

int create_children_processes(info_filho* dados_filho, unsigned short* numero_serie, unsigned short* numero_serie_threads, int* memoria_compartilhada, 
                                sem_t* semaforo, dados_mensagem mensagem_pai, int id_mensagem_pai) {
    pid_t pid_virtual;

    //Cria os processos filhos de acesso direto à memória
    for (unsigned short iter = 0; iter < QUANTIDADE_PROCESSOS_FILHOS_DIR; iter++) {

        // Se o retorno for 0, então se trata de um processo filho e este deve sair da função
        if (direct_access_children(dados_filho, numero_serie) == PID_VIRTUAL_FILHO) {

            //Inicia a execução dos processos filhos de acesso direto
            direct_access_children_execution(dados_filho, memoria_compartilhada, semaforo, mensagem_pai, id_mensagem_pai);
            return PID_VIRTUAL_FILHO;
        }
        //Atualiza o número de série
        *numero_serie = *numero_serie + 1;
    }
    
    //Cria os processos filhos que criarão suas threads
    for (unsigned short iter = 0; iter < QUANTIDADE_PROCESSOS_FILHOS_THR; iter++) {
        if (thread_access_children(dados_filho, numero_serie, numero_serie_threads, memoria_compartilhada, semaforo, mensagem_pai, id_mensagem_pai) == PID_VIRTUAL_FILHO) {
            return PID_VIRTUAL_FILHO;
        }
        //Atualiza o número de série do processo e das threads
        *numero_serie = *numero_serie + 1;
        *numero_serie_threads = *numero_serie_threads + QUANTIDADE_THREADS;
    }
    //Serve para o programa principal identificar quem é o pai
    return IDENTIFICADOR_PAI;
}

void main_execution(info_filho* dados_filho, unsigned short* numero_serie, unsigned short* numero_serie_threads, int* memoria_compartilhada, sem_t* semaforo) {
    
    //Cria a fila de mensagens
    dados_mensagem mensagem;
    int id_mensagem = create_message_line(CHAVE_MENSAGEM_PROCESSO_PAI);

    //Retorna se for um processo filho
    if (create_children_processes(dados_filho, numero_serie, numero_serie_threads, memoria_compartilhada, semaforo, mensagem, id_mensagem) != IDENTIFICADOR_PAI) {
        return;
    }
    
    //Trecho responsável pelo recebimento das mensagens vindas dos processos filhos
    for (int iter = 1; iter <= (QUANTIDADE_PROCESSOS_FILHOS_DIR + QUANTIDADE_PROCESSOS_FILHOS_THR); iter++) {

        //Recebe as mensagens
        if (msgrcv(id_mensagem, &mensagem, sizeof(mensagem.mtext), iter, 0) == VERIFICACAO_ERRO) {
            perror("Erro ao receber mensagem dos processos filhos!");
            exit(EXIT_FAILURE);
        }
    }

    //Apaga a fila de mensagens
    remove_message_line(id_mensagem);

    //Avisa sobre o encerramento do programa e mostra o valor das variáveis compartilhadas 
    printf("\nO programa sera finalizado!\n");
    printf("Variavel 1 shmem: %d \t Variavel 2 shmem: %d\n\n", memoria_compartilhada[0], memoria_compartilhada[1]);

}