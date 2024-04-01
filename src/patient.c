#include "memory.h"
#include "main.h"
#include "patient.h"


/* Função principal de um Paciente. Deve executar um ciclo infinito onde em 
* cada iteração lê uma admissão da main e se data->terminate ainda 
* for igual a 0, processa-a. Admissões com id igual a -1 são ignoradas
* (ad inválida) e se data->terminate for igual a 1 é porque foi dada ordem
* de terminação do programa, portanto deve-se fazer return do número de 
* admissões pedidas. Para efetuar estes passos, pode usar os outros
* métodos auxiliares definidos em patient.h.
*/
int execute_patient(int patient_id, struct data_container* data, struct communication* comm){
    while (data->terminate == 0){
        if(comm->main_patient->buffer->id!=-1){
            receptionist_receive_admission(comm->main_patient->buffer,data,comm);
        }
    }
    return data->patient_stats; //numero de admissoes pedidas
}
/* Função que lê uma admissão (do buffer de memória partilhada entre a main
* e os pacientes) que seja direcionada a patient_id. Antes de tentar ler a admissão, deve
* verificar se data->terminate tem valor 1. Em caso afirmativo, retorna imediatamente da função.
*/
void patient_receive_admission(struct admission* ad, int patient_id, struct data_container* data, struct communication* comm){
    // TODO
}

/* Função que valida uma admissão, alterando o seu campo receiving_patient para o patient_id
* passado como argumento, alterando o estado da mesma para 'P' (patient), e 
* incrementando o contador de admissões solicitadas por este paciente no data_container. 
* Atualiza também a admissão na estrutura data.
*/
void patient_process_admission(struct admission* ad, int patient_id, struct data_container* data){
    // TODO
}

/* Função que escreve uma admissão no buffer de memória partilhada entre os
* pacientes e os rececionistas.
*/
void patient_send_admission(struct admission* ad, struct data_container* data, struct communication* comm){
    write_main_patient_buffer(comm->main_patient, data->buffers_size, ad);
}