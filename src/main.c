#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "menu.h"
#include "utils.h"
#include "structs.h"
#include "cores.h"
#include "tabela.h"
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

void testeRapido(ConfigItem *configs) {
    BenchMetrics m;
    
    menu_close(); 


    m.tamL1 = 32; 
    m.tamL2 = 64;
    m.tamL3 = 256;
    m.tamRAM = TAM_RAM_DEFAULT;
    
    m.N_PROB = 90;    
    m.N_FOR = 5;  
    m.tamWriteBuffer = 4;

    configs[ID_MULT].ativo = 0; 

    printf("\n" BOLD(CYAN("=== Comparacão de Políticas  (L1=%d, MULTI=ON) ===")) "\n", m.tamL1);

    configs[ID_LIP].ativo = 0; 
    configs[ID_LFU].ativo = 0; 
    configs[ID_RRIP].ativo = 0;
    
    CacheBenchmark(&m, configs);
    printf("LRU  -> Hit L1: %5.1f%% (Tempo: %ld)\n", (float)m.hitsL1*100/(m.hitsL1+m.missesL1), m.relogio);

    configs[ID_LIP].ativo = 1; 
    configs[ID_LFU].ativo = 0; 
    configs[ID_RRIP].ativo = 0;
    
    CacheBenchmark(&m, configs);
    printf("LIP  -> Hit L1: %5.1f%% (Tempo: %ld)\n", (float)m.hitsL1*100/(m.hitsL1+m.missesL1), m.relogio);

    configs[ID_LIP].ativo = 0; 
    configs[ID_LFU].ativo = 1; 
    configs[ID_RRIP].ativo = 0;
    
    CacheBenchmark(&m, configs);
    printf("LFU  -> Hit L1: %5.1f%% (Tempo: %ld)\n", (float)m.hitsL1*100/(m.hitsL1+m.missesL1), m.relogio);

    configs[ID_LIP].ativo = 0; 
    configs[ID_LFU].ativo = 0; 
    configs[ID_RRIP].ativo = 1;
    
    CacheBenchmark(&m, configs);
    printf("RRIP -> Hit L1: %5.1f%% (Tempo: %ld)\n", (float)m.hitsL1*100/(m.hitsL1+m.missesL1), m.relogio);
    
    printf("\n" GREEN("Pressione ENTER para retornar ao menu...") "\n");
    getchar();
    menu_init(); 
}



int main() {
    srand(RAND);
    BenchMetrics tabela[50];
    ConfigItem configs[] = {
        {"WriteBuffer",                    0, 0},
        {"LRU Insertion Policy (LIP)",     1, 0},
        {"LFU Policy ",                    0, 0},
        {"RRIP Policy",                    0, 0},
        {"Gerador Multiplo",               0, 0},
        {"SALVAR E VOLTAR",                0, 1} 
    };

    menu_init();
    int rodando = 1;
    char *opcoes_principal[] = {"Benchmark Unico", "Configuracoes", "Tabelas", "Sair"};
    int qtdSalva = 0;
    while(rodando) {
        system("clear");
        int escolha = menu_run(opcoes_principal, 4, "CacheBenchmark - BCC 266");

        switch(escolha) {
            case 1: // Manual
                // BenchMetrics m;
                // setupBenchmark(&m, configs);
                // CacheBenchmark(&m, configs);
                // exibirRelatorioIndividual(&m,configs);
                // salvaTabela(&qtdSalva, tabela, m);
                testeRapido(configs);
                break;
            case 2: // configs
                menu_checkbox(configs, 6, "Configuracoes");
                break;
            case 3: // Bancada Padrão
                char *opcoesSubMenu []= {"Tabelas Locais", "Tabelas Padrao (M1 - M5)"};
                int opcao = menu_run(opcoesSubMenu, 2, "Opcoes de tabelas");
                switch(opcao)
                {
                    case 1:
                        imprimirTabelaSalva(tabela, qtdSalva);
                        break; 
                    case 2:
                        testePadrao(configs);
                }
                break;
            case 4:
                rodando = 0;
                break;
        }
    }

    menu_close();
    system("clear");
    return 0;
}
