#include "MMU.h"
#include "ram.h"
#include "structs.h"

#include "limits.h"
#include <stdlib.h>

//Custos hipoteticos conforme a hierarquia 
#define CUSTO_L1 10
#define CUSTO_L2 30
#define CUSTO_L3 100
#define CUSTO_RAM 600

#define ID_BUFFER 0
#define ID_LIP 1


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
            cache->memoria[i].alterado = false;
            cache->memoria[i].prioridade = 0;
            cache->memoria[i].endBloco = -1;
        }
        
    }
    return cache;
}

void destroiCache(Cache* c) {
    if (c) {
        if (c->memoria)
            free(c->memoria);
        free(c);
    }
}


void buffer_add(WriteBuffer *wb, int endBloco, LinhaCache linha, long *relogio);
void salvaRAM(Cache* L3, int indice, LinhaCache *RAM, WriteBuffer *buffer, int ConfigBuffer, long int *relogio);





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
    long int menorPrioridade = LONG_MAX;


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

int transfereCache(LinhaCache *RAM, WriteBuffer *buffer, ConfigItem *configs, Cache* cacheOrigem, Cache* cacheDestino, int indiceOrigem, int endRAM, long int *relogio, int configLIP) {
    LinhaCache dadoSobe = cacheOrigem->memoria[indiceOrigem];

    cacheOrigem->memoria[indiceOrigem].preenchido = false;

    int posDestino = buscaDadoAntigo(cacheDestino, endRAM);

    if (cacheDestino->memoria[posDestino].preenchido) {
        LinhaCache aux = cacheDestino->memoria[posDestino];

        int destinoParaOrigem = buscaDadoAntigo(cacheOrigem, aux.endBloco);

        salvaRAM(cacheOrigem, destinoParaOrigem, RAM, buffer, configs[ID_BUFFER].ativo, relogio);
        cacheOrigem->memoria[destinoParaOrigem] = aux;
        cacheOrigem->memoria[destinoParaOrigem].prioridade = *relogio;
        cacheOrigem->memoria[destinoParaOrigem].preenchido = true;
    }

    cacheDestino->memoria[posDestino] = dadoSobe;
    cacheDestino->memoria[posDestino].prioridade = (configLIP) ? 0 : *relogio;
    cacheDestino->memoria[posDestino].preenchido = true;

    return posDestino;
}

void buffer_add(WriteBuffer *wb, int endBloco, LinhaCache linha, long *relogio) {
    long delta = *relogio - wb->ultimoUso;
    if (delta > 0) {
        long qtdStores = delta / wb->custoPorStore;

        if (qtdStores > 0) {
            if (qtdStores > wb->qtdAtual)
                qtdStores = wb->qtdAtual;

            wb->qtdAtual -= qtdStores;
            wb->ultimoUso += (qtdStores * wb->custoPorStore);
        }
    }

    if (wb->qtdAtual == 0)
        wb->ultimoUso = *relogio;
        
    if (wb->qtdAtual > 0) {
        int i = wb->inicio;
        int count = 0;
        while (count < wb->qtdAtual) {
            if (wb->fila[i].endBloco == endBloco) {
                wb->fila[i].dado = linha;
                return;
            }
            i = (i + 1) % wb->tamMax;
            count++;
        }
    }
        



    while (wb->qtdAtual >= wb->tamMax) {
        *relogio += wb->custoPorStore;
        wb->ultimoUso = *relogio;
        
        wb->qtdAtual--;
        wb->inicio = (wb->inicio + 1) % wb->tamMax;
        wb->qtdStalls++;
    }

    wb->fila[wb->fim].endBloco = endBloco;
    wb->fila[wb->fim].dado = linha;

    wb->fim = (wb->fim + 1) % wb->tamMax;
    wb->qtdAtual++;
}

void salvaRAM(Cache* L3, int indice, LinhaCache *RAM, WriteBuffer *buffer, int ConfigBuffer, long int *relogio) { // Da cache l3 pra RAM
    if(!L3 || !RAM)
        return;
    
    
    if(L3->memoria[indice].preenchido && L3->memoria[indice].alterado)
    {
        int endRAM = L3->memoria[indice].endBloco;

        if (ConfigBuffer)
            buffer_add(buffer, endRAM, L3->memoria[indice], relogio);
        else {
            RAM[endRAM] = L3->memoria[indice];
            *relogio += CUSTO_RAM;
        }
        L3->memoria[indice].alterado = false;
    }      
}

