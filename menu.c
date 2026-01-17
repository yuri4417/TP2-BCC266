#include "menu.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h> 

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

int mostrar_relatorio(BenchMetrics *m, long tempoTotal) {
    clear();
    box(stdscr, 0, 0);
    
    attron(A_BOLD);
    mvprintw(1, 2, "--- RESULTADOS DA SIMULACAO ---");
    attroff(A_BOLD);

    mvprintw(3, 2, "Tempo Total: %ld ciclos", tempoTotal);
    
    mvprintw(5, 2, "L1 Hits: %d | Misses: %d", m->hitsL1, m->missesL1);
    mvprintw(6, 2, "L2 Hits: %d | Misses: %d", m->hitsL2, m->missesL2);
    mvprintw(7, 2, "L3 Hits: %d | Misses: %d", m->hitsL3, m->missesL3);
    
    mvprintw(9, 2, "RAM Size: %d blocos", m->tamRAM);
    mvprintw(10, 2, "Buffer Size: %d", m->tamWriteBuffer);

    attron(A_REVERSE);
    mvprintw(15, 2, " Deseja salvar este resultado na Tabela? (S/N) ");
    attroff(A_REVERSE);
    
    refresh();

    while(1) {
        int c = getch(); 
        c = tolower(c);  

        if (c == 's') {
            return 1; 
        }
        else if (c == 'n' || c == 27) { // 27 = ESC
            return 0; 
        }
        
        mvprintw(16, 2, "Opcao invalida! Pressione S ou N.");
        refresh();
    }
}