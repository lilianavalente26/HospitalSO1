/*
* Grupo SO-024
* Afonso Santos - fc59808
* Madalena Machado - fc59858
* Liliana Valente - fc59846
*/

#include <stdio.h>
#include "../include/memory.h"
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

void* create_shared_memory(char* name, int size){
    int shm = shm_open(name, O_CREAT | O_RDWR , 0666);
    ftruncate(shm, size); 
    if(shm == -1){
        perror ("create_shared_memory_1");
        exit(1);
    }
    // com a notacao NULL e o SO que determina automaticamente o endereco
    // inicial da projecao
    void* ptrSharedMemory = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);
    if(ptrSharedMemory == MAP_FAILED){
        perror("create_shared_memory_2");
        exit(1);
    }

    memset(ptrSharedMemory, 0, size);

    return ptrSharedMemory;
}

void* allocate_dynamic_memory(int size){
    //aloca memOria dinAmica com size
    int *ptr = malloc(size);
    //Houve erro a alocar
    if (ptr == NULL){
        perror("allocate_dynamic_memory");
        exit(1);
    }
    //preenche essa zona de memória com o valor 0
    *ptr = 0;
    //retorna o apontador para a mesma
    return ptr;
}

void destroy_shared_memory(char* name, void* ptr, int size){
    int statusMap = munmap(ptr, size); 

    if(statusMap == -1){
        perror("destroy_shared_memory_1");
        exit(1);
    }

    int statusPointer = shm_unlink(name);

    if(statusPointer == -1){
        perror("destroy_shared_memory_2");
        exit(1);
    }
}

void deallocate_dynamic_memory(void* ptr){
    if (ptr != NULL){
        free(ptr);
        ptr = NULL;
    }
}

void write_main_patient_buffer(struct circular_buffer* buffer, int buffer_size, struct admission* ad){
    int in = buffer->ptrs->in;
    int out = buffer->ptrs->out;
    //Verifica se o buffer estA cheio
    if ((in+1) % buffer_size != out) {
        buffer->buffer[in] = *ad;
        buffer->ptrs->in = (in + 1) % buffer_size;
    }
}

void write_patient_receptionist_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct admission* ad){
    int i = 0;
    int written = 0;
    /* Itera pelo buffer, procurando pelas posiCOes vazias
    *  Verifica se Já escreveu ou se ultrapassou o buffer_size
    */
    while (written != 1 || i+1 <= buffer_size) {
        //Verifica se a position estA livre
        if (buffer->ptrs[i] == 0){
            buffer->buffer[i] = *ad;
            buffer->ptrs[i] = 1;
            written = 1;
        }
        i++;
    }
}

void write_receptionist_doctor_buffer(struct circular_buffer* buffer, int buffer_size, struct admission* ad){
    int in = buffer->ptrs->in;
    int out = buffer->ptrs->out;
    //Verifica se o buffer estA cheio
    if ((in+1) % buffer_size != out) {
        buffer->buffer[in] = *ad;
        buffer->ptrs->in = (in + 1) % buffer_size;
    }
}

void read_main_patient_buffer(struct circular_buffer* buffer, int patient_id, int buffer_size, struct admission* ad){
    int in = buffer->ptrs->in;
    int out = buffer->ptrs->out;
    int read = 0;
    //Itera pelo ads contidos no buffer
    while ((out+1) % buffer_size != in){
        //Verifica se ad foi pedida pelo paciente
        if (buffer->buffer[out].requesting_patient == patient_id) {
            *ad = buffer->buffer[out];
            read = 1;
        }
        out++;
    }
    //Caso nAo tenha lido uma ad disponIvel
    if (read == 0) {
        ad->id = -1;
    }
}

void read_patient_receptionist_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct admission* ad){
    int i = 0;
    int read = 0;
    /* Itera pelo buffer, procurando pelas posiCOes vazias
    *  Verifica se Já escreveu ou se ultrapassou o buffer_size
    */
    while (read != 1 || i+1 <= buffer_size) {
        //Verifica se a position estA livre
        if (buffer->ptrs[i] == 1){
            *ad = buffer->buffer[i];
            read = 1;
        }
        i++;
    }

    if (read == 0) {
        ad->id = -1;
    }
}

void read_receptionist_doctor_buffer(struct circular_buffer* buffer, int doctor_id, int buffer_size, struct admission* ad){
    int in = buffer->ptrs->in;
    int out = buffer->ptrs->out;
    int read = 0;
    //Itera pelo ads contidos no buffer
    while ((out+1) % buffer_size != in){
        //Verifica se o doutor foi pedido
        if (buffer->buffer[out].requested_doctor == doctor_id) {
            *ad = buffer->buffer[out];
            read = 1;
        }
        out++;
    }
    //Caso nAo tenha lido uma ad disponIvel
    if (read == 0) {
        ad->id = -1;
    }
}
