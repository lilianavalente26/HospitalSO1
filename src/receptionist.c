/*
* Grupo SO-024
* Afonso Santos - fc59808
* Madalena Machado - fc59858
* Liliana Valente - fc59846
*/

#include "memory.h"
#include "main.h"
#include "receptionist.h"
#include <stdlib.h>
#include <time.h>

int count_receptionist_stats(struct data_container* data) {
    int count = 0;

    for (int i = 0; i < data->n_receptionists; i++) {
        count += data->receptionist_stats[i];
    }

    return count;
}

int execute_receptionist(int receptionist_id, struct data_container* data, struct communication* comm){
    struct admission *newAd = allocate_dynamic_memory(sizeof(struct admission));

    while (data->terminate == 0){
        if(comm->patient_receptionist->buffer->id!=-1){
            receptionist_receive_admission(newAd,data,comm);
            receptionist_process_admission(newAd, receptionist_id,data);
            receptionist_send_admission(newAd,data,comm);        
        }
    }
    return count_receptionist_stats(data); //numero de admissoes realizadas
}

void receptionist_receive_admission(struct admission* ad, struct data_container* data, struct communication* comm){
    //Caso data->terminate for 1 
    if (*data->terminate == 1) {
        return;
    } 
    //Escolhe um receptionist aleatOrio para receber a admission
    else {    
        read_patient_receptionist_buffer(comm->patient_receptionist,data->buffers_size,ad);
    }
}

void receptionist_process_admission(struct admission* ad, int receptionist_id, struct data_container* data){
    //Alterar os dados
    ad->receiving_receptionist = receptionist_id;
    ad->status = 'R';
    data->receptionist_stats[receptionist_id]++;

    //Atualizar a admission no data
    data->results[ad->id] = *ad;
}

void receptionist_send_admission(struct admission* ad, struct data_container* data, struct communication* comm){
    write_receptionist_doctor_buffer(comm->receptionist_doctor, data->buffers_size, ad);
}