/*
* Grupo SO-024
* Afonso Santos - fc59808
* Madalena Machado - fc59858
* Liliana Valente - fc59846
*/

#include <stdio.h>
#include <main.h>
#include <doctor.h>
#include <patient.h>
#include <receptionist.h>
#include <process.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* Função que lê os argumentos da aplicação, nomeadamente o número
* máximo de admissões, o tamanho dos buffers de memória partilhada
* usados para comunicação, e o número de pacientes, de rececionistas e de
* médicos. Guarda esta informação nos campos apropriados da
* estrutura data_container.
*/
void main_args(int argc, char* argv[], struct data_container* data) {
    if (argc != 6) {
        perror("NUmero de argumentos inválidos");
        exit(1);
    }
    data->max_ads = atoi(argv[1]);
    data->buffers_size = atoi(argv[2]);
    data->n_patients = atoi(argv[3]);
    data->n_receptionists = atoi(argv[4]);
    data->n_doctors = atoi(argv[5]);
}

/* Função que reserva a memória dinâmica necessária para a execução
* do hOSpital, nomeadamente para os arrays *_pids e *_stats da estrutura 
* data_container. Para tal, pode ser usada a função allocate_dynamic_memory.
*/
void allocate_dynamic_memory_buffers(struct data_container* data) {
    data->patient_pids = allocate_dynamic_memory(data->n_patients*sizeof(int));
    data->receptionist_pids = allocate_dynamic_memory(data->n_receptionists*sizeof(int));
    data->doctor_pids = allocate_dynamic_memory(data->n_doctors*sizeof(int));
}

/* Função que reserva a memória partilhada necessária para a execução do
* hOSpital. É necessário reservar memória partilhada para todos os buffers da
* estrutura communication, incluindo os buffers em si e respetivos
* pointers, assim como para o array data->results e variável data->terminate.
* Para tal, pode ser usada a função create_shared_memory.
*/
void create_shared_memory_buffers(struct data_container* data, struct communication* comm) {
    comm->main_patient->ptrs = create_shared_memory(STR_SHM_MAIN_PATIENT_PTR, sizeof(struct pointers));
    comm->main_patient->buffer = create_shared_memory(STR_SHM_MAIN_PATIENT_BUFFER, data->buffers_size * sizeof(struct admission));

    comm->patient_receptionist->ptrs = create_shared_memory(STR_SHM_PATIENT_RECEPT_PTR, data->n_patients * sizeof(int));
    comm->patient_receptionist->buffer = create_shared_memory(STR_SHM_PATIENT_RECEPT_BUFFER, data->buffers_size * sizeof(struct admission));

    comm->receptionist_doctor->ptrs = create_shared_memory(STR_SHM_RECEPT_DOCTOR_PTR, sizeof(struct pointers));
    comm->receptionist_doctor->buffer = create_shared_memory(STR_SHM_RECEPT_DOCTOR_BUFFER, data->buffers_size * sizeof(struct admission));

    data->patient_stats = create_shared_memory(STR_SHM_PATIENT_STATS, data->n_patients * sizeof(int));
    data->receptionist_stats = create_shared_memory(STR_SHM_RECEPT_STATS, data->n_receptionists * sizeof(int));
    data->doctor_stats = create_shared_memory(STR_SHM_DOCTOR_STATS, data->n_doctors * sizeof(int));
   
    data->results = create_shared_memory(STR_SHM_RESULTS, MAX_RESULTS * sizeof(struct admission));

    data->terminate = create_shared_memory(STR_SHM_TERMINATE, sizeof(int));
    *data->terminate = 0; 
}

/* Função que inicia os processos dos pacientes, rececionistas e
* médicos. Para tal, pode usar as funções launch_*,
* guardando os pids resultantes nos arrays respetivos
* da estrutura data.
*/
void launch_processes(struct data_container* data, struct communication* comm) {
    //DA launch a cada pacient
    for (int i = 0; i < data->n_patients; i++){
        launch_patient(data->patient_pids[i],data,comm);
    }
    //DA launch a cada recepcionist
    for (int i = 0; i < data->n_receptionists; i++) {
        launch_receptionist(data->receptionist_pids[i],data,comm);
    }
    //DA launch a cada doctor
    for (int i = 0; i < data->n_doctors; i++) {
        launch_doctor(data->doctor_pids[i],data,comm);
    }
}

