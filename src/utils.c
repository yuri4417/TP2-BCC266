#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "ram.h"
#include "menu.h"
#include "utils.h"
#include "structs.h"
#include "MMU.h"




Instrucao* gerarInstrucoes(int N_INST, int N_MEM, int N_PROB, int N_FOR, int N_OPCODE, int N_WORD) {

    Instrucao* programa = (Instrucao*) malloc((N_INST + 1) * sizeof(Instrucao));
    if (!programa)
     {
        exit(1);
    }

    Instrucao* padrao_repeticao = (Instrucao*) malloc(N_FOR * sizeof(Instrucao));
    for (int i = 0; i < N_FOR; i++) {
        padrao_repeticao[i].opcode = rand() % N_OPCODE;

        padrao_repeticao[i].add1.endBloco = rand() % N_MEM;
        padrao_repeticao[i].add1.endPalavra = rand() % N_WORD;

        padrao_repeticao[i].add2.endBloco = rand() % N_MEM;
        padrao_repeticao[i].add2.endPalavra = rand() % N_WORD;

        padrao_repeticao[i].add3.endBloco = rand() % N_MEM;
        padrao_repeticao[i].add3.endPalavra = rand() % N_WORD;
    }

    int i = 0;
    while (i < N_INST) {
        int random_prob = (rand() % 100) + 1;

        if (random_prob <= N_PROB && (i + N_FOR) <= N_INST) {
            for (int j = 0; j < N_FOR; j++) {
                programa[i] = padrao_repeticao[j];
                i++;
            }
        } 
        else {
            programa[i].opcode = rand() % N_OPCODE;
            
            programa[i].add1.endBloco = rand() % N_MEM;
            programa[i].add1.endPalavra = rand() % N_WORD;
            
            programa[i].add2.endBloco = rand() % N_MEM;
            programa[i].add2.endPalavra = rand() % N_WORD;
            
            programa[i].add3.endBloco = rand() % N_MEM;
            programa[i].add3.endPalavra = rand() % N_WORD;
            
            i++;
        }
    }

    free(padrao_repeticao);
    programa[N_INST].opcode = -1; // HALT
    return programa;
}

void setupBenchmark(BenchMetrics *metrics, ConfigItem *configs) {
    metrics->tamL1 = menu_valor("Tamanho da Cache L1 (em blocos)");
    metrics->tamL2 = menu_valor("Tamanho da Cache L2 (em blocos)");
    metrics->tamL3 = menu_valor("Tamanho da Cache L3 (em blocos)");
    if (configs[ID_BUFFER].ativo)
        metrics->tamWriteBuffer = menu_valor("Tamanho do WriteBuffer");

    metrics->relogio = 0;
    metrics->tamRAM = 1000;

    metrics->N_PROB = menu_valor("Probabilidade de Repeticao");
    metrics->N_FOR = menu_valor("Numero de Instrucoes na Repeticao");
}

void CacheBenchmark(BenchMetrics *metrics, ConfigItem *configs) {
    Cache *L1 = criaCache(metrics->tamL1); Cache *L2 = criaCache(metrics->tamL2); Cache *L3 = criaCache(metrics->tamL3);
    LinhaCache *RAM = criaRAM_aleatoria(metrics->tamRAM);
    Instrucao *programa = gerarInstrucoes(10000, metrics->tamRAM, metrics->N_PROB, metrics->N_FOR, 2, 4);


    WriteBuffer buffer;
    if (configs[ID_BUFFER].ativo) {
        buffer.fila = (ItemBuffer*) malloc(metrics->tamWriteBuffer * sizeof(ItemBuffer));
        buffer.inicio = 0;
        buffer.qtdStalls = 0;
        buffer.fim = 0;
        buffer.qtdAtual = 0;
        buffer.tamMax = metrics->tamWriteBuffer;
        buffer.ultimoUso = 0;
        buffer.custoPorStore = 600; 
    } 
    else
        metrics->tamWriteBuffer = -1;
    
    if (configs[ID_LIP].ativo) 
        strcpy(metrics->policy, "LIP");
    else if (configs[ID_RRIP].ativo)
        strcpy(metrics->policy, "RRIP");
    else
        strcpy(metrics->policy, "LRU");

    cpu(L1, L2, L3, RAM, programa, &metrics->relogio, &buffer, configs);


    metrics->hitsL1 = L1->hit; metrics->missesL1 = L1->miss;
    metrics->hitsL2 = L2->hit; metrics->missesL2 = L2->miss;
    metrics->hitsL3 = L3->hit; metrics->missesL3 = L3->miss;

    if (configs[ID_BUFFER].ativo) 
        metrics->qtdStalls = buffer.qtdStalls;
    else 
        metrics->qtdStalls = 0;

    destroiCache(L1); destroiCache(L2); destroiCache(L3); 
    liberaRAM(RAM);
    free(programa);
    if (configs[ID_BUFFER].ativo)
        free(buffer.fila);
}

