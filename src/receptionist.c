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

int execute_receptionist(int receptionist_id, struct data_container* data, struct communication* comm){
    while (data->terminate == 0){
        if(comm->patient_receptionist->buffer->id!=-1){
            receptionist_receive_admission(comm->patient_receptionist->buffer,data,comm);
        }
    }
    return *data->receptionist_stats; //numero de admissoes realizadas
}

void receptionist_receive_admission(struct admission* ad, struct data_container* data, struct communication* comm){
    //Caso data->terminate for 1 
    if (*data->terminate == 1) {
        return;
    } 
    //Escolhe um receptionist aleatOrio para receber a admission
    else {
        srand(time(NULL));
        int rand_index = rand() % data->n_receptionists;
        int receptionist_id = data->receptionist_pids[rand_index];
        
        receptionist_process_admission(ad, receptionist_id, data);
    }
}

void receptionist_process_admission(struct admission* ad, int receptionist_id, struct data_container* data){
    //Alterar os dados
    ad->receiving_receptionist = receptionist_id;
    ad->status = 'R';
    data->receptionist_stats++;

    //Atualizar a admission no data
    data->results = ad;
}

void receptionist_send_admission(struct admission* ad, struct data_container* data, struct communication* comm){
    write_receptionist_doctor_buffer(comm->receptionist_doctor, data->buffers_size, ad);
}
