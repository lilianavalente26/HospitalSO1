#include <stdio.h>
#include "doctor.h"

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
        return 0; // ja nao ha mais admissoe spara admitir
    }
    else if(comm->receptionist_doctor->buffer->requested_doctor == doctor_id){
        doctor_process_admition(ad, doctor_id, data);
    }
    else{
        return 0;
    }

}

void doctor_process_admission(struct admission* ad, int doctor_id, struct data_container* data){
    
}