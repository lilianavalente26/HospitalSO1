#include <stdio.h>
#include "memory.h"
#include <sys/mman.h> 
#include <sys/stat.h>  
#include <fcntl.h>

/* Função que reserva uma zona de memória partilhada com tamanho indicado
* por size e nome name, preenche essa zona de memória com o valor 0, e 
* retorna um apontador para a mesma. Pode concatenar o resultado da função
* getuid() a name, para tornar o nome único para o processo.
*/
void* create_shared_memory(char* name, int size){
    int shm = shm_open(name, O_CREAT | O_RDWR , 0666);
    int ftruncate(shm, size); 
    if(shm == -1){
        perror ("fodeu no bestie create_shared_memory");
        exit(1);
    }
    // com a notacao NULL e o SO que determina automaticamente o endereco
    // inicial da projecao
    void* ptrSharedMemory = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);
    if(ptrSharedMemory == MAP_FAILED){
        perror("fodeu a criar o pointer para a shared memory");
        exit(1);
    }

    memset(ptrSharedMemory, 0, size);


    return ptrSharedMemory;
}

/* Função que reserva uma zona de memória dinâmica com tamanho indicado
* por size, preenche essa zona de memória com o valor 0, e retorna um 
* apontador para a mesma.
*/
void* allocate_dynamic_memory(int size){

}

/* Função que liberta uma zona de memória partilhada previamente reservada.
*/
void destroy_shared_memory(char* name, void* ptr, int size){

}

/* Função que liberta uma zona de memória dinâmica previamente reservada.
*/
void deallocate_dynamic_memory(void* ptr){

}

/* Função que escreve uma admissão no buffer de memória partilhada entre a Main
* e os pacientes. A admissão deve ser escrita numa posição livre do buffer, 
* tendo em conta o tipo de buffer e as regras de escrita em buffers desse tipo.
* Se não houver nenhuma posição livre, não escreve nada.
*/
void write_main_patient_buffer(struct circular_buffer* buffer, int buffer_size, struct admission* ad){

}

/* Função que escreve uma admissão no buffer de memória partilhada entre os pacientes
* e os rececionistas. A admissão deve ser escrita numa posição livre do buffer, 
* tendo em conta o tipo de buffer e as regras de escrita em buffers desse tipo.
* Se não houver nenhuma posição livre, não escreve nada.
*/
void write_patient_receptionist_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct admission* ad){

}

/* Função que escreve uma admissão no buffer de memória partilhada entre os rececionistas
* e os médicos. A admissão deve ser escrita numa posição livre do buffer, 
* tendo em conta o tipo de buffer e as regras de escrita em buffers desse tipo.
* Se não houver nenhuma posição livre, não escreve nada.
*/
void write_receptionist_doctor_buffer(struct circular_buffer* buffer, int buffer_size, struct admission* ad){

}

/* Função que lê uma admissão do buffer de memória partilhada entre a Main
* e os pacientes, se houver alguma disponível para ler que seja direcionada ao paciente especificado.
* A leitura deve ser feita tendo em conta o tipo de buffer e as regras de leitura em buffers desse tipo.
* Se não houver nenhuma admissão disponível, afeta ad->id com o valor -1.
*/
void read_main_patient_buffer(struct circular_buffer* buffer, int patient_id, int buffer_size, struct admission* ad){

}

/* Função que lê uma admissão do buffer de memória partilhada entre os pacientes e rececionistas,
* se houver alguma disponível para ler (qualquer rececionista pode ler qualquer admissão).
* A leitura deve ser feita tendo em conta o tipo de buffer e as regras de leitura em buffers desse tipo.
* Se não houver nenhuma admissão disponível, afeta ad->id com o valor -1.
*/
void read_patient_receptionist_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct admission* ad){

}

/* Função que lê uma admissão do buffer de memória partilhada entre os rececionistas e os médicos,
* se houver alguma disponível para ler dirigida ao médico especificado. A leitura deve
* ser feita tendo em conta o tipo de buffer e as regras de leitura em buffers desse tipo. Se não houver
* nenhuma admissão disponível, afeta ad->id com o valor -1.
*/
void read_receptionist_doctor_buffer(struct circular_buffer* buffer, int doctor_id, int buffer_size, struct admission* ad){

}
