#include <stdio.h>
#include <main.h>

int main(int argc, char *argv[]) {
    //init data structures
    struct data_container* data = allocate_dynamic_memory(sizeof(struct data_container));
    struct communication* comm = allocate_dynamic_memory(sizeof(struct communication));
    comm->main_patient = allocate_dynamic_memory(sizeof(struct circular_buffer));
    comm->patient_receptionist = allocate_dynamic_memory(sizeof(struct rnd_access_buffer));
    comm->receptionist_doctor = allocate_dynamic_memory(sizeof(struct circular_buffer));
    
    //execute main code
    main_args(argc, argv, data);
    allocate_dynamic_memory_buffers(data);
    create_shared_memory_buffers(data, comm);
    launch_processes(data, comm);
    user_interaction(data, comm);
    
    //release memory before terminating
    deallocate_dynamic_memory(data);
    deallocate_dynamic_memory(comm->main_patient);
    deallocate_dynamic_memory(comm->patient_receptionist);
    deallocate_dynamic_memory(comm->receptionist_doctor);
    deallocate_dynamic_memory(comm);
}
/* Função que lê os argumentos da aplicação, nomeadamente o número
* máximo de admissões, o tamanho dos buffers de memória partilhada
* usados para comunicação, e o número de pacientes, de rececionistas e de
* médicos. Guarda esta informação nos campos apropriados da
* estrutura data_container.
*/
void main_args(int argc, char* argv[], struct data_container* data){

}

/* Função que reserva a memória dinâmica necessária para a execução
* do hOSpital, nomeadamente para os arrays *_pids e *_stats da estrutura 
* data_container. Para tal, pode ser usada a função allocate_dynamic_memory.
*/
void allocate_dynamic_memory_buffers(struct data_container* data){
    
}

/* Função que reserva a memória partilhada necessária para a execução do
* hOSpital. É necessário reservar memória partilhada para todos os buffers da
* estrutura communication, incluindo os buffers em si e respetivos
* pointers, assim como para o array data->results e variável data->terminate.
* Para tal, pode ser usada a função create_shared_memory.
*/
void create_shared_memory_buffers(struct data_container* data, struct communication* comm){
    // reserva SHM para pacientes
    comm->main_patient->ptrs =(struct pointers*)create_shared_memory(STR_SHM_MAIN_PATIENT_PTR, sizeof(struct pointers));
    comm->main_patient->buffer =(struct admission*)create_shared_memory(STR_SHM_MAIN_PATIENT_BUFFER, data->buffers_size * sizeof(struct admission));

    // reserva SHM para pacientes-recepcionistas
    comm->patient_receptionist->buffer =(struct admission*)create_shared_memory(STR_SHM_PATIENT_RECEPT_BUFFER, data->buffers_size * sizeof(struct admission));
    comm->patient_receptionist->ptrs =(int*)create_shared_memory(STR_SHM_PATIENT_RECEPT_PTR, data->n_patients * sizeof(int));

    // reserva SHM para recepcionistas-doutores
    comm->receptionist_doctor->buffer =(struct admission*)create_shared_memory(STR_SHM_RECEPT_DOCTOR_BUFFER, data->buffers_size * sizeof(struct admission));
    comm->receptionist_doctor->ptrs =(struct pointers*)create_shared_memory(STR_SHM_RECEPT_DOCTOR_PTR, sizeof(struct pointers));

    // sizeOf(int) é o espaço na memória em bytes ocupado por um número inteiro
    // reserva SHM para os stats
    data->receptionist_stats = (int*)create_shared_memory(STR_SHM_RECEPT_STATS, data->n_receptionists * sizeof(int));
    data->doctor_stats = (int*)create_shared_memory(STR_SHM_DOCTOR_STATS, data->n_doctors * sizeof(int));

    // reserva SHM para os diagnosticos
    data->results = (struct admission*)create_shared_memory(STR_SHM_RESULTS, MAX_RESULTS * sizeof(struct admission));

    // reserva SHM para terminate
    data->terminate = (int*)create_shared_memory(STR_SHM_TERMINATE, sizeof(int));
    *data->terminate = 0;
}

/* Função que inicia os processos dos pacientes, rececionistas e
* médicos. Para tal, pode usar as funções launch_*,
* guardando os pids resultantes nos arrays respetivos
* da estrutura data.
*/
void launch_processes(struct data_container* data, struct communication* comm){

}

