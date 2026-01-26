#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h> 

#include "tabela.h"
#include "utils.h"   
#include "cores.h"   
#include "structs.h" 

#define H3  TAB_HOR TAB_HOR TAB_HOR 
#define H4  TAB_HOR TAB_HOR TAB_HOR TAB_HOR
#define H8  TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR
#define H12 TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR
#define H_RESUMO H12 H12 H12 H12 H12

void exibirInfoGeral(BenchMetrics *m, ConfigItem *configs) {
    long totalAcessos = m->hitsL1 + m->missesL1;
    
    printf("\n");
    printf(TAB_TL H_RESUMO TAB_TR "\n");
    printf(TAB_VER " Resumo da execucao:%41s" TAB_VER "\n", "");
    printf(TAB_ML H_RESUMO TAB_MR "\n");
    printf(TAB_VER " %-18s%-9d KB %29s" TAB_VER "\n", "RAM Total:", m->tamRAM, "");
    printf(TAB_VER " %-18s", "Write Buffer:");
    if (configs[0].ativo) {
        printf("%s%34s" TAB_VER "\n", GREEN("ATIVADO"), "");
    } else {
        printf("%s%31s" TAB_VER "\n", RED("DESATIVADO"), "");
    }
    printf(TAB_VER " %-18s%-3d %% %36s" TAB_VER "\n", "Prob. Repeticao:", m->N_PROB, "");
    printf(TAB_VER " %-18s%-3d %38s" TAB_VER "\n", "Instr. p/ Loop:", m->N_FOR, "");
    printf(TAB_VER " %-18s%-9ld %32s" TAB_VER "\n", "Total Instrucoes:", totalAcessos, "");
    printf(TAB_VER " %-18s%-9ld %32s" TAB_VER "\n", "Total Stalls:", m->qtdStalls, "");

    printf(TAB_BL H_RESUMO TAB_BR "\n\n");
}

void cabecalho() {
    printf(TAB_TL H3 TAB_TJ H4 TAB_TJ H4 TAB_TJ H4); 
    printf(TAB_TJ H8 TAB_TJ H8 TAB_TJ H8); 
    printf(TAB_TJ H8 TAB_TJ H8 TAB_TJ H8); 
    printf(TAB_TJ H8 TAB_TJ H12 TAB_TR "\n");

    printf(TAB_VER " M " TAB_VER " L1 " TAB_VER " L2 " TAB_VER " L3 ");
    printf(TAB_VER "  H.L1  " TAB_VER "  M.L1  "); 
    printf(TAB_VER "  H.L2  " TAB_VER "  M.L2  ");
    printf(TAB_VER "  H.L3  " TAB_VER "  M.L3  ");
    printf(TAB_VER "  RAM%%  " TAB_VER "    TEMPO   " TAB_VER "\n");

    //linha pra dividir dos resultados
    printf(TAB_ML H3 TAB_MJ H4 TAB_MJ H4 TAB_MJ H4); 
    printf(TAB_MJ H8 TAB_MJ H8 TAB_MJ H8); 
    printf(TAB_MJ H8 TAB_MJ H8 TAB_MJ H8); 
    printf(TAB_MJ H8 TAB_MJ H12 TAB_MR "\n");
}

void imprimirLinha(int id, BenchMetrics *m) {
    long totalAcessos = m->hitsL1 + m->missesL1;
    float taxaRAM = totalAcessos ? ((float)m->missesL3 / totalAcessos) * 100.0 : 0.0;

    printf(TAB_VER " M%-d" TAB_VER " %-2d " TAB_VER " %-2d " TAB_VER " %-2d ",id, m->tamL1, m->tamL2, m->tamL3);
    printf(TAB_VER " %6ld " TAB_VER " %6ld " ,m->hitsL1, m->missesL1);
    printf(TAB_VER " %6ld " TAB_VER " %6ld ",m->hitsL2, m->missesL2);
    printf(TAB_VER " %6ld " TAB_VER " %6ld ",m->hitsL3, m->missesL3);
    printf(TAB_VER " %5.1f%% " TAB_VER " %10ld " TAB_VER "\n",taxaRAM, m->relogio);
}

void rodape() {
    printf(TAB_BL H3 TAB_BJ H4 TAB_BJ H4 TAB_BJ H4); 
    printf(TAB_BJ H8 TAB_BJ H8 TAB_BJ H8); 
    printf(TAB_BJ H8 TAB_BJ H8 TAB_BJ H8); 
    printf(TAB_BJ H8 TAB_BJ H12 TAB_BR "\n");
}

