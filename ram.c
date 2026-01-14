#include "ram.h"
#include "structs.h"
#include <stdlib.h>
#include <stdio.h>

Bloco* criaRAM_vazia(int tamanho) {
    Bloco *ptr = (Bloco*) malloc(sizeof(Bloco));
    if (ptr) {
        for (int i = 0; i < tamanho; i++) {
            ptr[i].endBloco = i;
            ptr[i].alterado = false;
            ptr[i].custo = 0;
            ptr[i].cacheHit = 0;
            for (int j = 0; j < 4; j++)
                ptr[i].palavras[j] = 0;
        }
    }
    return ptr;
}


Bloco* criaRAM_aleatoria(int tamanho) {
    Bloco *ptr = (Bloco*) malloc(sizeof(Bloco));
    if (ptr) {
        for (int i = 0; i < tamanho; i++) {
            ptr[i].endBloco = i;
            ptr[i].alterado = false;
            ptr[i].custo = 0;
            ptr[i].cacheHit = 0;
            for (int j = 0; j < 4; j++)
                ptr[i].palavras[j] = rand();
        }
    }
    return ptr;
}

void liberaRAM(Bloco* ram) {
    if (ram)
        free(ram);
}

void store(Bloco* ram, int pos, Bloco dado) {
    ram[pos] = dado;
}

Bloco load(Bloco* ram, int pos) {
    return ram[pos];
}



void imprimeBloco(Bloco* bloco) {
    printf("[");
    for (int i = 0; i < 4; i++) 
        printf("%d%s", bloco->palavras[i], (i < 3) ? ", " : "" );
    printf("] ");
}

void imprimeRAM(Bloco* ram, int tam) {
    printf("[ ");
    for (int i = 0; i < tam; i++) {
        imprimeBloco(ram[i].palavras);
        printf("%s", (i < tam - 1) ? ", " : "");
    }
    printf("]\n");
}