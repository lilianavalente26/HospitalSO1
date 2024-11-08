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

int execute_patient(int patient_id, struct data_container* data, struct communication* comm){
    struct admission newAd;
    
    while (*data->terminate == 0){
        patient_receive_admission(&newAd, patient_id, data,comm);
        if(newAd.id != -1){
            patient_process_admission(&newAd,patient_id,data);
            patient_send_admission(&newAd,data,comm);        
        }
    }
    return count_patient_stats(data); //numero de admissoes pedidas
}

void patient_receive_admission(struct admission* ad, int patient_id, struct data_container* data, struct communication* comm){
    if (*data->terminate == 1) {
        return; 
    }
    else
        read_main_patient_buffer(comm->main_patient,patient_id,data->buffers_size,ad);
    
}

void patient_process_admission(struct admission* ad, int patient_id, struct data_container* data){
    //Alterar os dados
    ad->receiving_patient = patient_id;
    ad->status = 'P';
    data->patient_stats[patient_id] = data->patient_stats[patient_id] + 1;

    //Atualizar a admission no data
    data->results[ad->id] = *ad;
}

void patient_send_admission(struct admission* ad, struct data_container* data, struct communication* comm){
    write_patient_receptionist_buffer(comm->patient_receptionist, data->buffers_size, ad);
}
