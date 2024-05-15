/*
* Grupo SO-024
* Afonso Santos - fc59808
* Madalena Machado - fc59858
* Liliana Valente - fc59846
*/

#include "hosptime.h" // e suposto isto nao estar a dar erro
#include <stdio.h>
#include <linux/time.h>
 // isto tem que sair daqui, so esta aqui para que a funcao que chamei seja valida

int get_current_time(struct timespec *time){
    if(clock_gettime(CLOCK_REALTIME, time) == -1){
        print("Erro no get_current_time");
        return -1;
    }
    return clock_gettime(CLOCK_REALTIME, time);
}