/* Função que faz a interação do utilizador, podendo receber 4 comandos:
* ad paciente médico - cria uma nova admissão, através da função create_request
* info - verifica o estado de uma admissão através da função read_info
* help - imprime informação sobre os comandos disponiveis
* end - termina o execução do hOSpital através da função stop_execution
*/
void user_interaction(struct data_container* data, struct communication* comm){
    char input[5]; 
    int i = 1;
    scanf("%s" , input);
    if(strcmp(input,"info")){
        read_info(data);
    }
    else if(strcmp(input,"help")){
        printf("Pode introduzir somente as seguintes instrucoes: \n"
        "ad paciente médico - cria uma nova admissão, através da função create_request\n"
        "info - estado de uma admissão\n"
        "help - informacao sobre os comandos disponiveis\n"
        "end - termina o execução do hOSpital\n ");
    }
    else if(strcmp(input,"end")){
        end_execution(data, comm);
    }
    else if(strcmp(input,"ad paciente médico")){
        create_request(i, data, comm);
        i++;
    }
    else{
        printf("a palavra introduzida nao e valida.\n" 
        "Pode introduzir somente as seguintes instrucoes: \n"
        "ad paciente médico - cria uma nova admissão, através da função create_request\n"
        "info - estado de uma admissão\n"
        "help - informacao sobre os comandos disponiveis\n"
        "end - termina o execução do hOSpital\n ");
    }
}

/* Cria uma nova admissão identificada pelo valor atual de ad_counter e com os 
* dados introduzidos pelo utilizador na linha de comandos, escrevendo a mesma 
* no buffer de memória partilhada entre a main e os pacientes. Imprime o id da 
* admissão e incrementa o contador de admissões ad_counter.
*/
void create_request(int* ad_counter, struct data_container* data, struct communication* comm);

/* Função que lê um id de admissão do utilizador e verifica se a mesma é valida.
* Em caso afirmativo imprime a informação da mesma, nomeadamente o seu estado, o 
* id do paciente que fez o pedido, o id do médico requisitado, e os ids do paciente,
* rececionista, e médico que a receberam e processaram.
*/
void read_info(struct data_container* data){
    int admission_id;
    printf("Qual o id de admissao a consultar? ");
    scanf("%d", &admission_id);

    if (admission_id >= 0 && admission_id < data->max_ads) {
        struct admission* ad = &(data->results[admission_id]);
        printf("Informacoes da admissao %d:\n", admission_id);
        printf("Estado da admissao: %c\n", ad->status);
        printf("id do paciente que fez o pedido: %d\n", ad->requesting_patient);
        printf("id do medico requisitado: %d\n", ad->requested_doctor);
        printf("id do paciente que recebeu a admissao: %d\n", ad->receiving_patient);
        printf("id do recepcionista que realizou a admissao: %d\n", ad->receiving_receptionist);
        printf("id do medico que realizou a consulta: %d\n", ad->receiving_doctor);
    } else {
        printf("id de admissao invalido!\n");
    }
}


/* Função que imprime o estado do data_container, nomeadamente todos os seus campos.
* No caso dos arrays, deve-se imprimir no formato [0, 1, 2, ..., N], onde N é o último elemento do array.
*/
void print_status(struct data_container* data);

/* Função que termina a execução do programa hOSpital. Deve começar por 
* afetar a flag data->terminate com o valor 1. 
* De seguida, e por esta ordem, deve esperar que os processos filho terminem, 
* deve escrever as estatisticas finais do programa, e por fim libertar
* as zonas de memória partilhada e dinâmica previamente 
* reservadas. Para tal, pode usar as outras funções auxiliares do main.h.
*/
void end_execution(struct data_container* data, struct communication* comm){
    data->terminate = 1;
    wait_processes(data);
    write_statistic(data);
    destroy_memory_buffers(data,comm);
}

/* Função que espera que todos os processos previamente iniciados terminem,
* incluindo pacientes, rececionistas e médicos. Para tal, pode usar a função 
* wait_process do process.h.
*/
void wait_processes(struct data_container* data){
    
}

/* Função que imprime as estatisticas finais do hOSpital, nomeadamente quantas
* admissões foram solicitadas por cada paciente, realizadas por cada rececionista
* e atendidas por cada médico.
*/
void write_statistics(struct data_container* data);

/* Função que liberta todos os buffers de memória dinâmica e partilhada previamente
* reservados na estrutura data.
*/
void destroy_memory_buffers(struct data_container* data, struct communication* comm);
