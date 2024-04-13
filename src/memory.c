#include <stdio.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

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
    //aloca memOria dinAmica com size
    int ptr* = malloc(size);
    //Houve erro a alocar
    if (ptr = NULL){
        perror("fodeu");
        exit(1);
    }
    //preenche essa zona de memória com o valor 0
    &ptr = 0;
    //retorna o apontador para a mesma
    return ptr;
}

/* Função que liberta uma zona de memória partilhada previamente reservada.
*/
void destroy_shared_memory(char* name, void* ptr, int size){
    int statusMap = munmap(ptr, size); 

    if(statusMap == -1){
        perror("fodeu ao destruir a mapa da memoria partilhada");
        exit(1);
    }

    int statusPointer = shm_unlink(name);

    if(statusPointer == -1){
        perror("fodeu ao destruir o pointer para a memoria partilhada");
        exit(1);
    }
}

/* Função que liberta uma zona de memória dinâmica previamente reservada.
*/
void deallocate_dynamic_memory(void* ptr){
    free(ptr);
}

/* Função que escreve uma admissão no buffer de memória partilhada entre a Main
* e os pacientes. A admissão deve ser escrita numa posição livre do buffer, 
* tendo em conta o tipo de buffer e as regras de escrita em buffers desse tipo.
* Se não houver nenhuma posição livre, não escreve nada.
*/
void write_main_patient_buffer(struct circular_buffer* buffer, int buffer_size, struct admission* ad){
    int in = buffer->ptrs->in;
    int out = buffer->ptrs->out;
    //Verifica se o buffer estA cheio
    if ((in+1) % buffer_size != out) {
        buffer->buffer[in] = ad;
        buffer->ptrs->in++;
    }

    return 0;
}

/* Função que escreve uma admissão no buffer de memória partilhada entre os pacientes
* e os rececionistas. A admissão deve ser escrita numa posição livre do buffer, 
* tendo em conta o tipo de buffer e as regras de escrita em buffers desse tipo.
* Se não houver nenhuma posição livre, não escreve nada.
*/
void write_patient_receptionist_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct admission* ad){
    int i = 0;
    int written = 0;
    int position = buffer->ptrs[i];
    /* Itera pelo buffer, procurando pelas posiCOes vazias
    *  Verifica se Já escreveu ou se ultrapassou o buffer_size
    */
    while (written != 1 || i+1 <= buffer_size) {
        //Verifica se a position estA livre
        if (position[i] == 0){
            buffer->ad[i] = ad;
            position[i] = 1;
            written = 1;
        }
        i++;
    }

    return 0;
}

/* Função que escreve uma admissão no buffer de memória partilhada entre os rececionistas
* e os médicos. A admissão deve ser escrita numa posição livre do buffer, 
* tendo em conta o tipo de buffer e as regras de escrita em buffers desse tipo.
* Se não houver nenhuma posição livre, não escreve nada.
*/
void write_receptionist_doctor_buffer(struct circular_buffer* buffer, int buffer_size, struct admission* ad){
    int in = buffer->ptrs->in;
    int out = buffer->ptrs->out;
    //Verifica se o buffer estA cheio
    if ((in+1) % buffer_size != out) {
        buffer->buffer[in] = ad;
        buffer->ptrs->in++;
    }

    return 0;
}

/* Função que lê uma admissão do buffer de memória partilhada entre a Main
* e os pacientes, se houver alguma disponível para ler que seja direcionada
* ao paciente especificado.
* A leitura deve ser feita tendo em conta o tipo de buffer e as regras de leitura em buffers desse tipo.
* Se não houver nenhuma admissão disponível, afeta ad->id com o valor -1.
*/
void read_main_patient_buffer(struct circular_buffer* buffer, int patient_id, int buffer_size, struct admission* ad){
    int in = buffer->ptrs->in;
    int out = buffer->ptrs->out;
    int read = 0;
    //Itera pelo ads contidos no buffer
    while ((out+1) % buffer_size != in){
        //Verifica se ad foi pedida pelo paciente
        if (buffer->buffer[out]->requesting_patient = patient_id) {
            ad = buffer->buffer[out];
            read = 1;
        }
        out++;
    }
    //Caso nAo tenha lido uma ad disponIvel
    if (read == 0) {
        ad->id = -1;
    }
        
    return 0;
}

/* Função que lê uma admissão do buffer de memória partilhada entre os pacientes e rececionistas,
* se houver alguma disponível para ler (qualquer rececionista pode ler qualquer admissão).
* A leitura deve ser feita tendo em conta o tipo de buffer e as regras de leitura em buffers desse tipo.
* Se não houver nenhuma admissão disponível, afeta ad->id com o valor -1.
*/
void read_patient_receptionist_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct admission* ad){
    int i = 0;
    int read = 0;
    int position = buffer->ptrs[i];
    /* Itera pelo buffer, procurando pelas posiCOes vazias
    *  Verifica se Já escreveu ou se ultrapassou o buffer_size
    */
    while (read != 1 || i+1 <= buffer_size) {
        //Verifica se a position estA livre
        if (position[i] == 1){
            ad = buffer->ad[i];
            read = 1;
        }
        i++;
    }

    if (read == 0) {
        ad->id = -1;
    }

    return 0;
}

/* Função que lê uma admissão do buffer de memória partilhada entre os rececionistas e os médicos,
* se houver alguma disponível para ler dirigida ao médico especificado. A leitura deve
* ser feita tendo em conta o tipo de buffer e as regras de leitura em buffers desse tipo. Se não houver
* nenhuma admissão disponível, afeta ad->id com o valor -1.
*/
void read_receptionist_doctor_buffer(struct circular_buffer* buffer, int doctor_id, int buffer_size, struct admission* ad){
    int in = buffer->ptrs->in;
    int out = buffer->ptrs->out;
    int read = 0;
    //Itera pelo ads contidos no buffer
    while ((out+1) % buffer_size != in){
        //Verifica se o doutor foi pedido
        if (buffer->buffer[out]->requested_doctor = doctor_id) {
            ad = buffer->buffer[out];
            read = 1;
        }
        out++;
    }
    //Caso nAo tenha lido uma ad disponIvel
    if (read == 0) {
        ad->id = -1;
    }
        
    return 0;
}