void rodarBateriaOficial(ConfigItem *configs) {
    endwin();
    int probEscolhida = selecionarProbabilidade();
    int nForEscolhido = selecionarNFor();
    system("clear"); 

    int maquinas[5][3] = { {8,16,32}, {32,64,128}, {16,64,256}, {8,32,128}, {16,32,64} };

    printf("\n" BOLD(CYAN("BATERIA OFICIAL (Resultados Detalhados")) "\n");

    // Instância para o Resumo
    BenchMetrics resumo;
    resumo.tamRAM = 1000; resumo.N_PROB = probEscolhida; resumo.N_FOR = nForEscolhido;
    resumo.hitsL1 = 0; resumo.missesL1 = 30000; // Valores ilustrativos para o topo
    resumo.qtdStalls = 0;
    inicializarMetricas(&resumo);
    exibirInfoGeral(&resumo, configs);

    cabecalho();

    for (int i = 0; i < 5; i++) {
        BenchMetrics m;
        m.tamL1 = maquinas[i][0]; m.tamL2 = maquinas[i][1]; m.tamL3 = maquinas[i][2];
        m.tamRAM = 1000; m.N_PROB = probEscolhida; m.N_FOR = nForEscolhido;
        m.tamWriteBuffer = configs[0].ativo ? 10 : -1;

        CacheBenchmark(&m, configs);
        imprimirLinha(i + 1, &m);

        if (i < 4) { // Linha divisória entre máquinas
            printf(TAB_ML H3 TAB_MJ H4 TAB_MJ H4 TAB_MJ H4);
            printf(TAB_MJ H8 TAB_MJ H8 TAB_MJ H8); 
            printf(TAB_MJ H8 TAB_MJ H8 TAB_MJ H8); 
            printf(TAB_MJ H8 TAB_MJ H12 TAB_MR "\n");
        }
    }

    rodape();
    printf("\n" GREEN("Concluido! Pressione ENTER para sair...") "\n");
    getchar(); getchar();
    refresh();
}
void exibirRelatorioIndividual(BenchMetrics *m, ConfigItem *configs) {
    endwin();
    system("clear");
    exibirInfoGeral(m, configs);
    cabecalho();
    imprimirLinha(1, m);
    rodape();
    printf("\n");
}

int selecionarProbabilidade() {
    system("clear");
    printf("Escolha a Probabilidade de Repeticao:\n");
    printf("1 - 50%%\n");
    printf("2 - 75%%\n");
    printf("3 - 90%% \n");
    printf("\nEscolha: ");
    
    int op;
    if (scanf("%d", &op) != 1) return 50; 

    switch(op) {
        case 1: return 50;
        case 2: return 75;
        case 3: return 90;
        default: return 50;
    }
}

int selecionarNFor() {
    system("clear");
    printf("\n" BOLD(YELLOW("Tamanho do for ")) "\n\n");  
    
    printf("1 - 4 inst.\n");
    printf("2 - 10 inst.\n");
    printf("3 - Grande  32 inst.\n");
    printf("4 - Personalizado\n");
    printf("\nEscolha: ");
    
    int op;
    if (scanf("%d", &op) != 1) return 10; 

    switch(op) {
        case 1: return 4;
        case 2: return 10;
        case 3: return 32;
        case 4: {
            printf("\nDigite o tamanho do loop desejado: ");
            int val;
            scanf("%d", &val);
            return (val > 0) ? val : 10;
        }
        default: return 10;
    }
}

void inicializarMetricas(BenchMetrics *m) {
    m->hitsL1 = 0; m->missesL1 = 0;
    m->hitsL2 = 0; m->missesL2 = 0;
    m->hitsL3 = 0; m->missesL3 = 0;
    m->qtdStalls = 0;
    m->relogio = 0;
    m->N_PROB = 0; 
    m->N_FOR = 0;
    m->tamRAM = 0;
}


void imprimirTabelaSalva(BenchMetrics *lista, int qtd) {
    endwin();
    system("clear");

    printf("\n" BOLD(MAGENTA("Tabela de resultados salvos")) "\n\n");
    if (qtd == 0) {
        printf(RED("Nenhum resultado salvo.") "\n");
    } else {
        cabecalho();
        for (int i = 0; i < qtd; i++) {
            imprimirLinha(i + 1, &lista[i]);

            if (i < qtd - 1) {
               printf(TAB_ML H3 TAB_MJ H4 TAB_MJ H4 TAB_MJ H4);
                printf(TAB_MJ H8 TAB_MJ H8); 
                printf(TAB_MJ H8 TAB_MJ H8); 
                printf(TAB_MJ H8 TAB_MJ H8); 
                printf(TAB_MJ H8 TAB_MJ H12 TAB_MR "\n");
            }
        }
        rodape();
    }

    printf("\n" GREEN("Pressione ENTER para voltar ao menu...") "\n");
    getchar();
    refresh();
}