/* Função que faz a interação do utilizador, podendo receber 4 comandos:
* ad paciente médico - cria uma nova admissão, através da função create_request
* info - verifica o estado de uma admissão através da função read_info
* help - imprime informação sobre os comandos disponiveis
* end - termina o execução do hOSpital através da função stop_execution
*/
void user_interaction(struct data_container* data, struct communication* comm) {
    printf(">Introduza um dos 4 comandos: ad (paciente) (médico), info, help, end):");
    printf("\n");
    char input[20]; 
    scanf("%s", input);
    if(strcmp(input,"info") == 0){
        read_info(data);
        user_interaction(data, comm);
    }
    else if(strcmp(input,"help") == 0){
        printf("Pode introduzir somente as seguintes instrucoes: \n"
        "ad (paciente) (médico) - cria uma nova admissão, através da função create_request\n"
        "info - estado de uma admissão\n"
        "help - informacao sobre os comandos disponiveis\n"
        "end - termina o execução do hOSpital\n");
        user_interaction(data, comm);
    }
    else if(strcmp(input,"end") == 0){
        end_execution(data, comm);
        exit(0);
    }
    else if(strcmp(input,"ad") == 0){
        int *ad_counter = data->patient_stats;
        create_request(ad_counter, data, comm);
        user_interaction(data, comm);
    }
    else{
        printf("A palavra introduzida nao e valida.\n" 
        "Pode introduzir somente as seguintes instrucoes: \n"
        "ad (paciente) (médico) - cria uma nova admissão, através da função create_request\n"
        "info - estado de uma admissão\n"
        "help - informacao sobre os comandos disponiveis\n"
        "end - termina o execução do hOSpital\n");
        user_interaction(data, comm);
    }
}

/* MEtodo Auxiliar
* Verifica se o patient_id E vAlido.
* Se vAlido, retorna o patient_id.
* Caso contrArio, pede um novo patient_id.
*/
int IDCheckerPatient(int patient_id, struct data_container* data) {
    scanf("%d", &patient_id);
    if (patient_id > 0 && patient_id <= data->n_patients) {
        return patient_id;
    }
    else {
        printf("Id invAlido, insira um id vAlido:");
        return IDCheckerPatient(patient_id, data);
    }
}

/* MEtodo Auxiliar
* Verifica se o recepcionist_id E vAlido.
* Se vAlido, retorna o recepcionist_id.
* Caso contrArio, pede um novo recepcionist_id.
*/
int IDCheckerReceptionist(int receptionist_id, struct data_container* data) {
    scanf("%d", &receptionist_id);
    if (receptionist_id > 0 && receptionist_id <= data->n_receptionists) {
        return receptionist_id;
    }
    else {
        printf("Id invAlido, insira um id vAlido:");
        return IDCheckerReceptionist(receptionist_id, data);
    }
}

/* MEtodo Auxiliar
* Verifica se o doctor_id E vAlido.
* Se vAlido, retorna o doctor_id.
* Caso contrArio, pede um novo doctor_id.
*/
int IDCheckerDoctor(int doctor_id, struct data_container* data) {
    scanf("%d", &doctor_id);
    if (doctor_id > 0 && doctor_id <= data->n_doctors) {
        return doctor_id;
    }
    else {
        printf("Id invAlido, insira um id vAlido:");
        return IDCheckerDoctor(doctor_id, data);
    }
}

/* Cria uma nova admissão identificada pelo valor atual de ad_counter e com os 
* dados introduzidos pelo utilizador na linha de comandos, escrevendo a mesma 
* no buffer de memória partilhada entre a main e os pacientes. Imprime o id da 
* admissão e incrementa o contador de admissões ad_counter.
*/
void create_request(int* ad_counter, struct data_container* data, struct communication* comm) {
    struct admission *newAd = allocate_dynamic_memory(sizeof(struct admission));
    int patient_id = 0;
    int doctor_id = 0;

    printf("Insira id do paciente: ");
    patient_id = IDCheckerPatient(patient_id, data);
    printf("Insira id do medico pretendido: ");
    doctor_id = IDCheckerDoctor(doctor_id, data);

    newAd->id = *ad_counter;
    newAd->requesting_patient = patient_id;
    newAd->requested_doctor = doctor_id;

    write_main_patient_buffer(comm->main_patient, data->buffers_size, newAd);
    printf("O id da nova admissao eh: %d\n", *ad_counter);
    ad_counter++;
    
    /*
    int requesting_patient = 0, requested_doctor = 0, receiving_patient = 0,
        receiving_receptionist = 0, receiving_doctor = 0;

    //cria uma nova admissao
    struct admission *newAd = NULL;

    newAd = malloc(sizeof(struct admission));
    if (newAd == NULL) {
        perror("create_request: malloc failed");
        exit(EXIT_FAILURE);
    }

    newAd->id = *ad_counter;
    newAd->status = 'N';

    printf("Insira id do paciente: ");
    newAd->requesting_patient = IDCheckerPatient(requesting_patient, data);

    printf("Insira id do medico pretendido: ");
    newAd->requested_doctor = IDCheckerDoctor(requested_doctor, data);

    printf("Insira id do paciente que recebeu a admissao: ");
    newAd->receiving_patient = IDCheckerPatient(receiving_patient, data);

    printf("Insira id do rececionista que realizou a admissao: ");
    newAd->receiving_receptionist = IDCheckerReceptionist(receiving_receptionist, data);

    printf("Insira id do medico que realizou a consulta: ");
    newAd->receiving_doctor = IDCheckerDoctor(receiving_doctor, data);

    write_main_patient_buffer(comm->main_patient, data->buffers_size, newAd);
    printf("O id da nova admissao eh: %d\n", *ad_counter);
    ad_counter++;

    free(newAd);
    */
}

