#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "menu.h"
#include "utils.h"
#include "structs.h"
#include "cores.h"
#include "tabela.h"
void tela_configs(ConfigItem *configs, char* text, int n) {
    menu_checkbox(configs, n, text);
}
// Função para rodar a bateria de testes 
/*void testePadrao(ConfigItem *configs) {
    // Dados da tabela 
    int m1[3] = {8, 16, 32};
    int m2[3] = {32, 64, 128};
    int m3[3] = {16, 64, 256};
    int m4[3] = {8, 32, 128};
    int m5[3] = {16, 32, 64};
    int *maquinas[5] = {m1, m2, m3, m4, m5};

    printf("\n" BOLD(CYAN("=== EXECUTANDO BATERIA DE TESTES (M1 - M5) ===")) "\n");
    
    for (int i = 0; i < 5; i++) {
        BenchMetrics m;
        m.tamL1 = maquinas[i][0];
        m.tamL2 = maquinas[i][1];
        m.tamL3 = maquinas[i][2];
        m.tamRAM = 1000;
        m.N_PROB = 75; // Valor médio sugerido 
        m.N_FOR = 10;
        m.relogio = 0;
        m.tamWriteBuffer = configs[0].ativo ? 10 : -1;

        printf(YELLOW("Rodando Maquina %d...") " ", i + 1);
        fflush(stdout);

        CacheBenchmark(&m, configs); // Executa a simulação
        
        printf(GREEN("Concluido!") " Tempo: %ld ciclos\n", m.relogio);
        
        // Exibe o relatório individual 
        mostrar_relatorio(&m); 
    }
}
*/
int main() {
    srand(time(NULL));
    
    BenchMetrics tabela[50];
    ConfigItem configs[] = {
        {"WriteBuffer",                    0, 0},
        {"LRU Insertion Policy (LIP)",     1, 0},
        {"Modo Escuro",                    0, 0},
        {"SALVAR E VOLTAR",                0, 1} 
    };

    menu_init();
    int rodando = 1;
    char *opcoes_principal[] = {"Iniciar Benchmark Manual", "Opções de tabelas", "Configuracoes", "Sair"};
    int qtdSalva = 0;
    while(rodando) {
        system("clear");
        int escolha = menu_run(opcoes_principal, 4, "CacheBenchmark - BCC 266");

        switch(escolha) {
            case 1: // Manual
                BenchMetrics m;
                setupBenchmark(&m, configs);
                CacheBenchmark(&m, configs);
                exibirRelatorioIndividual(&m,configs);
                salvaTabela(&qtdSalva, tabela, m);
                //mostrar_relatorio(&m);
                break;
                
            case 2: // M1 - M5 automático
                char *opcoesSubMenu []= {"Iniciar tabelas salvas", "Tabelas padrões"};
                int opcao = menu_run(opcoesSubMenu, 2, "Opções de tabelas");
                switch(opcao)
                {
                    case 1:
                        imprimirTabelaSalva(tabela, qtdSalva);
                        break; 
                    case 2:
                        testePadrao(configs);
                }
                
                break;
            case 3: // configs
                menu_checkbox(configs, 4, "Configuracoes");
                break;
            case 4:
                rodando = 0;
                break;
        }
    }

    menu_close();
    return 0;
}
