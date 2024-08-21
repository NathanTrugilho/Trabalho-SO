#include "sleep_funcion.h"

void random_sleep() {
    
    // Seed para a função rand() baseado no tempo atual
    srand(time(NULL));
    
    // Gera um número aleatório 200ms e 2seg
    int tempo_miliseg = (rand() % ((TEMPO_MAX_MS - TEMPO_MIN_MS) + CORRECAO_INCLUIR_TEMPO_MAX)) + TEMPO_MIN_MS;
    
    // Dorme pelo tempo aleatório gerado
    usleep(tempo_miliseg * CORRECAO_MICROSEGUNDOS);
}