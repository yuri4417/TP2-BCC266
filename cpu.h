#ifndef CPU_H
#define CPU_H

#include "structs.h"
  
void cpu(Cache *L1, Cache *L2, Cache *L3, LinhaCache *RAM, Instrucao *programa, long int *relogio, WriteBuffer *buffer, ConfigItem *configs);





#endif