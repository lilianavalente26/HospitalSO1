/* Grupo SO-024
* Afonso Santos - fc59808
* Madalena Machado - fc59858
* Liliana Valente - fc59846
*/

#include "main.h"
#include "memory.h"
#include "synchronization.h"
#include "hospsignal.h"
#include <stdio.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

void print_admission_status(struct data_container *data, struct communication *comm, struct semaphores *sems){
    for (int i = 0; i < data->max_ads; i++){
        if(strcmp(data->results[i].status,"M") == 0){
            printf("ad:%d ",data->results[i].id);
            printf("status:%c ",data->results[i].status);
            printf("start_time: %d ",data->results[i].id,data->results[i].create_time);
        }
        else if(strcmp(data->results[i].status,"P") == 0){
            printf("ad:%d ",data->results[i].id);
            printf("status:%c ",data->results[i].status);
            printf("start_time: %d ",data->results[i].id,data->results[i].create_time);
            printf("patient:%d ", data->results[i].receiving_patient);
            printf("patient_time: %d ", data->results[i].patient_time);
        }
        else if(strcmp(data->results[i].status,"A") == 0 || strcmp(data->results[i].status,"N") == 0){
            printf("ad:%d ",data->results[i].id);
            printf("status:%c ",data->results[i].status);
            printf("start_time: %d ",data->results[i].id,data->results[i].create_time);
            printf("patient:%d ", data->results[i].receiving_patient);
            printf("patient_time: %d ", data->results[i].patient_time);
            printf("receptionist:%d ", data->results[i].receiving_receptionist);
            printf("receptionist_time: %d ", data->results[i].receptionist_time);
            printf("doctor:%d ", data->results[i].receiving_doctor);
            printf("doctor_time: %d\n", data->results[i].doctor_time);
        }
        else { //estado R 
            printf("ad:%d ",data->results[i].id);
            printf("status:%c ",data->results[i].status);
            printf("start_time: %d ",data->results[i].id,data->results[i].create_time);
            printf("patient:%d ", data->results[i].receiving_patient);
            printf("patient_time: %d ", data->results[i].patient_time);
            printf("receptionist:%d ", data->results[i].receiving_receptionist);
            printf("receptionist_time: %d ", data->results[i].receptionist_time);
        }
    }
    
}

void interrupt_program(struct data_container* data, struct communication* comm, struct semaphores* sems){
    signal(SIGINT,SIG_DFL);
    end_execution(data,comm,sems);
    exit(0);
}

/*
void setup_signal_handler(struct data_container* data, struct communication* comm, struct semaphores* sems) {
    // Configurar o tratador de sinal
    struct sigaction sa;
    sa.sa_handler = interrupt_program(data,comm,sems);
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Erro ao configurar o tratador de sinal");
        exit(1);
    }
}
*/