#include "structs.h"
#include "cpu.h"
#include "ram.h"

#include "MMU.h"

void cpu(Instrucao *programa, long int relogio) {
    LinhaCache reg1, reg2, reg3;
    int PC = 0; 
    int opcode = programa[0].opcode;

    while (opcode != -1) {

        Instrucao atual = programa[PC];
        opcode = atual.opcode;

        switch (opcode) {
            case 0:

        }


        PC++;

    }

}