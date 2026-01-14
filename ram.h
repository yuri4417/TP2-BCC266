#ifndef RAM_H
#define RAM_H
#include <stdio.h>
#include "structs.h"
#include <stdbool.h>

Bloco* criaRAM_vazia(int tamanho);
Bloco* criaRAM_aleatoria(int tamanho);
void liberaRAM(Bloco* ram);
void store(Bloco* ram, int pos, Bloco dado);
Bloco load(Bloco* ram, int pos);
void imprimeBloco(Bloco* bloco);
void imprimeRAM(Bloco* ram, int tam);

#endif