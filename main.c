#include <stdio.h>
#include <time.h>

#include "menu.h"
#include "structs.h"

void tela_opcoes_avancadas() {

    static ConfigItem configs[] = {
        {"Habilitar Firewall",  1, 0},
        {"Logs do Sistema",     0, 0},
        {"Modo Escuro",         1, 0},
        {"SALVAR E VOLTAR",     0, 1} 
    };
    int n = sizeof(configs) / sizeof(ConfigItem);
    menu_checkbox(configs, n, "CONFIGURACOES AVANCADAS");
}

int main() {
    srand(time(NULL));
    char *opcoes_principal[] = {
        "Iniciar Benchmark",
        "Configuracoes",
        "Tabela de Resultados",
        "Sair"
    };

    // INICIALIZAR RELOGIO EM ALGUM LUGAR


    
    menu_init();
    int rodando = 1;
    while(rodando) {
        int escolha = menu_run(opcoes_principal, 4, "CacheBenchmark™- v0.5");

        switch(escolha) {
            case 1: //Iniciar Benchmark
                menu_close();
                getchar();
                menu_init();
                break;
            case 2: //Configuracoes
                tela_opcoes_avancadas(); 
                break;
            case 3: //Tabela de Resultados
                menu_close();
                
            case -1: //Sair
            case 4:
                rodando = 0;
                break;
        }
    }

    menu_close();
    
    return 0;
}