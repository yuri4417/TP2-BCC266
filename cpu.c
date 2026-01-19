#include "structs.h"
#include "cpu.h"
#include "ram.h"

#include "MMU.h"

void cpu(Cache *L1, Cache *L2, Cache *L3, LinhaCache *RAM, Instrucao *programa, long int *relogio, WriteBuffer *buffer, ConfigItem *configs) {
    LinhaCache reg1, reg2, reg3;
    int PC = 0; 
    int opcode = programa[PC].opcode;
    while (opcode != -1) {
        Instrucao atual = programa[PC];
        opcode = atual.opcode;

        if (opcode == -1)
            break;

        (*relogio)++;
        reg2 = MMU_Read(atual.add2, L1, L2, L3, RAM, buffer, relogio, configs);
        reg1 = MMU_Read(atual.add1, L1, L2, L3, RAM, buffer, relogio, configs);
        switch (opcode) {
            // case -1:
            //     break;
            case 0:
                reg3.palavras[atual.add3.endPalavra] = reg1.palavras[atual.add1.endPalavra] + reg2.palavras[atual.add2.endPalavra];
                break;
            case 1:
                reg3.palavras[atual.add3.endPalavra] = reg1.palavras[atual.add1.endPalavra] - reg2.palavras[atual.add2.endPalavra];
                break;
        }

        MMU_Write(L1, L2, L3, RAM, buffer, atual.add3, reg3.palavras[atual.add3.endPalavra], relogio, configs);
        PC++;

    }

}