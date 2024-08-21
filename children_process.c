#include "children_process.h"

void display_children_data(info_filho* dados_filho, int *memoria_compartilhada, int iter) {
    
    printf("\n==============================================\n");
    printf("Numero de serie filho: %d\n", dados_filho -> numero_serie);
    printf("Pid do filho: %d\n", dados_filho -> pid_real);
    printf("Este filho nao possui threads!\n");
    printf("Variavel 1 shmem: %d \t Variavel 2 shmem: %d\n", memoria_compartilhada[0], memoria_compartilhada[1]);
    printf("Iteracoes: %d/%d\n", iter, QUANTIDADE_ITERACOES);
    printf("==============================================\n");
}

void display_thread_data(info_filho* dados_filho, int *memoria_compartilhada, unsigned short numero_serie_thread, int iter, int quantidade_iteracoes) {
   
    lista_encadeada_threads *aux = dados_filho -> lista_threads;

    printf("\n==============================================\n");

    for (; aux != NULL; aux = aux -> prox_lista_threads) {
        if (aux -> numero_serie == numero_serie_thread) {
            printf("Numero de serie desta Thread: %d\n", aux -> numero_serie);
            printf("Pai desta Thread %d \n", dados_filho -> numero_serie);
            printf("TID desta Thread %ld\n", (long)aux -> thread_id);
        }
    }
    
    printf("Variavel 1 shmem: %d \t Variavel 2 shmem: %d\n", memoria_compartilhada[0], memoria_compartilhada[1]);
    printf("Iteracoes: %d/%d\n", iter, quantidade_iteracoes);
    printf("==============================================\n");
}

pid_t direct_access_children(info_filho *dados_filho, unsigned short *numero_serie) {
    pid_t pid_virtual = fork();

    if(pid_virtual == PID_VIRTUAL_FILHO){ //Verifica se é o filho e guarda suas informações

        dados_filho -> numero_serie = *numero_serie;

        dados_filho -> pid_real = getpid();

        dados_filho -> lista_threads = NULL; //Como a lista encadeada de threads está vazia, então ele não usa threads no acesso a área de memória compartilhada
    }
    return pid_virtual; //retorna o pid_virtual
}

void direct_access_children_execution(info_filho *dados_filho, int *memoria_compartilhada, sem_t *semaforo, dados_mensagem mensagem_pai, int id_mensagem_pai) {
    int variavel_local, iter;

    //Fazer os passos da execução principal
    for (iter = 1; iter <= QUANTIDADE_ITERACOES; iter++) {
        sem_wait(semaforo); //Garante a exclusão mútua 

        //Região crítica =====================================================

        variavel_local = memoria_compartilhada[0]; //Passo 1

        --variavel_local; //Passo 2

        random_sleep(); //Passo 3

        memoria_compartilhada[0] = variavel_local; //Passo 4

        memoria_compartilhada[1] = memoria_compartilhada[1] + 1; //Passo 5

        display_children_data(dados_filho, memoria_compartilhada, iter); //Passo 6

        //Fim região crítica =================================================

        sem_post(semaforo); //Garante a exclusão mútua 

        random_sleep(); //Passo 7
    }

    //Prepara a mensagem (define o tipo e seu conteúdo)
    snprintf(mensagem_pai.mtext, MSG_SIZE, " ");
    mensagem_pai.mtype = dados_filho -> numero_serie; 

    //Envia a mensagem para seu pai
    if (msgsnd(id_mensagem_pai, &mensagem_pai, MSG_SIZE, 0) == VERIFICACAO_ERRO) {
        perror("Erro ao enviar mensagem do processo pai");
        exit(EXIT_FAILURE);
    }

    //Indica ao pai que o filho terminou de executar!
    printf("\nO filho %d terminou de executar!\n", dados_filho -> numero_serie);
}

pid_t thread_access_children(info_filho *dados_filho, unsigned short *numero_serie, unsigned short *numero_serie_threads, int *memoria_compartilhada, 
                                sem_t *semaforo, dados_mensagem mensagem_pai, int id_mensagem_pai) {
    pid_t pid_virtual = fork();

    if(pid_virtual == PID_VIRTUAL_FILHO) { //Verifica se é o filho e guarda suas informações

        dados_filho -> numero_serie = *numero_serie;

        dados_filho -> pid_real = getpid();
        
        dados_filho -> lista_threads = NULL;

        thread_access_children_execution(dados_filho, numero_serie_threads, memoria_compartilhada, semaforo);

        //Prepara e envia a mensagem ao processo pai de que este filho já terminou de executar
        snprintf(mensagem_pai.mtext, MSG_SIZE, " ");
        mensagem_pai.mtype = dados_filho -> numero_serie; 

        if (msgsnd(id_mensagem_pai, &mensagem_pai, MSG_SIZE, 0) == VERIFICACAO_ERRO) {
            perror("Erro ao enviar mensagem do processo pai!\n");
            exit(EXIT_FAILURE);
        }
    }
    return pid_virtual; //retorna o pid_virtual
}

