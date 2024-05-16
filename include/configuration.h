#ifndef CONFIGURATION_H_GUARD
#define CONFIGURATION_H_GUARD

#include "memory.h"
#include "synchronization.h"
#include <stdio.h>

/*
 * Função que lê o ficheiro config.txt e guarda o seu conteúdo nas respetivas variáveis
*/
void config_args(FILE* config_text, struct data_container* data);

#endif