int procuraBuffer(WriteBuffer *wb, int endBloco, LinhaCache *linha) {
    if (!wb || wb->qtdAtual == 0)
        return 0;

    int i = wb->inicio;
    int achou = 0;

    for (int cnt = 0; cnt < wb->qtdAtual; cnt++) {
        if (wb->fila[i].endBloco == endBloco) {
            *linha = wb->fila[i].dado;
            achou = 1;
            break;
        }
        i = (i + 1) % wb->tamMax;
    }
    return achou;
}

int carregaRAM(Cache* L3, int endRAM, LinhaCache *RAM, WriteBuffer *buffer, int ConfigBuffer, long int *relogio) { // Da RAM pra cache l3
    if(!RAM || !L3)
        return -1;

    int indiceAntigo = buscaDadoAntigo(L3, endRAM);//pega qual valor não está sendo usado para passar pra RAM
    
    salvaRAM(L3, indiceAntigo, RAM, buffer, ConfigBuffer, relogio);

    LinhaCache linhaNova;
    int achouBuffer = 0;

    if (ConfigBuffer)
        achouBuffer = procuraBuffer(buffer, endRAM, &linhaNova);
    
    if (achouBuffer)
        *relogio += CUSTO_L1;
    else {
        linhaNova = RAM[endRAM];
        *relogio += CUSTO_RAM;
    }

    L3->memoria[indiceAntigo] = linhaNova;
    L3->memoria[indiceAntigo].endBloco = endRAM;
    L3->memoria[indiceAntigo].preenchido = true;
    L3->memoria[indiceAntigo].alterado = false;
    L3->memoria[indiceAntigo].prioridade = *relogio;

    return indiceAntigo;
}

int moveL1(Endereco add, Cache *L1, Cache *L2, Cache *L3, LinhaCache *RAM, WriteBuffer *buffer, long int *relogio, ConfigItem *configs) {

    *relogio += CUSTO_L1;
    int pos = buscarCache(L1, add.endBloco, *relogio);
    if (pos != -1) 
        return pos;

    *relogio += CUSTO_L2;
    int posL2 = buscarCache(L2, add.endBloco, *relogio);
    if (posL2 != -1) 
        return transfereCache(RAM, buffer, configs, L2, L1, posL2, add.endBloco, relogio, configs[ID_LIP].ativo);

    *relogio += CUSTO_L3;
    int posL3 = buscarCache(L3, add.endBloco, *relogio);
    if (posL3 != -1) {
        int novoposL2 = transfereCache(RAM, buffer, configs, L3, L2, posL3, add.endBloco, relogio, 0);
        return transfereCache(RAM, buffer, configs, L2, L1, novoposL2, add.endBloco, relogio, configs[ID_LIP].ativo);
    }

    *relogio += CUSTO_RAM;
    int novoposL3 = carregaRAM(L3, add.endBloco, RAM, buffer, configs[ID_BUFFER].ativo, relogio);
    int novoposL2 = transfereCache(RAM, buffer, configs, L3, L2, novoposL3, add.endBloco, relogio, 0);
    return transfereCache(RAM, buffer, configs, L2, L1, novoposL2, add.endBloco, relogio, configs[ID_LIP].ativo);
}

LinhaCache MMU_Read(Endereco add, Cache *L1, Cache *L2, Cache *L3, LinhaCache *RAM, WriteBuffer *buffer, long int *relogio, ConfigItem *configs) {
    int posL1 = moveL1(add, L1, L2, L3, RAM, buffer, relogio, configs);

    if (posL1 != -1)
        return L1->memoria[posL1];
    else
        exit(-1);

}

void MMU_Write(Cache *L1, Cache *L2, Cache *L3, LinhaCache *RAM, WriteBuffer *buffer, Endereco add, int valor, long int *relogio, ConfigItem *configs) {
    int endpalavra = add.endPalavra;

    int posL1 = moveL1(add, L1, L2, L3, RAM, buffer, relogio, configs);
    if (posL1 != -1) {
        L1->memoria[posL1].palavras[endpalavra] = valor;
        L1->memoria[posL1].alterado = true;
        L1->memoria[posL1].preenchido = true;
        L1->memoria[posL1].prioridade = *relogio;
    }
    else
        exit(-1);
}

