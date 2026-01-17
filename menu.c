#include "menu.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h> 


#define COR_VERDE 1
#define COR_VERMELHO 2
#define COR_AMARELO 3
#define COR_TITULO 4

void menu_init() {
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
}

void menu_close() {
    endwin();
}

// Auxiliar para desenhar menus de lista
void desenhar_opcoes(int highlight, char *opcoes[], int n_opcoes, char *titulo) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    erase();
    box(stdscr, 0, 0);
    mvprintw(1, (max_x - strlen(titulo)) / 2, "%s", titulo);

    int start_y = (max_y - n_opcoes) / 2;
    if (start_y < 2) start_y = 2;

    for(int i = 0; i < n_opcoes; ++i) {
        int x = (max_x - strlen(opcoes[i])) / 2;
        int y = start_y + i;
        if(highlight == i + 1) {
            attron(A_REVERSE);
            mvprintw(y, x, "%s", opcoes[i]);
            attroff(A_REVERSE);
        } else {
            mvprintw(y, x, "%s", opcoes[i]);
        }
    }
    refresh();
}

int menu_run(char *opcoes[], int n_opcoes, char *titulo) {
    int highlight = 1;
    int c;
    desenhar_opcoes(highlight, opcoes, n_opcoes, titulo);
    while(1) {
        c = getch();
        switch(c) {
            case KEY_UP:
                if(highlight == 1) highlight = n_opcoes;
                else --highlight;
                break;
            case KEY_DOWN:
                if(highlight == n_opcoes) highlight = 1;
                else ++highlight;
                break;
            case 10: return highlight;
            // case KEY_F(10): return -1;
        }
        desenhar_opcoes(highlight, opcoes, n_opcoes, titulo);
    }
}

// Auxiliar para desenhar checkboxes
void desenhar_structs(int highlight, ConfigItem *itens, int n_itens, char *titulo) {
    int max_y, max_x;
    getmaxyx(stdscr
        , max_y, max_x);
    erase();
    
    box(stdscr, 0, 0);
    mvprintw(1, (max_x - strlen(titulo)) / 2, "%s", titulo);

    int start_y = (max_y - n_itens) / 2;
    if (start_y < 3) start_y = 3;

    for(int i = 0; i < n_itens; ++i) {
        int len = strlen(itens[i].texto);
        int offset = itens[i].flagBotao ? 0 : 4;
        int x = (max_x - (len + offset)) / 2;
        int y = start_y + i;
        char marcador = itens[i].ativo ? 'X' : ' ';

        if(highlight == i + 1) {
            attron(A_REVERSE);
            if (itens[i].flagBotao) mvprintw(y, x, " %s ", itens[i].texto);
            else mvprintw(y, x, "[%c] %s", marcador, itens[i].texto);
            attroff(A_REVERSE);
        } else {
            if (itens[i].flagBotao) mvprintw(y, x, " %s ", itens[i].texto);
            else mvprintw(y, x, "[%c] %s", marcador, itens[i].texto);
        }
    }
    refresh();
}

// Menu de checkboxes
void menu_checkbox(ConfigItem *itens, int n_itens, char *titulo) {
    int highlight = 1;
    int c;
    desenhar_structs(highlight, itens, n_itens, titulo);
    while(1) {
        c = getch();
        switch(c) {
            case KEY_UP:
                if(highlight == 1) highlight = n_itens;
                else --highlight;
                break;
            case KEY_DOWN:
                if(highlight == n_itens) highlight = 1;
                else ++highlight;
                break;
            case 32: // Espaço
            case 10: // Enter
                if (itens[highlight-1].flagBotao) return;
                itens[highlight-1].ativo = !itens[highlight-1].ativo;
                break;
            case KEY_F(10): return;
        }
        desenhar_structs(highlight, itens, n_itens, titulo);
    }
}
// Menu para entrada de valor numérico
int menu_valor(char *mensagem) {
    char input[50];
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    echo();
    curs_set(1);
    erase();
    box(stdscr, 0, 0);
    mvprintw(max_y / 2 - 1, (max_x - strlen(mensagem)) / 2, "%s", mensagem);
    move(max_y / 2 + 1, (max_x - 10) / 2);
    
    getnstr(input, 49); 

    noecho();
    curs_set(0);

   
    if (strlen(input) == 0) return -1;
    for (int i = 0; i < (int)strlen(input); i++) {
        if (!isdigit(input[i])) return -1; // Se houver algo que não é número, retorna -1
    }

    return atoi(input);
}

