#include "MMU.h"

#include <stdlib.h>
#include "structs.h"

//Custos hipoteticos conforme a hierarquia 
#define CUSTO_C1 1
#define CUSTO_C2 10
#define CUSTO_C3 1000
#define CUSTO_C4 10000

// contador de custos hipoteticos pra gerar a tabela de resultado final
long int tempoTotalSimulacao = 0;

Cache* criaCache(int tamanhoTotal) {
    Cache *cache = (Cache*) malloc(sizeof(Cache));
    if (cache) {
        cache->memoria = (LinhaCache*) malloc(sizeof(LinhaCache) * tamanhoTotal);
        if (!cache->memoria) {
            free(cache);
            return NULL; 
        }

        cache->nroConjuntos = tamanhoTotal / 4;
        cache->hit = 0;
        cache->miss = 0;

        for (int i = 0; i < tamanhoTotal; i++) {
            cache->memoria[i].preenchido = false;
            cache->memoria[i].prioridade = 0;
        }
    }
    return cache;
}

void destroiCache(Cache* c) {
    if (c)
        if (c->memoria)
            free(c->memoria);
        free(c);
}

int buscarCache(Cache* c, int endRAM) {
    int posCache = endRAM % c->nroConjuntos;

    int inicioBloco = posCache * 4;
    for (int i = inicioBloco; i < inicioBloco + 4; i++) {
        if (c->memoria[i].preenchido && c->memoria[i].endBloco == endRAM) {
            c->hit++;
            // implementar promoção de prioridade aqui
            return i;
        }
    }
    c->miss++;
    return -1;
}

void promoverBlocoParaCache(Cache *c,LinhaCache b ){

    int posConjunto = b.endBloco % c-> nroConjuntos;
    int iniciaNoVetor = posConjunto * 4;
//sobrescrever a primeira posição do conjunto com o novo bloco
    c->memoria[iniciaNoVetor].preenchido = true;
    c->memoria[iniciaNoVetor].endBloco = b.endBloco;
    
    for(int i = 0; i < 4; i++){
        c->memoria[iniciaNoVetor].palavras[i] = b.palavras[i];
    }

}
//Vou completar isso aqui
int UCM