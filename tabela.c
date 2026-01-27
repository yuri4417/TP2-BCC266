#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h> 
#include <ctype.h>

#include "tabela.h"
#include "utils.h"   
#include "cores.h"   
#include "structs.h" 

//criando macros para evitar repetição
#define H3  TAB_HOR TAB_HOR TAB_HOR 
#define H6  TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR
#define H12 TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR
#define H20 TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR

//120 caracteres
#define H120 H12 H12 H12 H12 H12 H12 H12 H12 H12 H12


int selecionarProbabilidade();
int selecionarNFor();
void inicializarMetricas(BenchMetrics *m);

void exibirInfoGeral(BenchMetrics *m, ConfigItem *configs) {
    
    printf("\n");
    printf(TAB_TL H120 TAB_TR "\n");
    printf(TAB_VER " Resumo da execucao:%100s" TAB_VER "\n", ""); 
    printf(TAB_ML H120 TAB_MR "\n");
    printf(TAB_VER " %-18s%-10d KB %87s" TAB_VER "\n", "RAM Total:", m->tamRAM, "");
    
    if (configs[0].ativo) {
        printf(TAB_VER " %-18s%s%94s" TAB_VER "\n", "Write Buffer:", GREEN("ATIVADO"), "");
    } else {
        printf(TAB_VER " %-18s%s%91s" TAB_VER "\n", "Write Buffer:", RED("DESATIVADO"), "");
    }
    
    if (configs[1].ativo) {
        printf(TAB_VER " %-18s%s%94s" TAB_VER "\n", "Politica LIP:", GREEN("ATIVADO"), "");
    } else {
        printf(TAB_VER " %-18s%s%91s" TAB_VER "\n", "Politica LIP:", RED("DESATIVADO"), "");
    }

    printf(TAB_VER " %-18s%-3d %% %95s" TAB_VER "\n", "Prob. Repeticao:", m->N_PROB, "");
    printf(TAB_VER " %-18s%-10d%91s" TAB_VER "\n", "Instr. p/ Loop:", m->N_FOR, "");
    printf(TAB_VER " %-18s%-10ld%91s" TAB_VER "\n", "Total Stalls:", m->qtdStalls, "");
    printf(TAB_BL H120 TAB_BR "\n\n");
}
void cabecalho() {
    printf(TAB_TL H3 TAB_TJ H6 TAB_TJ H6 TAB_TJ H6);
    printf(TAB_TJ H12 TAB_TJ H12 TAB_TJ H12);   // L1
    printf(TAB_TJ H12 TAB_TJ H12 TAB_TJ H12);   // L2
    printf(TAB_TJ H12 TAB_TJ H12 TAB_TJ H12);   // L3
    printf(TAB_TJ H12 TAB_TJ H20 TAB_TR "\n");

    printf(TAB_VER " M " TAB_VER "  L1  " TAB_VER "  L2  " TAB_VER "  L3  ");
    printf(TAB_VER "    H.L1    " TAB_VER "   Hit L1%%  " TAB_VER "    M.L1    ");
    printf(TAB_VER "    H.L2    " TAB_VER "   Hit L2%%  " TAB_VER "    M.L2    ");
    printf(TAB_VER "    H.L3    " TAB_VER "   Hit L3%%  " TAB_VER "    M.L3    ");
    printf(TAB_VER "    RAM%%    " TAB_VER "        TEMPO       " TAB_VER "\n");

    printf(TAB_ML H3 TAB_MJ H6 TAB_MJ H6 TAB_MJ H6);
    printf(TAB_MJ H12 TAB_MJ H12 TAB_MJ H12);
    printf(TAB_MJ H12 TAB_MJ H12 TAB_MJ H12);
    printf(TAB_MJ H12 TAB_MJ H12 TAB_MJ H12);
    printf(TAB_MJ H12 TAB_MJ H20 TAB_MR "\n");
}

