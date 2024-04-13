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

/* Função principal de um Rececionista. Deve executar um ciclo infinito onde em 
* cada iteração lê uma admissão dos pacientes e se a mesma tiver id 
* diferente de -1 e se data->terminate ainda for igual a 0, processa-a e
* a encaminha para os médicos. Admissões com id igual a -1 são 
* ignoradas (ad inválida) e se data->terminate for igual a 1 é porque foi 
* dada ordem de terminação do programa, portanto deve-se fazer return do
* número de admissões realizadas. Para efetuar estes passos, pode usar os
* outros métodos auxiliares definidos em receptionist.h.
*/
int execute_receptionist(int receptionist_id, struct data_container* data, struct communication* comm){
    while (data->terminate == 0){
        if(comm->patient_receptionist->buffer->id!=-1){
            receptionist_receive_admission(comm->patient_receptionist->buffer,data,comm);
        }
    }
    return *data->receptionist_stats; //numero de admissoes realizadas
}
/* Função que lê uma admissão do buffer de memória partilhada entre os pacientes e os rececionistas.
* Antes de tentar ler a admissão, deve verificar se data->terminate tem valor 1.
* Em caso afirmativo, retorna imediatamente da função.
*/
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

/* Função que realiza uma admissão, alterando o seu campo receiving_receptionist para o id
* passado como argumento, alterando o estado da mesma para 'R' (receptionist), e 
* incrementando o contador de admissões realizadas por este rececionista no data_container. 
* Atualiza também a admissão na estrutura data.
*/
void receptionist_process_admission(struct admission* ad, int receptionist_id, struct data_container* data){
    //Alterar os dados
    ad->receiving_receptionist = receptionist_id;
    ad->status = 'R';
    data->receptionist_stats++;

    //Atualizar a admission no data
    data->results = ad;
}

/* Função que escreve uma admissão no buffer de memória partilhada entre
* os rececionistas e os médicos.
*/
void receptionist_send_admission(struct admission* ad, struct data_container* data, struct communication* comm){
    write_receptionist_doctor_buffer(comm->receptionist_doctor, data->buffers_size, ad);
}
