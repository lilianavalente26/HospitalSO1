/*
* Grupo SO-024
* Afonso Santos - fc59808
* Madalena Machado - fc59858
* Liliana Valente - fc59846
*/

#include <stdio.h>
#include <doctor.h>


int count_doctor_stats(struct data_container* data) {
    int count = 0;

    for (int i = 0; i < data->n_doctors; i++) {
        count += data->doctor_stats[i];
    }

    return count;
}

int execute_doctor(int doctor_id, struct data_container* data, struct communication* comm){
    struct admission *newAd = allocate_dynamic_memory(sizeof(struct admission));

    while (data->terminate == 0){
        if(comm->receptionist_doctor->buffer->id!=-1){
            doctor_receive_admission(comm->receptionist_doctor->buffer, doctor_id, data,comm);
            doctor_process_admission(newAd,doctor_id,data);
        }
    }
    return count_doctor_stats(data); //numero de consultas realizadas
}

void doctor_receive_admission(struct admission* ad, int doctor_id, struct data_container* data, struct communication* comm){
    if (*data->terminate == 1) {
        return; // ja nao ha mais admissoe spara admitir
    }
    if(ad->receiving_doctor == doctor_id){
        read_receptionist_doctor_buffer(comm->receptionist_doctor, doctor_id, data->buffers_size, ad);
    }
}

void doctor_process_admission(struct admission* ad, int doctor_id, struct data_container* data){
    //AlteraCAo do receiving_doctor
    ad->receiving_doctor = doctor_id;
    
    //VariAveis
    int *docStats = data->doctor_stats;
    char *adStatus = &ad->status;

    //Verifica se existe espaCo para a admission
    //Caso tenha espaCo
    if (*docStats < data->max_ads) {
        docStats[doctor_id]++;
        *adStatus = 'A';
    }
    //Caso nAo tenha espaCo
    else {
        *adStatus = 'N';
    }
    //Atualizar a admission no data
    data->results[ad->id] = *ad;
}