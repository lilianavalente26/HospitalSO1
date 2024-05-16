/* Grupo SO-024
* Afonso Santos - fc59808
* Madalena Machado - fc59858
* Liliana Valente - fc59846
*/

#include "main.h"
#include "configuration.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void config_args(FILE* config_text, struct data_container* data) {
    int maxChar = 256;
    char max_ads, buffers_size, n_patients, n_receptionists, n_doctors, alarm_time;

    // Guardar cada linha numa variAvel
    fgets(&max_ads,maxChar,config_text);
    fgets(&buffers_size,maxChar,config_text);
    fgets(&n_patients,maxChar,config_text);
    fgets(&n_receptionists,maxChar,config_text);
    fgets(&n_doctors,maxChar,config_text);
    fgets(data->log_filename,maxChar,config_text);
    fgets(data->statistics_filename,maxChar,config_text);
    fgets(&alarm_time,maxChar,config_text);

    // Transformar a variAvel para o tipo certo
    data->max_ads = atoi(&max_ads);
    data->buffers_size = atoi(&buffers_size);
    data->n_patients = atoi(&n_patients);
    data->n_receptionists = atoi(&n_receptionists);
    data->n_doctors = atoi(&n_doctors);
    data->alarm_time = atoi(&alarm_time);
}