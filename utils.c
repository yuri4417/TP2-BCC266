#include <stdio.h>
#include "utils.h"
#include "structs.h"

Instrucao* gerarInstrucoes(int N_INST, int N_MEM, int N_PROB, int N_FOR, int N_OPCODE, int N_WORD) {

    Instrucao* programa = (Instrucao*) malloc(N_INST * sizeof(Instrucao));
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

    return programa;
}