void thread_access_children_execution(info_filho *dados_filho, unsigned short *numero_serie_threads, int *memoria_compartilhada, sem_t *semaforo) {

    //Inicializa a fila de mensagens
    dados_mensagem mensagem;
    int id_mensagem = create_message_line(dados_filho -> numero_serie);

    //Guarda o número de série das threads em uma outra variável para fazer o recebimento das mensagens
    unsigned short num_serie_threads_ini = *numero_serie_threads;

    // Gera a aleatoriedade de iteracoes
    int vetor_numeros_iteracoes[QUANTIDADE_THREADS]; 
    int qtd_iteracoes = QUANTIDADE_ITERACOES;

    for (int i = 0; i < QUANTIDADE_THREADS; i++) {
        if (i == QUANTIDADE_THREADS - 1) {
            vetor_numeros_iteracoes[i] = qtd_iteracoes;
            break;
        }
        vetor_numeros_iteracoes[i] = rand() % (qtd_iteracoes + 1);
        qtd_iteracoes = qtd_iteracoes - vetor_numeros_iteracoes[i];
    }

    //Cria as threads
    for (int iter = 0; iter < QUANTIDADE_THREADS; iter++) {
        create_thread(dados_filho, numero_serie_threads, memoria_compartilhada, semaforo, vetor_numeros_iteracoes[iter], mensagem, id_mensagem);
        *numero_serie_threads = *numero_serie_threads + 1;
    }
    
    //Recebe as mensagens vindas das threads
    for (int iter = num_serie_threads_ini; iter < num_serie_threads_ini + QUANTIDADE_THREADS; iter++) {
        if (msgrcv(id_mensagem, &mensagem, sizeof(mensagem.mtext), iter, 0) == VERIFICACAO_ERRO) {
            perror("Erro ao receber mensagem das threads!\n");
            exit(EXIT_FAILURE);
        }
    }

    //Limpa a lista de threads
    free(dados_filho -> lista_threads);

    //Indica que o filho terminou de executar!
    printf("\nO filho %d terminou de executar!\n", dados_filho -> numero_serie);

    //Apaga a fila de mensagens
    remove_message_line(id_mensagem);
}

void create_thread(info_filho* dados_filho, unsigned short* numero_serie_threads, int* memoria_compartilhada, sem_t* semaforo, 
                    int iteracoes_por_thread, dados_mensagem mensagem, int id_mensagem) {
    
    //Empacota os dados numa estrutura que será passada como parâmetro na criação da thread
    dados_thread *dados = malloc(sizeof(dados_thread));
    dados -> dados_filho = dados_filho;
    dados -> memoria_compartilhada = memoria_compartilhada;
    dados -> semaforo = semaforo;
    dados -> numero_serie_threads = *numero_serie_threads;
    dados -> quantidade_iteracoes = iteracoes_por_thread;
    dados -> id_mensagem = id_mensagem;
    dados -> mensagem = mensagem;
    
    //Cria um novo nó
    lista_encadeada_threads *novo_no = malloc(sizeof(lista_encadeada_threads));
 
    //Preenchimento dos dados do novo nó
    novo_no -> numero_serie = *numero_serie_threads;
    novo_no -> prox_lista_threads = NULL;

    //Encontra a posição do novo nó
    if (dados_filho -> lista_threads == NULL)
        dados_filho -> lista_threads = novo_no;

    else {
        lista_encadeada_threads *ultimo_no = dados_filho -> lista_threads;
        while (ultimo_no -> prox_lista_threads != NULL) {
            ultimo_no = ultimo_no -> prox_lista_threads;
        }
        ultimo_no -> prox_lista_threads = novo_no;
    }

    //Criação da thread e atribuição do ID da thread à estrutura
    unsigned short status = pthread_create(&(novo_no -> thread_id), NULL, (void *)thread_execution, (void *)dados);

    //Verifica se a thread foi criada corretamente
    if (status != 0) {
        fprintf(stderr, "Erro ao criar a thread: %d\n", status);
        exit(EXIT_FAILURE);
    }
}

void thread_execution(dados_thread *dados) {
    
    //Desempacota os dados recebidos
    sem_t *semaforo =  dados -> semaforo;
    int *memoria_compartilhada = dados -> memoria_compartilhada;
    info_filho *dados_filho = dados -> dados_filho;
    unsigned short numero_serie_thread = dados -> numero_serie_threads;
    int quantidade_iteracoes = dados -> quantidade_iteracoes;
    dados_mensagem mensagem = dados -> mensagem;
    int id_mensagem = dados -> id_mensagem;
    int variavel_local;

    //Faz os passos da execução principal
    for (int iter = 1; iter <= quantidade_iteracoes; iter++) {
        sem_wait(semaforo); //Garante a exclusão mútua 

        //Região crítica =====================================================

        variavel_local = memoria_compartilhada[0]; //Passo 1

        --variavel_local; //Passo 2

        random_sleep(); //Passo 3

        memoria_compartilhada[0] = variavel_local; //Passo 4

        memoria_compartilhada[1] = memoria_compartilhada[1] + 1; //Passo 5

        display_thread_data(dados_filho, memoria_compartilhada, numero_serie_thread, iter, quantidade_iteracoes); //Passo 6

        //Fim região crítica =================================================

        sem_post(semaforo); //Garante a exclusão mútua 

        random_sleep(); //Passo 7
    }
    
    //limpa os dados alocados (a estrutura usada para empacotar)
    free(dados);

    //Prepara a mensagem (define o tipo e seu conteúdo)
    snprintf(mensagem.mtext, MSG_SIZE, " ");
    mensagem.mtype = numero_serie_thread; 

    //Envia a mensagem para o processo filho (que criou esta thread)
    if (msgsnd(id_mensagem, &mensagem, MSG_SIZE, 0) == VERIFICACAO_ERRO) {
        perror("Erro ao enviar mensagem do processo pai");
        exit(EXIT_FAILURE);
    }

}