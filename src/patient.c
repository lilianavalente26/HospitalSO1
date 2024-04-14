/*
* Grupo SO-024
* Afonso Santos - fc59808
* Madalena Machado - fc59858
* Liliana Valente - fc59846
*/

#include "memory.h"
#include "main.h"
#include "patient.h"


int execute_patient(int patient_id, struct data_container* data, struct communication* comm){
    while (*data->terminate == 0){
        if(comm->main_patient->buffer->id!=-1){
            patient_receive_admission(comm->main_patient->buffer, patient_id, data, comm);
        }
    }
    return *data->patient_stats; //numero de admissoes pedidas
}

void patient_receive_admission(struct admission* ad, int patient_id, struct data_container* data, struct communication* comm){
    if (*data->terminate == 1) {
        return; 
    }
    else if(comm->main_patient->buffer->requesting_patient == patient_id){
        patient_process_admission(ad, patient_id, data);
    }
}

void patient_process_admission(struct admission* ad, int patient_id, struct data_container* data){
    //Alterar os dados
    ad->receiving_patient = patient_id;
    ad->status = 'P';
    data->patient_stats++;

    //Atualizar a admission no data
    data->results = ad;
}

void patient_send_admission(struct admission* ad, struct data_container* data, struct communication* comm){
    write_patient_receptionist_buffer(comm->patient_receptionist, data->buffers_size, ad);
}
