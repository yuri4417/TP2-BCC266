#ifndef TABELA_H
#define TABELA_H

#include "structs.h"
 
void cabecalho();
void imprimirLinha(int idMaquina, BenchMetrics *m);
void rodape();

//fuções para imprimir a tabela
void rodarBateriaOficial(ConfigItem *configs);               // Fixa (M1 - M5)

void imprimirTabelaSalva(BenchMetrics *lista, int qtd);       // Variável (M1 - Mn)

void exibirRelatorioIndividual(BenchMetrics *m, ConfigItem *configs);
#endif