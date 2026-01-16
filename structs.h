
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

#endif
