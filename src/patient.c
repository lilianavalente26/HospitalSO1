/*
* Grupo SO-024
* Afonso Santos - fc59808
* Madalena Machado - fc59858
* Liliana Valente - fc59846
*/

#include "memory.h"
#include "main.h"
#include "patient.h"
#include <stdio.h>

int count_patient_stats(struct data_container* data) {
    int count = 0;

    for (int i = 0; i < data->n_patients; i++) {
        count += data->patient_stats[i];
    }

    return count;
}

int execute_patient(int patient_id, struct data_container* data, struct communication* comm, struct semaphores* sems){
    struct admission *newAd = allocate_dynamic_memory(sizeof(struct admission));
    
    while (*data->terminate == 0){
        patient_receive_admission(newAd, patient_id, data,comm,sems);
        if(newAd->id != -1){
            patient_process_admission(newAd,patient_id,data,sems);
            patient_send_admission(newAd,data,comm,sems);        
        }
    }
    return count_patient_stats(data); //numero de admissoes pedidas
}

void patient_receive_admission(struct admission* ad, int patient_id, struct data_container* data, struct communication* comm, struct semaphores* sems){
    if (*data->terminate == 1) {
        return; 
    }
    else{
        consume_begin(sems->main_patient);

        read_main_patient_buffer(comm->main_patient,patient_id,data->buffers_size,ad);

        consume_end(sems->main_patient);
    }
}

void patient_process_admission(struct admission* ad, int patient_id, struct data_container* data, struct semaphores* sems){
    //produce_begin(sems->main_patient);
    //Alterar os dados
    ad->receiving_patient = patient_id;
    ad->status = 'P';

    //produce_end(sems->main_patient);
    
    //semaphore_lock(sems->patient_stats_mutex);
    data->patient_stats[patient_id] = data->patient_stats[patient_id] + 1;
    //semaphore_unlock(sems->patient_stats_mutex); 

    //Atualizar a admission no data
    semaphore_lock(sems->results_mutex);
    data->results[ad->id] = *ad;
    semaphore_unlock(sems->results_mutex);
}

void patient_send_admission(struct admission* ad, struct data_container* data, struct communication* comm, struct semaphores* sems){
    produce_begin(sems->patient_receptionist);

    write_patient_receptionist_buffer(comm->patient_receptionist, data->buffers_size, ad);
    
    produce_end(sems->patient_receptionist);
}