void imprimirLinha(int id, BenchMetrics *m) {
    long totalAcessos = m->hitsL1 + m->missesL1;
    
    float pL1 = totalAcessos ? (float)m->hitsL1 * 100.0 / totalAcessos : 0.0;
    long tL2 = m->hitsL2 + m->missesL2;
    float pL2 = tL2 ? (float)m->hitsL2 * 100.0 / tL2 : 0.0;
    long tL3 = m->hitsL3 + m->missesL3;
    float pL3 = tL3 ? (float)m->hitsL3 * 100.0 / tL3 : 0.0;
    
    float taxaRAM = totalAcessos ? ((float)m->missesL3 / totalAcessos) * 100.0 : 0.0;

    printf(TAB_VER " M%-d" TAB_VER " %-4d " TAB_VER " %-4d " TAB_VER " %-4d ",id, m->tamL1, m->tamL2, m->tamL3);
    printf(TAB_VER " %10d " TAB_VER " %9.1f%% " TAB_VER " %10d ",m->hitsL1, pL1, m->missesL1);
    printf(TAB_VER " %10d " TAB_VER " %9.1f%% " TAB_VER " %10d ",m->hitsL2, pL2, m->missesL2);
    printf(TAB_VER " %10d " TAB_VER " %9.1f%% " TAB_VER " %10d ",m->hitsL3, pL3, m->missesL3);
    printf(TAB_VER " %9.1f%% " TAB_VER " %18ld " TAB_VER "\n",taxaRAM, m->relogio);
}

void rodape() {
    printf(TAB_BL H3 TAB_BJ H6 TAB_BJ H6 TAB_BJ H6);
    printf(TAB_BJ H12 TAB_BJ H12 TAB_BJ H12);
    printf(TAB_BJ H12 TAB_BJ H12 TAB_BJ H12);
    printf(TAB_BJ H12 TAB_BJ H12 TAB_BJ H12);
    printf(TAB_BJ H12 TAB_BJ H20 TAB_BR "\n");
}

void testePadrao(ConfigItem *configs) {
    endwin();
    setbuf(stdout, NULL);
    system("clear"); 

    int prob = selecionarProbabilidade();
    int nFor = selecionarNFor();
    
    system("clear"); 

    int maquinas[5][3] = { 
        {8, 16, 32}, {32, 64, 128}, {16, 64, 256}, {8, 32, 128}, {16, 32, 64} 
    };
    
    BenchMetrics resultados[5]; 
    BenchMetrics totalizador;
    inicializarMetricas(&totalizador);

    totalizador.tamRAM = 1000; 
    totalizador.N_PROB = prob; 
    totalizador.N_FOR = nFor;


    for (int i = 0; i < 5; i++) {
        BenchMetrics *r = &resultados[i];
        inicializarMetricas(r);
        
        r->tamL1 = maquinas[i][0]; 
        r->tamL2 = maquinas[i][1]; 
        r->tamL3 = maquinas[i][2];
        r->tamRAM = 1000; 
        r->N_PROB = prob; 
        r->N_FOR = nFor;
        r->tamWriteBuffer = configs[0].ativo ? 4 : -1;

        CacheBenchmark(r, configs);
        totalizador.qtdStalls += r->qtdStalls;
    }

    system("clear");
    printf("\n" BOLD(CYAN("Resultados:")) "\n");

    exibirInfoGeral(&totalizador, configs);
    cabecalho();

    for (int i = 0; i < 5; i++) {
        imprimirLinha(i + 1, &resultados[i]);

        if (i < 4) { 
            printf(TAB_ML H3 TAB_MJ H6 TAB_MJ H6 TAB_MJ H6);
            printf(TAB_MJ H12 TAB_MJ H12 TAB_MJ H12);
            printf(TAB_MJ H12 TAB_MJ H12 TAB_MJ H12);
            printf(TAB_MJ H12 TAB_MJ H12 TAB_MJ H12);
            printf(TAB_MJ H12 TAB_MJ H20 TAB_MR "\n");
        }
    }

    rodape();
    printf("\n" GREEN("Pressione ENTER para sair...") "\n");
    int c; 
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
    refresh();
}

