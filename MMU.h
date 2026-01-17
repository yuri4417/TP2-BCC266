
#ifndef MMU_H
#define MMU_H


#include "structs.h"

Cache* criaCache(int qtdBlocos);
void destroiCache(Cache* c);

LinhaCache MMU_Read(Endereco add, Cache *L1, Cache *L2, Cache *L3, LinhaCache *RAM, WriteBuffer *buffer, int configBuffer, long int *relogio);
void MMU_Write(Cache *L1, Cache *L2, Cache *L3, LinhaCache *RAM, WriteBuffer *buffer, Endereco add, int valor, int configBuffer, long int *relogio);
#endif