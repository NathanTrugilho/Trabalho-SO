#ifndef MESSAGE_H
#define MESSAGE_H

#include "imports.h"

#define MSG_SIZE 256
#define VERIFICACAO_ERRO -1

// Definição da estrutura da mensagem
typedef struct _dados_mensagem {
    long mtype; // Tipo da mensagem
    char mtext[MSG_SIZE]; // Conteúdo da mensagem
} dados_mensagem;

//Cria a fila de mensagens
int create_message_line(int num_serie);

//Remove a fila de mensagens
void remove_message_line(int id_mensagem);

#endif