void salvaTabela(int *qtdSalva, BenchMetrics *tabelaSalva, BenchMetrics m )
{
    printf("\n" YELLOW("Deseja salvar esse resultado na tabela? [S/N]: ") " ");
    char resp = tolower(getchar()); 
    if (resp != '\n') {
        int c; 
        while ((c = getchar()) != '\n' && c != EOF);
    }
    if (resp == 's') {
            if ((*qtdSalva) < 50) {
                tabelaSalva[(*qtdSalva)] = m; // Copia os dados para o array
                (*qtdSalva)++;
                printf(GREEN("Resultado salvo com sucesso! ID: %d") "\n", (*qtdSalva));
            } else 
                printf(RED("Memoria cheia! Nao e possivel salvar mais.") "\n");
            
        printf("Pressione ENTER para continuar...");
        getchar();
    }
}

void exibirRelatorioIndividual(BenchMetrics *m, ConfigItem *configs) {
    endwin();
    setbuf(stdout, NULL);
    system("clear");
    exibirInfoGeral(m, configs);
    cabecalho();
    imprimirLinha(1, m);
    rodape();
    printf("\n");
    printf("\n\nPressione ENTER para voltar ao menu...");
    getchar();
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

int selecionarProbabilidade() {
    system("clear");
    printf("Escolha a Probabilidade de Repeticao:\n");
    printf("1 - 50%%\n");
    printf("2 - 75%%\n");
    printf("3 - 90%% \n");
    printf("\nEscolha: ");
    int op;
    scanf("%d", &op); 
    switch(op) {
        case 1: 
            return 50;
        case 2: 
            return 75;
        case 3: 
            return 90;
        default: 
            return 75;
    }
}

int selecionarNFor() {
    system("clear");
    printf("\n" BOLD(YELLOW("Tamanho do for ")) "\n\n");  
    printf("1 - 5 inst.\n");
    printf("2 - Personalizado\n");
    printf("\nEscolha: ");
    int op;
    scanf("%d", &op); 
    switch(op) {
        case 1: 
            return 5;
        case 2: {
            printf("\nDigite o tamanho do loop desejado: ");
            int val;
            scanf("%d", &val);
            return (val > 0) ? val : 5;
        }
        default: 
            return 5;
    }
}

void imprimirTabelaSalva(BenchMetrics *lista, int qtd) {
    endwin();
    setbuf(stdout, NULL);
    system("clear");

    printf("\n" BOLD(MAGENTA("Tabela de resultados salvos")) "\n\n");
    if (qtd == 0) {
        printf(RED("Nenhum resultado salvo.") "\n");
    } else {
        for(int i =0; i<qtd; i++)
        {
            printf(BOLD("Probabilidade de repetição = %d %%  e tamanho do for = %d da M%-d\n"),lista[i].N_PROB, lista[i].N_FOR, i+1);
        }
        cabecalho();
        for (int i = 0; i < qtd; i++) {
            imprimirLinha(i + 1, &lista[i]);

            if (i < qtd - 1) {
               printf(TAB_ML H3 TAB_MJ H6 TAB_MJ H6 TAB_MJ H6);
               printf(TAB_MJ H12 TAB_MJ H12 TAB_MJ H12);
               printf(TAB_MJ H12 TAB_MJ H12 TAB_MJ H12);
               printf(TAB_MJ H12 TAB_MJ H12 TAB_MJ H12);
               printf(TAB_MJ H12 TAB_MJ H20 TAB_MR "\n");
            }
        }
        rodape();
    }

    printf("\n" GREEN("Pressione ENTER para voltar ao menu...") "\n");
    
    getchar();
    refresh();
}