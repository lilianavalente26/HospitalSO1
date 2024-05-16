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

void print_admission_status(struct data_container *data, struct semaphores *sems){
    for (int i = 0; i < data->max_ads; i++){
        if(strcmp(data->results[i].status,"M") == 0){
            printf("ad:%d ",data->results[i].id);
            printf("status:%c ",data->results[i].status);
            printf("start_time: %d ",&data->results[i].create_time.tv_sec);
        }
        else if(strcmp(data->results[i].status,"P") == 0){
            printf("ad:%d ",data->results[i].id);
            printf("status:%c ",data->results[i].status);
            printf("start_time: %d ",&data->results[i].create_time.tv_sec);
            printf("patient:%d ", data->results[i].receiving_patient);
            printf("patient_time: %d ", &data->results[i].patient_time.tv_sec);
        }
        else if(strcmp(data->results[i].status,"A") == 0 || strcmp(data->results[i].status,"N") == 0){
            printf("ad:%d ",data->results[i].id);
            printf("status:%c ",data->results[i].status);
            printf("start_time: %d ",&data->results[i].create_time.tv_sec);
            printf("patient:%d ", data->results[i].receiving_patient);
            printf("patient_time: %d ", &data->results[i].patient_time.tv_sec);
            printf("receptionist:%d ", data->results[i].receiving_receptionist);
            printf("receptionist_time: %d ", &data->results[i].receptionist_time.tv_sec);
            printf("doctor:%d ", data->results[i].receiving_doctor);
            printf("doctor_time: %d\n", &data->results[i].doctor_time.tv_sec);
        }
        else { // Estado R 
            printf("ad:%d ",data->results[i].id);
            printf("status:%c ",data->results[i].status);
            printf("start_time: %d ",&data->results[i].create_time.tv_sec);
            printf("patient:%d ", data->results[i].receiving_patient);
            printf("patient_time: %d ", &data->results[i].patient_time.tv_sec);
            printf("receptionist:%d ", data->results[i].receiving_receptionist);
            printf("receptionist_time: %d ", &data->results[i].receptionist_time.tv_sec);
        }
    }
    signal(SIGALRM, print_admission_status);
}

void start_alarm(struct data_container *data, struct semaphores *sems){
    // Quando expira escreve o estado atual de todas as admissoes 
    signal(SIGALRM,print_admission_status); 
    // Alarme, que expira a cada alarm_time segundos
    alarm(data->alarm_time);
    
}

void interrupt_program(struct semaphores* sems){
    signal(SIGINT,end_execution);
    exit(0);
}

/*
void setup_signal_handler(struct semaphores* sems) {
    // Configurar o tratador de sinal
    struct sigaction sa;
    sa.sa_handler = interrupt_program(sems);
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Erro ao configurar o tratador de sinal");
        exit(1);
    }
}
*/