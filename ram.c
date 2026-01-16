#include "ram.h"
#include "structs.h"
#include <stdlib.h>
#include <stdio.h>


LinhaCache* criaRAM_vazia(int tamanho) {
    LinhaCache *ptr = (LinhaCache*) malloc(sizeof(LinhaCache) * tamanho);
    if (ptr) {
        for (int i = 0; i < tamanho; i++) {

            for (int j = 0; j < 4; j++)
                ptr[i].palavras[j] = 0;
            ptr[i].endBloco = i;
        }
    }
    return ptr;
}


LinhaCache* criaRAM_aleatoria(int tamanho) {
    LinhaCache *ptr = (LinhaCache*) malloc(sizeof(LinhaCache));
    if (ptr) {
        for (int i = 0; i < tamanho; i++) {
            ptr[i].endBloco = i;
            for (int j = 0; j < 4; j++)
                ptr[i].palavras[j] = rand();
        }
    }
    return ptr;
}

void liberaRAM(LinhaCache* ram) {
    if (ram)
        free(ram);
}

void store(LinhaCache* ram, LinhaCache dado, int nroBloco) {
    ram[nroBloco] = dado;
}

LinhaCache load(LinhaCache* ram, int endRAM) {
    return ram[endRAM];
}



void imprimeLinhaCache(LinhaCache* LinhaCache) {
    printf("[");
    for (int i = 0; i < 4; i++) 
        printf("%d%s", LinhaCache->palavras[i], (i < 3) ? ", " : "" );
    printf("] ");
}

void imprimeRAM(LinhaCache* ram, int tam) {
    printf("[ ");
    for (int i = 0; i < tam; i++) {
        imprimeLinhaCache(ram[i].palavras);
        printf("%s", (i < tam - 1) ? ", " : "");
    }
    printf("]\n");
}