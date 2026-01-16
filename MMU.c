#include "MMU.h"
#include "ram.h"
#include <stdlib.h>
#include "structs.h"

//Custos hipoteticos conforme a hierarquia 
#define CUSTO_C1 10
#define CUSTO_C2 110
#define CUSTO_C3 1110
#define CUSTO_RAM 11110

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
            cache->memoria[i].endBloco = -1;
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

int buscarCache(Cache* c, int endRAM, long int relogioAtual) {
    int posCache = endRAM % c->nroConjuntos;

    int inicioBloco = posCache * 4;
    for (int i = inicioBloco; i < inicioBloco + 4; i++) {
        if (c->memoria[i].preenchido && c->memoria[i].endBloco == endRAM) {
            c->hit++;
            c->memoria[i].prioridade = relogioAtual;
            return i;
        }
    }

    
    c->miss++;
    return -1;
}



int buscaDadoAntigo(Cache* c, int endRAM) {
    int posMenorPrioridade = -1;
    int menorPrioridade = __INT_MAX__;


    int posConjunto = endRAM % c->nroConjuntos;
    int inicioBloco = posConjunto * 4;

    for (int i = inicioBloco; i < inicioBloco + 4; i++) {
        if (!c->memoria[i].preenchido) 
            return i;
        
        if (c->memoria[i].prioridade < menorPrioridade) {
            menorPrioridade = c->memoria[i].prioridade;
            posMenorPrioridade = i;
        }
    }

    return posMenorPrioridade;
}

int transfereCache(Cache* cacheOrigem, Cache* cacheDestino, int indiceOrigem, int endRAM, long int relogioAtual) {
    LinhaCache dadoSobe = cacheOrigem->memoria[indiceOrigem];

    cacheOrigem->memoria[indiceOrigem].preenchido = false;

    int posDestino = buscaDadoAntigo(cacheDestino, endRAM);

    if (cacheDestino->memoria[posDestino].preenchido) {
        LinhaCache aux = cacheDestino->memoria[posDestino];

        int destinoParaOrigem = buscaDadoAntigo(cacheOrigem, aux.endBloco);

        cacheOrigem->memoria[destinoParaOrigem] = aux;
        cacheOrigem->memoria[destinoParaOrigem].prioridade = relogioAtual;
        cacheOrigem->memoria[destinoParaOrigem].preenchido = true;
    }

    cacheDestino->memoria[posDestino] = dadoSobe;
    cacheDestino->memoria[posDestino].prioridade = relogioAtual;
    cacheDestino->memoria[posDestino].preenchido = true;

    return posDestino;
}

int carregaRAM(Cache* L3, int endRAM, LinhaCache *RAM) { // Da RAM pra cache l3
    if(!RAM || !L3)
        return -1;
    int indiceAntigo = buscaDadoAntigo(L3, endRAM);//pega qual valor não está sendo usado para passar pra RAM
    
    if(L3->memoria[indiceAntigo].preenchido)   
        salvaRAM(L3, endRAM, RAM);
    
    L3->memoria[indiceAntigo] = RAM[endRAM];

    L3->memoria[indiceAntigo].endBloco = endRAM;
    L3->memoria[indiceAntigo].preenchido = true;
    // observar qual valor inicializar na prioridade

    return indiceAntigo;
}


void salvaRAM(Cache* L3, int endRAM, LinhaCache *RAM) { // Da cache l3 pra RAM
    if(!L3 || !RAM)
        return;
    
    int indiceAntigo = buscaDadoAntigo(L3,endRAM); // vai pegar o indice  do dado mais antigo que a função retornar
    
    if(L3->memoria[indiceAntigo].preenchido)
    {
        int enderecoAntigo = L3->memoria[indiceAntigo].endBloco;
    
        RAM[enderecoAntigo] = L3->memoria[indiceAntigo];
        
        L3->memoria[indiceAntigo].preenchido = false;

    }
        
}

LinhaCache MMU(Endereco add, Cache *L1, Cache *L2, Cache *L3, LinhaCache *RAM, long int *relogio) {

    (*relogio)++;

    int posL1 = buscarCache(L1, add.endBloco, *relogio);
    if (posL1 != -1) {
        L1->hit++;
        return L1->memoria[posL1];
    }
    L1->miss++;

    int posL2 = buscarCache(L2, add.endBloco, *relogio);
    if (posL2 != -1) {
        L2->hit++;
        int novaL1 = transfereCache(L2, L1, posL2, add.endBloco, *relogio);
        return L1->memoria[novaL1];
    }
    L2->miss++;

    int posL3 = buscarCache(L3, add.endBloco, *relogio);
    if (posL3 != -1) {
        L3->hit++;
        int destL2 = transfereCache(L3, L2, posL3, add.endBloco, *relogio);
        
        int novaL1 = transfereCache(L2, L1, destL2, add.endBloco, *relogio);
        return L1->memoria[novaL1];
    }
    L3->miss++;

    // Dado não está em nenhuma cache, buscar na RAM
    // carregaRAM(L3, add.endBloco);
    // transfereCache(L3, L2, add.endBloco, 0);
    // transfereCache(L2, L1, add.endBloco, 0);
    // int posFinalL1 = buscarCache(L1, add.endBloco, *relogio);
    // if (posFinalL1 == -1) 
    //     exit(-1);
    
    // return L1->memoria[posFinalL1];

}

//Vou completar isso aqui
// int UCM