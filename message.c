#include "message.h"

int create_message_line(int num_serie) {

    //Estou usando o numero de série como uma chave porque ele é único e não corre o risco de gerar o mesmo identificador
    int chave = num_serie;
    //Gera um identificador (msgid) para a mensagem
    int msgid = msgget(chave, 0666 | IPC_CREAT);

    if (msgid == VERIFICACAO_ERRO) {
        perror("Erro ao criar fila de mensagens");
        exit(EXIT_FAILURE);
    }
    return msgid; //Retorna o identificador gerado que será utilizado no envio e recebimento de mensagens
}

void remove_message_line(int id_mensagem) {

    // Apaga a fila de mensagens
    if (msgctl(id_mensagem, IPC_RMID, NULL) == VERIFICACAO_ERRO) {
        perror("Erro ao remover fila de mensagens");
        exit(EXIT_FAILURE);
    }
}