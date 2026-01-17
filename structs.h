
#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>

typedef struct {
    long int relogio;
    int tamRAM;
    int tamWriteBuffer;

    
    int tamL1;
    int hitsL1;
    int missesL1;

    int tamL2;
    int hitsL2;
    int missesL2;

    int tamL3;
    int hitsL3;
    int missesL3;

    int N_INST;
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

    // int cacheHit;
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
    Endereco add;
    int valor;
} ItemBuffer;

typedef struct {
    ItemBuffer *fila;
    int tamMax;
    int qtdAtual;

    int inicio, fim;

    long int ultimoUso;
    int custoPorStore;

} WriteBuffer;


#endif
