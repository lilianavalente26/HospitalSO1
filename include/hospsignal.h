#ifndef HOSPSIGNAL_H_GUARD
#define HOSPSIGNAL_H_GUARD

#include "memory.h"
#include "synchronization.h"

/* Função que imprime o estado atual de todas as admissões, incluindo as que já foram finalizadas
pelos médicos e as que ainda estão em andamento
*/
print_admission_status(struct data_container* data, struct communication* comm, struct semaphores* sems);

#endif
