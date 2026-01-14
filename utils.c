#include <stdio.h>
#include "utils.h"
#include "structs.h"

Instrucao* gerarInstrucoes(int N_INST, int N_MEM, int N_PROB, int N_FOR, int N_OPCODE, int N_WORD) {

    Instrucao* programa = (Instrucao*) malloc(N_INST * sizeof(Instrucao));
    if (programa == NULL) {
        exit(1);
    }

    int **rep = (int**) malloc(N_FOR * sizeof(int*));
    for(int i=0; i<N_FOR; i++)
        rep[i] = (int*) malloc(7 * sizeof(int));

    int tamanho_instrucao = 6;

    for (int i = 0; i < N_FOR; i++) {
        int random = rand() % N_OPCODE;
        rep[i][0] = random;

        for (int j = 1; j <= tamanho_instrucao; j += 2) {
            rep[i][j] = rand() % N_MEM;
            rep[i][j + 1] = rand() % N_WORD;
        }
    }

    int i = 0;
    while (i < N_INST) {
        int random_prob = (rand() % 100) + 1;

        if (random_prob <= N_PROB && (i + N_FOR) <= N_INST) {
            for (int j = 0; j < N_FOR; j++) {
                programa[i].opcode       = rep[j][0];
                programa[i].add1_bloco   = rep[j][1];
                programa[i].add1_palavra = rep[j][2];
                programa[i].add2_bloco   = rep[j][3];
                programa[i].add2_palavra = rep[j][4];
                programa[i].add3_bloco   = rep[j][5];
                programa[i].add3_palavra = rep[j][6];
                i++;
            }
        } else {
            int temp[7];
            int random_op = rand() % N_OPCODE;
            temp[0] = random_op;

            for (int j = 1; j <= tamanho_instrucao; j += 2) {
                temp[j] = rand() % N_MEM;
                temp[j + 1] = rand() % N_WORD;
            }

            programa[i].opcode       = temp[0];
            programa[i].add1_bloco   = temp[1];
            programa[i].add1_palavra = temp[2];
            programa[i].add2_bloco   = temp[3];
            programa[i].add2_palavra = temp[4];
            programa[i].add3_bloco   = temp[5];
            programa[i].add3_palavra = temp[6];
            i++;
        }
    }

    for(int k=0; k<N_FOR; k++) free(rep[k]);
    free(rep);

    return programa;
}
