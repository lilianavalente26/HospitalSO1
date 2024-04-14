/*
* Grupo SO-024
* Afonso Santos - fc59808
* Madalena Machado - fc59858
* Liliana Valente - fc59846
*/

#include <stdio.h>
#include <doctor.h>

int execute_doctor(int doctor_id, struct data_container* data, struct communication* comm){
    while (data->terminate == 0){
        if(comm->receptionist_doctor->buffer->id!=-1){
        doctor_receive_admission(comm->receptionist_doctor->buffer, doctor_id, data,comm);
        }
    }
    return *data->doctor_stats; //numero de consultas realizadas
}

void doctor_receive_admission(struct admission* ad, int doctor_id, struct data_container* data, struct communication* comm){
    if (*data->terminate == 1) {
        return; // ja nao ha mais admissoe spara admitir
    }
    if(comm->receptionist_doctor->buffer->requested_doctor == doctor_id){
        doctor_process_admission(ad, doctor_id, data);
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
        docStats++;
        *adStatus = 'A';
    }
    //Caso nAo tenha espaCo
    else {
        *adStatus = 'N';
    }
    //Atualizar a admission no data
    data->results = ad;
}