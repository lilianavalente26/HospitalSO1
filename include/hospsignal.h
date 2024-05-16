#ifndef HOSPSIGNAL_H_GUARD
#define HOSPSIGNAL_H_GUARD

#include "memory.h"
#include "synchronization.h"

/*
 * Função que imprime o estado atual de todas as admissões, incluindo as que já foram finalizadas
 * pelos médicos e as que ainda estão em andamento.
*/
void print_admission_status(struct data_container* data, struct semaphores* sems);

/*
 * Função que interrompe a execução do programa, ao capturar o sinal de interrupção do programa 
 * (SIGINT - ativado pela combinação de teclas CTRL-C). Deve proceder-se à invocação da função 
 * end_execution e garantir que tanto o processo pai como os processos filhos capturam o sinal.
*/
void interrupt_program(struct data_container* data, struct communication* comm, struct semaphores* sems);

#endif
