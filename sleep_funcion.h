#ifndef SLEEP_FUNCTION_H
#define SLEEP_FUNCTION_H

#include "imports.h"

#define TEMPO_MAX_MS 2000
#define TEMPO_MIN_MS 200
#define CORRECAO_MICROSEGUNDOS 1000 //Serve para converter o tempo de milisegundos para microsegundos

//serve para incluir os 2 segundos no intervalo possível de tempo para o programa dormir
#define CORRECAO_INCLUIR_TEMPO_MAX 1 

void random_sleep();

#endif