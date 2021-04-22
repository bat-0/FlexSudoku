//
// Created by lfb on 09/04/2021.
//

#ifndef SUDOKU_3_MENUS_H
#define SUDOKU_3_MENUS_H

#include "sudokuClassic.h"
#include "sudokuHex.h"
#include <stdio.h>

// WELCOME BANNER
void printLogo();
void printBanner();

// LIMPA A CONSOLA
void clearConsole();

// CAIXA DE TEXTO
void printMessage(char *msg);
// IMPRIME TEXTO DENTRO DA CAIXA DE TEXTO
void printMsgString(char *msg);
// LINE BREAKER "*****"
void printMsgLine();
// LINHA DE INPUT
void printOptionString(char *msg);
//INIT - MENU PRINCIPAL
void initProg();
//MENU DE MODOS DE SUDOKU
void modeMenu();
// ECRA COM AS REGRAS DE CADA MODO DE JOGO
void showRules(int mode);
// MENU REFERENTE A CADA MODO DE JOGO
void levelMenu(int mode, char *msg);
//FUNCAO DE LIGAÇAO ENTRE MENUS E BIBLIOTECAS DE JOGO
void gameSelection(int mode, int level);
// SALVA MATRIZES DE JOGO EM FICHEIROS .TXT (Saved/filename.txt)
void saveGame(matrix board);
// SALVA MATRIZES DE JOGO EM FICHEIROS .DAT (Saved/filename.dat)
void saveGameHex(cMatrix board);
void saveGameHexAlpha(cMatrix board);
// LIGAÇÃO ENTRE O MENU DE JOGO E A LEITURA DE UM JOGO GRAVADO
void loadGameSelection(int mode);

#endif //SUDOKU_3_MENUS_H
