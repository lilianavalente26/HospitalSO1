#ifndef HOSPTIME_H
#define HOSPTIME_H

#include <time.h>
/*
 * Funcao que retorna o instante temporal que foi invocada
*/
int get_current_time(struct timespec *time);

#endif