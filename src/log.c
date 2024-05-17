/* Grupo SO-024
* Afonso Santos - fc59808
* Madalena Machado - fc59858
* Liliana Valente - fc59846
*/

#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void log_operation(struct data_container* data, const char input, const char arguments) {
    // Abre o arquivo de log especificado na estrutura data_container
    FILE *log_file = fopen(data->log_filename, "a");
    if (log_file == NULL) {
        printf("Erro ao abrir o arquivo de log.\n");
        return;
    }

    // Obtém o timestamp atual
    struct timespec current_time;
    clock_gettime(CLOCK_REALTIME, &current_time);
    struct tm *local_time = localtime(&current_time.tv_sec);

    // Formata o timestamp
    char timestamp[256];
    snprintf(timestamp, sizeof(timestamp), "%02d/%02d/%d_%02d:%02d:%02d.%03ld",
             local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900,
             local_time->tm_hour, local_time->tm_min, local_time->tm_sec, current_time.tv_nsec / 1000000);

    // Escreve no arquivo de log
    fprintf(log_file, "%s %s %s\n", timestamp, &input, &arguments);

    fclose(log_file);
}