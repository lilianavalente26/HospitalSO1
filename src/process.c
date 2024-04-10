#include "memory.h"
#include "main.h"
#include "process.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

/* Função que inicia um novo processo Paciente através da função fork do SO. O novo
* processo irá executar a função execute_patient respetiva, fazendo exit do retorno.
* O processo pai devolve o pid do processo criado.
*/
int launch_patient(int patient_id, struct data_container* data, struct communication* comm){
    int pid = fork();

    if (pid == -1) { /* Houve algum erro */
        perror("launch_patient");
        exit(1);
    }

    if (pid == 0) { /* Processo filho */
        execute_patient(patient_id, data, comm);
        exit(0);
    }

    else { /* Processo pai*/
        return pid;
    }
}

/* Função que inicia um novo processo Rececionista através da função fork do SO. O novo
* processo irá executar a função execute_receptionist, fazendo exit do retorno.
* O processo pai devolve o pid do processo criado.
*/
int launch_receptionist(int receptionist_id, struct data_container* data, struct communication* comm){
    int pid = fork();

    if (pid == -1) { /* Houve algum erro */
        perror("launch_receptionist");
        exit(1);
    }

    if (pid == 0) { /* Processo filho */
        execute_receptionist(receptionist_id, data, comm);
        exit(0);
    }

    else { /* Processo pai*/
        return pid;
    }
}

/* Função que inicia um novo processo Médico através da função fork do SO. O novo
* processo irá executar a função execute_doctor, fazendo exit do retorno.
* O processo pai devolve o pid do processo criado.
*/
int launch_doctor(int doctor_id, struct data_container* data, struct communication* comm){
    int pid = fork();

    if (pid == -1) { /* Houve algum erro */
        perror("launch_doctor");
        exit(-1);
    }

    if (pid == 0) { /* Processo filho */
        execute_doctor(doctor_id, data, comm);
        exit(0);
    }

    else { /* Processo pai*/
        return pid;
    }
}

/* Função que espera que um processo termine através da função waitpid. 
* Devolve o retorno do processo, se este tiver terminado normalmente.
*/
int wait_process(int process_id){
    int status;
    pid_t processo = waitpid(process_id, &status, 0);
    
    if (processo = -1) { /*Houve um erro*/
        perror("wait_process");
        return -1;
    }

    else if (WIFEXITED(status)) { /*processo terminou normalmente*/
        return WIFEXITED(status);
    }

    else { /*erro na terminação*/
        return -1;
    }
}