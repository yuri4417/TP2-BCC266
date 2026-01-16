
#ifndef RAM_H
#define RAM_H
#include <stdio.h>
#include "structs.h"
#include <stdbool.h>


LinhaCache* criaRAM_vazia(int tamanho);
LinhaCache* criaRAM_aleatoria(int tamanho);
void liberaRAM(LinhaCache* ram);
void store(LinhaCache* ram, int pos, LinhaCache dado);
LinhaCache load(LinhaCache* ram, int endRAM);
void imprimeLinhaCache(LinhaCache* LinhaCache);
void imprimeRAM(LinhaCache* ram, int tam);

#endif