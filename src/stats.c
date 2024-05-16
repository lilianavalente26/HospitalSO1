/* Grupo SO-024
* Afonso Santos - fc59808
* Madalena Machado - fc59858
* Liliana Valente - fc59846
*/

#include "main.h"
#include "stats.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <linux/time.h>

char format_time(struct timespec time){
    // Obtém o timestamp atual
    clock_gettime(CLOCK_REALTIME, &time);
    struct tm *local_time = localtime(&time.tv_sec);

    // Formata o timestamp
    char timestamp[30];
    snprintf(timestamp, sizeof(timestamp), "%02d/%02d/%d_%02d:%02d:%02d.%03ld",
             local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900,
             local_time->tm_hour, local_time->tm_min, local_time->tm_sec, time.tv_nsec / 1000000);

    return timestamp;
}

void print_stats(struct data_container* data, struct communication* comm, struct semaphores* sems){
    printf("Process Statistics:\n");
    for (int i = 0; i < data->n_patients; i++)
    {
        printf("    Patient %d requested %d admissions!\n",i,data->patient_stats[i]);
    }
    for (int i = 0; i < data->n_receptionists; i++)
    {
        printf("    Receptionist %d forwarded %d admissions!\n",i,data->receptionist_stats[i]);
    }
    for (int i = 0; i < data->n_doctors; i++)
    {
        printf("    Doctor %d processed %d admissions!\n",i,data->doctor_stats[i]);
    }

    printf("Admission Statistics:\n");
    double totalSecs, totalNSecs;
    for (int i = 0; i < data->max_ads; i++)
    {
        printf("Admission: %d\n",data->results[i].id);
        printf("Status: %c\n",data->results[i].status);
        printf("Patient id: %d\n",data->results[i].receiving_patient);
        printf("Receptionist id: %d\n",data->results[i].receiving_receptionist);
        printf("Doctor id: %d\n",data->results[i].receiving_doctor);
        printf("Start time: %c\n",format_time(data->results[i].create_time));
        printf("Patient time: %c\n",format_time(data->results[i].patient_time));
        printf("Receptionist time: %c\n",format_time(data->results[i].receptionist_time));
        printf("Doctor time: %c\n",format_time(data->results[i].doctor_time));
        totalSecs = data->results[i].doctor_time.tv_sec - data->results[i].create_time.tv_sec;
        totalNSecs = data->results[i].doctor_time.tv_nsec - data->results[i].create_time.tv_nsec;
        printf("Total time: %d.%03ld",totalSecs,totalNSecs);
    }
    
}