/* Função que lê um id de admissão do utilizador e verifica se a mesma é valida.
* Em caso afirmativo imprime a informação da mesma, nomeadamente o seu estado, o 
* id do paciente que fez o pedido, o id do médico requisitado, e os ids do paciente,
* rececionista, e médico que a receberam e processaram.
*/
void read_info(struct data_container* data){
    int admission_id;
    printf(">Qual o id de admissao a consultar? ");
    scanf("%d", &admission_id);

    int i = 0;
    int found = 0;
    if (admission_id >= 0 && admission_id < data->max_ads) {
        while (found != 1 && i < MAX_RESULTS) {
            if (data->results[i].id == admission_id){
                printf("found");
                found = 1;
                struct admission* ad = &data->results[i-1];
                printf("Informacoes da admissao '%d':\n", admission_id);
                printf("Estado da admissao: %c\n", ad->status);
                printf("id do paciente que fez o pedido: %d\n", ad->requesting_patient);
                printf("id do medico requisitado: %d\n", ad->requested_doctor);
                printf("id do paciente que recebeu a admissao: %d\n", ad->receiving_patient);
                printf("id do recepcionista que realizou a admissao: %d\n", ad->receiving_receptionist);
                printf("id do medico que realizou a consulta: %d\n", ad->receiving_doctor);
            }
            i++;
        }
    } else {
        printf("id de admissao invalido!\n");
    }
}

/* MEtodo Auxiliar
*Imprime os patient_ids contidos no data
*/
void print_patient_ids(struct data_container* data) {
    printf("Patient ids:");
    for (int i = 0; i < data->n_patients; i++){
        if (i == 0) {
            printf("[%d,", data->patient_pids[i]);
        }
        else if (i+1 <= data->n_patients){
            printf("%d,", data->patient_pids[i]);
        }
        else{
            printf("%d]", data->patient_pids[i]);
        }
    }
    printf("\n");
}

/* MEtodo Auxiliar
*Imprime os receptionist_ids contidos no data
*/
void print_receptionist_ids(struct data_container* data){
    printf("Receptionist ids:");
    for (int i = 0; i < data->n_receptionists; i++){
        if (i == 0) {
            printf("[%d,", data->receptionist_pids[i]);
        }
        else if (i+1 <= data->n_receptionists){
            printf("%d,", data->receptionist_pids[i]);
        }
        else{
            printf("%d]", data->receptionist_pids[i]);
        }
    }
    printf("\n");
}

/* MEtodo Auxiliar
*Imprime os doctor_ids contidos no data
*/
void print_doctor_ids(struct data_container* data) {
    printf("Doctor ids:");
    for (int i = 0; i < data->n_doctors; i++){
        if (i == 0) {
            printf("[%d,", data->doctor_pids[i]);
        }
        else if (i+1 <= data->n_doctors){
            printf("%d,", data->doctor_pids[i]);
        }
        else{
            printf("%d]", data->doctor_pids[i]);
        }
    }
    printf("\n");
}

/* MEtodo Auxiliar
*Imprime os results contidos no data
*/
void print_data_results(struct data_container* data) {
    printf("Admission results:");
    int j = 0;
    while (data->results[j].id != -1){
        if (j == 0) {
            printf("[%d,", data->results[j].id);
        }
        else if (data->results[j+1].id == -1){
            printf("%d,", data->results[j].id);
        }
        else{
            printf("%d]", data->results[j].id);
        }
        j++;
    }
    printf("\n");
}

