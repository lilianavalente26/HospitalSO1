#include <stdio.h>
#include <doctor.h>

int execute_doctor(int doctor_id, struct data_container* data, struct communication* comm){
    
}
/* Função que lê uma admissão (do buffer de memória partilhada entre
* os rececionistas e os médicos) que seja direcionada ao médico doctor_id.
* Antes de tentar ler a admissão, o processo deve
* verificar se data->terminate tem valor 1.
* Em caso afirmativo, retorna imediatamente da função.
*/
void doctor_receive_admission(struct admission* ad, int doctor_id, struct data_container* data, struct communication* comm){
    if (data->terminate == 1) {
        return; // ja nao ha mais admissoe spara admitir
    }
    if(ad->requested_doctor == doctor_id){
        comm->receptionist_doctor; // isto supostamente mostra o que a recepcionsta encaminhou para o doutor
    }
    

}

void doctor_process_admission(struct admission* ad, int doctor_id, struct data_container* data){
    
}