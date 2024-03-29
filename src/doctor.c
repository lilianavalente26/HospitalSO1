#include <stdio.h>
#include <doctor.h>

int execute_doctor(int doctor_id, struct data_container* data, struct communication* comm){
    
}

void doctor_receive_admission(struct admission* ad, int doctor_id, struct data_container* data, struct communication* comm){

}

void doctor_process_admission(struct admission* ad, int doctor_id, struct data_container* data){
    //AlteraCAo do receiving_doctor
    ad->receiving_doctor = doctor_id;
    
    //VariAveis
    int *docStats = data->doctor_stats;
    char *adStatus = ad->status;

    //Verifica se existe espaCo para a admission
    //Caso tenha espaCo
    if (docStats < data->max_ads) {
        *docStats++;
        *adStatus = 'A';
    }
    //Caso nAo tenha espaCo
    else {
        *adStatus = 'N';
    }
    //Atualizar a admission no data
    data->results = ad;
}