int mostrar_relatorio(BenchMetrics *m) {
    clear();
    box(stdscr, 0, 0);

    if (has_colors()) {
        start_color();
        init_pair(COR_VERDE, COLOR_GREEN, COLOR_BLACK);
        init_pair(COR_VERMELHO, COLOR_RED, COLOR_BLACK);
        init_pair(COR_AMARELO, COLOR_YELLOW, COLOR_BLACK);
        init_pair(COR_TITULO, COLOR_CYAN, COLOR_BLACK);
    }

    long tL1 = m->hitsL1 + m->missesL1;
    long tL2 = m->hitsL2 + m->missesL2;
    long tL3 = m->hitsL3 + m->missesL3;

    float pL1 = tL1 ? (float)m->hitsL1/tL1 * 100 : 0;
    float pL2 = tL2 ? (float)m->hitsL2/tL2 * 100 : 0;
    float pL3 = tL3 ? (float)m->hitsL3/tL3 * 100 : 0;

    attron(A_BOLD | COLOR_PAIR(COR_TITULO));
    mvprintw(1, 2, "=== RELATORIO DE BENCHMARK ===");
    attroff(A_BOLD | COLOR_PAIR(COR_TITULO));

    mvhline(2, 1, ACS_HLINE, getmaxx(stdscr)-2); 


    attron(A_BOLD);
    mvprintw(3, 2, "CONFIGURACAO:");
    attroff(A_BOLD);

    mvprintw(4, 4, "L1: %d | L2: %d | L3: %d (blocos)", m->tamL1, m->tamL2, m->tamL3);
    mvprintw(5, 4, "RAM: %d blocos", m->tamRAM);


    mvprintw(6, 4, "Buffer de Escrita: ");
    if (m->tamWriteBuffer != -1) {
        attron(COLOR_PAIR(COR_VERDE) | A_BOLD);
        printw("ATIVADO (Tam: %d)", m->tamWriteBuffer);
        attroff(COLOR_PAIR(COR_VERDE) | A_BOLD);
    } else {
        attron(COLOR_PAIR(COR_VERMELHO) | A_BOLD);
        printw("DESATIVADO");
        attroff(COLOR_PAIR(COR_VERMELHO) | A_BOLD);
    }

    mvhline(7, 1, ACS_HLINE, getmaxx(stdscr)-2);


    attron(A_BOLD);
    mvprintw(8, 2, "ESTATISTICAS DE CACHE:");
    attroff(A_BOLD);

    mvprintw(9, 4,  "L1 Hits: %-8d (%5.1f%%) | Misses: %-8d", m->hitsL1, pL1, m->missesL1);
    mvprintw(10, 4, "L2 Hits: %-8d (%5.1f%%) | Misses: %-8d", m->hitsL2, pL2, m->missesL2);
    mvprintw(11, 4, "L3 Hits: %-8d (%5.1f%%) | Misses: %-8d", m->hitsL3, pL3, m->missesL3);
    
    mvprintw(12, 4, "Acessos a RAM: %d", m->missesL3);

    mvhline(13, 1, ACS_HLINE, getmaxx(stdscr)-2);


    mvprintw(15, 4, "CPU Stalls: ");
    
    if (m->tamWriteBuffer != -1) {
        if (m->qtdStalls > 0) {
            attron(COLOR_PAIR(COR_AMARELO) | A_BLINK); 
            printw("%ld Stalls de CPU", m->qtdStalls);
            attroff(COLOR_PAIR(COR_AMARELO) | A_BLINK);
        } else {
            attron(COLOR_PAIR(COR_VERDE));
            printw("Nenhum Stall de CPU identificado");
            attroff(COLOR_PAIR(COR_VERDE));
        }
    } else
        printw("Buffer desligado");
    

    mvhline(16, 1, ACS_HLINE, getmaxx(stdscr)-2);


    attron(A_REVERSE | A_BOLD);
    mvprintw(18, 2, " TEMPO TOTAL: %ld ciclos ", m->relogio);
    attroff(A_REVERSE | A_BOLD);


    mvprintw(21, 2, "Deseja salvar este resultado na Tabela? (S/N)");
    refresh();


    while(1) {
        int c = tolower(getch()); 
        if (c == 's') return 1;
        if (c == 'n' || c == 27) return 0; // 27 = ESC
        
        attron(COLOR_PAIR(COR_VERMELHO));
        mvprintw(22, 2, "Opcao invalida! Use S ou N.");
        attroff(COLOR_PAIR(COR_VERMELHO));
        refresh();
    }
}
