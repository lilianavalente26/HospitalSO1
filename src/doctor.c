#include <stdio.h>
#include <doctor.h>

int execute_doctor(int doctor_id, struct data_container* data, struct communication* comm){
    int consultas;
    while (comm->receptionist_doctor > 0){
        if(data->terminate == 0 && comm->receptionist_doctor->buffer->id!=-1){
            doctor_process_admission(comm->receptionist_doctor, doctor_id, data);
        }
        else if (data->terminate == 1){
            return sizeof(comm->receptionist_doctor);
        }
    }
}

void doctor_receive_admission(struct admission* ad, int doctor_id, struct data_container* data, struct communication* comm){

}

void doctor_process_admission(struct admission* ad, int doctor_id, struct data_container* data){
    
}