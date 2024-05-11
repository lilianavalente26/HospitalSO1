/* Grupo SO-024
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


void allocate_dynamic_memory_buffers(struct data_container* data) {
    data->patient_pids = allocate_dynamic_memory(data->n_patients*sizeof(int));
    data->receptionist_pids = allocate_dynamic_memory(data->n_receptionists*sizeof(int));
    data->doctor_pids = allocate_dynamic_memory(data->n_doctors*sizeof(int));
}


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

void launch_processes(struct data_container* data, struct communication* comm) {
    //DA launch a cada pacient
    for (int i = 0; i < data->n_patients; i++){
        data->patient_pids[i] = launch_patient(i,data,comm);
    }
    //DA launch a cada recepcionist
    for (int i = 0; i < data->n_receptionists; i++) {
        data->receptionist_pids[i] = launch_receptionist(i,data,comm);
    }
    //DA launch a cada doctor
    for (int i = 0; i < data->n_doctors; i++) {
        //data->doctor_pids[i] = launch_doctor(i,data,comm);
    }
}

void user_interaction(struct data_container* data, struct communication* comm) {
    char input[20]; 
    int ad_counter = 0;
    while (*data->terminate != 1)
    {
        printf(">Introduza um dos 4 comandos: ad (paciente) (mEdico), info, help, end):");
        printf("\n");
        scanf("%s", input);
        if(strcmp(input,"info") == 0){
            read_info(data);
        }
        else if(strcmp(input,"help") == 0){
            printf("Pode introduzir somente as seguintes instrucOes: \n"
            "ad (paciente) (mEdico) - cria uma nova admissAo, atravEs da funcAo create_request\n"
            "info - estado de uma admissAo\n"
            "help - informacAo sobre os comandos disponIveis\n"
            "end - termina o execucAo do hOSpital\n");
        }
        else if(strcmp(input,"end") == 0){
            end_execution(data, comm);
            exit(0);
        }
        else if(strcmp(input,"ad") == 0){;
            create_request(&ad_counter, data, comm);
        }
        else{
            printf("A palavra introduzida nao E vAlida.\n" 
            "Pode introduzir somente as seguintes instrucOes: \n"
            "ad (paciente) (médico) - cria uma nova admissAo, atravEs da funcAo create_request\n"
            "info - estado de uma admissAo\n"
            "help - informacAo sobre os comandos disponIveis\n"
            "end - termina o execucAo do hOSpital\n");
        }
    }
}

/* MEtodo Auxiliar
* Verifica se o patient_id E vAlido.
* Se vAlido, retorna o patient_id.
* Caso contrArio, pede um novo patient_id.
*/
int IDCheckerPatient(int patient_id, struct data_container* data) {
    scanf("%d", &patient_id);
    if (patient_id >= 0 && patient_id < data->n_patients) {
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
    if (receptionist_id >= 0 && receptionist_id < data->n_receptionists) {
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
    if (doctor_id >= 0 && doctor_id < data->n_doctors) {
        return doctor_id;
    }
    else {
        printf("Id invAlido, insira um id vAlido:");
        return IDCheckerDoctor(doctor_id, data);
    }
}

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
    printf("O id da nova admissao eh: %d\n", newAd->id);
    data->results[newAd->id] = *newAd;
    *ad_counter += 1;

    //atualiza data

    patient_receive_admission(newAd,patient_id,data,comm);
    patient_send_admission(newAd,data,comm);
    receptionist_receive_admission(newAd,data,comm);
    receptionist_send_admission(newAd,data,comm);
    doctor_receive_admission(newAd,doctor_id,data,comm);

    data->results[newAd->id] = *newAd;
}

void read_info(struct data_container* data){
    //Perguntar pela admisssion
    int admission_id;
    printf(">Qual o id de admissao a consultar? ");
    scanf("%d", &admission_id);

    int i = 0;
    int found = 0;
    //Verifica se admission E vAlida
    if (admission_id >= 0 && admission_id < data->max_ads) {
        //Loop até encontrar a admission
        while (found != 1 && i < MAX_RESULTS) {
            if (data->results[i].id == admission_id){
                found = 1;
                struct admission* ad = &data->results[i];
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
    }
    //Caso nAo seja vAlido 
    else {
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

    for (int i = 0; i < MAX_RESULTS; i++) {
        if (i == 0) {
            printf("[%d,", data->results[i].id);
        }
        else {
            printf("%d,", data->results[i].id);
        }
    }
    /*
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
    */
    printf("\n");
}

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

void end_execution(struct data_container* data, struct communication* comm){
    *data->terminate = 1;
    wait_processes(data);
    write_statistics(data);
    destroy_memory_buffers(data,comm);
}

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

void write_statistics(struct data_container* data){
    //verifica-se a flag terminate esta a 1, o que indica que o programa ja terminou
    if(*data->terminate == 1){
        for (int i = 0; i < data->n_patients; i++)
        {
            printf("O numero de admissoes solicitadas pelo paciente %d, corresponde a %d\n",i, data->patient_stats[i]);
        }
        for (int i = 0; i < data->n_receptionists; i++)
        {
            printf("O numero de admissoes realizadas pelo rececionista %d, corresponde a %d\n",i, data->receptionist_stats[i]);
        }
        for (int i = 0; i < data->n_doctors; i++)
        {
            printf("O numero de admissoes atendidas pelo medico %d, corresponde a %d\n",i, data->doctor_stats[i]);
        }
    }
}

void destroy_memory_buffers(struct data_container* data, struct communication* comm){
    //dealocar memOria dinAmica
    deallocate_dynamic_memory(data->patient_pids);
    deallocate_dynamic_memory(data->receptionist_pids);
    deallocate_dynamic_memory(data->doctor_pids);
    //destrOi meOria partilhada
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