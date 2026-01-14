#ifndef MENU_H
#define MENU_H

#include <ncurses.h>
#include "structs.h"

void menu_init();
void menu_close();
int menu_run(char *opcoes[], int n_opcoes, char *titulo);
void menu_checkbox(ConfigItem *itens, int n_itens, char *titulo);
int menu_valor(char *mensagem);

#endif