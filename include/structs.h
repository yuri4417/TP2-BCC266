
#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>

//Custos hipoteticos conforme a hierarquia 
#define CUSTO_L1 10
#define CUSTO_L2 30
#define CUSTO_L3 100
#define CUSTO_RAM 600

#define POL_LRU 0
#define POL_LIP 1
#define POL_RRIP 2



#define ID_BUFFER 0
#define ID_LIP 1
#define ID_RRIP 2




typedef struct { 
    long int relogio;
    long qtdStalls;
    int tamL1, tamL2, tamL3, tamRAM, tamWriteBuffer;
    int hitsL1, missesL1;
    int hitsL2, missesL2;
    int hitsL3, missesL3;

    char policy[5];

    int N_PROB;
    int N_FOR;
} BenchMetrics;

typedef struct {
    char *texto;
    int ativo;
    int flagBotao;    // 0 = Checkbox, 1 = Botão
} ConfigItem;

typedef struct {
    int palavras[4];
    int endBloco;

    long int prioridade;
    bool preenchido;
    bool alterado;

} LinhaCache;

typedef struct {
    LinhaCache *memoria;
    int nroConjuntos;

    long int hit;
    long int miss;
} Cache;


typedef struct {
    int endBloco;
    int endPalavra;
} Endereco;

typedef struct {
    int opcode;
    Endereco add1;
    Endereco add2;
    Endereco add3;
} Instrucao;

typedef struct {
    int endBloco;
    LinhaCache dado;
} ItemBuffer;

typedef struct {
    ItemBuffer *fila;
    int tamMax;
    int qtdAtual;

    int inicio, fim;

    long int ultimoUso;
    int custoPorStore;

    long qtdStalls;
} WriteBuffer;


#endif