/* Função que imprime o estado do data_container, nomeadamente todos os seus campos.
* No caso dos arrays, deve-se imprimir no formato [0, 1, 2, ..., N], onde N é o último elemento do array.
*/
void print_status(struct data_container* data) {
    printf("max_ads:%d\n",data->max_ads);
    printf("buffers_size:%d\n",data->buffers_size);
    printf("n_patient:%d\n",data->n_patients);
    printf("n_receptionists:%d\n",data->n_receptionists);
    printf("n_doctors:%d\n",data->n_doctors);
    printf("\n");

    print_patient_ids(data);
    print_receptionist_ids(data);
    print_doctor_ids(data);
    print_data_results(data);

    printf("%d", *data->terminate);
}

/* Função que termina a execução do programa hOSpital. Deve começar por 
* afetar a flag data->terminate com o valor 1. De seguida, e por esta
* ordem, deve esperar que os processos filho terminem, deve escrever as
* estatisticas finais do programa, e por fim libertar
* as zonas de memória partilhada e dinâmica previamente 
* reservadas. Para tal, pode usar as outras funções auxiliares do main.h.
*/
void end_execution(struct data_container* data, struct communication* comm){
    *data->terminate = 1;
    wait_processes(data);
    write_statistics(data);
    destroy_memory_buffers(data,comm);
}

/* Função que espera que todos os processos previamente iniciados terminem,
* incluindo pacientes, rececionistas e médicos. Para tal, pode usar a função 
* wait_process do process.h.
*/
void wait_processes(struct data_container* data) {
    //espera pelo pacients
    for (int i = 0; i < data->n_patients; i++) {
        wait_process(data->patient_pids[i]);
    }
    //espera pelo recepcionists
    for (int i = 0; i < data->n_receptionists; i++) {
        wait_process(data->receptionist_pids[i]);
    }
    //espera pelo doctors
    for (int i = 0; i < data->n_doctors; i++) {
        wait_process(data->doctor_pids[i]);
    }
}

/* Função que imprime as estatisticas finais do hOSpital, nomeadamente quantas
* admissões foram solicitadas por cada paciente, realizadas por cada rececionista
* e atendidas por cada médico.
*/
void write_statistics(struct data_container* data){
    //verifica-se a flag terminate esta a 1, o que indica que o programa ja terminou
    if(*data->terminate == 1){
        for (int i = 0; i < data->n_patients; i++)
        {
            printf("O numero de admissoes solicitadas por cada paciente corresponte a %d\n",data->patient_stats[i]);
        }
        for (int i = 0; i < data->n_receptionists; i++)
        {
            printf("O numero de admissoes realizadas por cada rececionista corresponte a %d\n",data->receptionist_stats[i]);
        }
        for (int i = 0; i < data->n_receptionists; i++)
        {
            printf("O numero de admissoes atendidas por cada medico corresponte a %d\n",data->doctor_stats[i]);
        }
    }
}

/* Função que liberta todos os buffers de memória dinâmica e partilhada previamente
* reservados na estrutura data.
*/
void destroy_memory_buffers(struct data_container* data, struct communication* comm){
    //dealocar memOria dinAmica
    deallocate_dynamic_memory(data->patient_pids);
    deallocate_dynamic_memory(data->receptionist_pids);
    deallocate_dynamic_memory(data->doctor_pids);
    //detrOi meOria partilhada
    destroy_shared_memory(STR_SHM_MAIN_PATIENT_PTR, comm->main_patient->ptrs, sizeof(struct pointers));
    destroy_shared_memory(STR_SHM_MAIN_PATIENT_BUFFER, comm->main_patient->buffer, data->n_patients * sizeof(int));
    destroy_shared_memory(STR_SHM_PATIENT_RECEPT_PTR, comm->patient_receptionist->ptrs, data->n_patients * sizeof(int));
    destroy_shared_memory(STR_SHM_PATIENT_RECEPT_BUFFER, comm->patient_receptionist->buffer, data->buffers_size * sizeof(struct admission));
    destroy_shared_memory(STR_SHM_RECEPT_DOCTOR_PTR, comm->receptionist_doctor->ptrs, sizeof(struct pointers));
    destroy_shared_memory(STR_SHM_RECEPT_DOCTOR_BUFFER, comm->receptionist_doctor->buffer, data->buffers_size * sizeof(struct admission));
    destroy_shared_memory(STR_SHM_PATIENT_STATS, data->patient_stats, data->n_patients * sizeof(int));
    destroy_shared_memory(STR_SHM_RECEPT_STATS, data->receptionist_stats, data->n_receptionists * sizeof(int));
    destroy_shared_memory(STR_SHM_DOCTOR_STATS, data->doctor_stats, data->n_doctors * sizeof(int));
    destroy_shared_memory(STR_SHM_RESULTS, data->results, MAX_RESULTS * sizeof(struct admission));
    destroy_shared_memory(STR_SHM_TERMINATE, data->terminate, sizeof(int));
}

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
