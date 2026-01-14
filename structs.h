#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>


typedef struct {
    char *texto;
    int ativo;
    int flagBotao;    // 0 = Checkbox, 1 = Botão
} ConfigItem;

typedef struct {
    int palavras[4];
    int endBloco;

    int prioridade;
    bool preenchido;

    // bool alterado;
    // int cacheHit;
} LinhaCache;

typedef struct {
    LinhaCache *memoria;
    int nroConjuntos;

    long int hit;
    long int miss;
} Cache;


typedef struct {
    int opcode;
    int add1_bloco;
    int add1_palavra;
    int add2_bloco;
    int add2_palavra;
    int add3_bloco;
    int add3_palavra;
} Instrucao;

#endif
