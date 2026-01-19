#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "menu.h"
#include "utils.h"
#include "structs.h"
int a = 1;
void tela_configs(ConfigItem *configs, char* text, int n) {
    menu_checkbox(configs, n, text);
}

int main() {
    srand(time(NULL));
    char *opcoes_principal[] = {
        "Iniciar Benchmark",
        "Configuracoes",
        "Tabela de Resultados",
        "Sair"
    };

    ConfigItem configs[] = {
        {"WriteBuffer",                    0, 0},
        {"LRU Insertion Policy (LIP)",     1, 0},
        {"Modo Escuro",                    0, 0},
        {"SALVAR E VOLTAR",                0, 1} 
    };
    BenchMetrics metrics;   
    menu_init();
    int rodando = 1;
    while(rodando) {
        int escolha = menu_run(opcoes_principal, 4, "CacheBenchmark - v0.8");

        switch(escolha) {
            case 1: //Iniciar Benchmark
                setupBenchmark(&metrics, configs);
                CacheBenchmark(&metrics, configs);
                mostrar_relatorio(&metrics);
                break;
            case 2: //Configuracoes
                tela_configs(configs, "Configuracoes do Sistema", (sizeof(configs) / sizeof(ConfigItem))); 
                break;
            case 3: //Tabela de Resultados
                menu_close();
                break; 
            case -1: //Sair
            case 4:
                rodando = 0;
                break;
        }
    }

    menu_close();
    
    return 0;
}