//
// Created by lfb on 09/04/2021.
//

#include <string.h>
#include <stdlib.h>
#include "menus.h"
#include "sudokuClassic.h"
#include "sudokuHex.h"
#include "sudokuX.h"
#include "sudokuAlpha.h"


/* Imprime o Logo e Welcome Banner da aplicação*/
void printLogo(){
    // WECOLME BANNER - criado @ https://manytools.org/hacker-tools/ascii-banner/
    printf("  ______ _      ________   __   _____ _    _ _____   ____  _  ___    _ \n");
    printf(" |  ____| |    |  ____\\ \\ / /  / ____| |  | |  __ \\ / __ \\| |/ / |  | |\n");
    printf(" | |__  | |    | |__   \\ V /  | (___ | |  | | |  | | |  | | ' /| |  | |\n");
    printf(" |  __| | |    |  __|   > <    \\___ \\| |  | | |  | | |  | |  < | |  | |\n");
    printf(" | |    | |____| |____ / . \\   ____) | |__| | |__| | |__| | . \\| |__| |\n");
    printf(" |_|    |______|______/_/ \\_\\ |_____/ \\____/|_____/ \\____/|_|\\_\\\\____/ \n");
    printf("\n");
}
void printBanner(){
    printMsgLine();
    printMsgString("LFBatista - 2021");
    printMsgString("IPT - TePSI - Prog II");
    printMsgString("Project: Flex Sudoku");
    printMsgLine();
}

// limpa consola e imprime o Logo de novo
void clearConsole(){
    //limpa a consola
    system("clear");
    printLogo();
    printMsgLine();
}

/*Imprime na consola uma caixa de texto com 71 caracteres em cada linha
A string passada (msg) será encaixada na 2a linha através da função fitString*/
void printMessage(char * msg){
    printMsgLine(71);
    printMsgString(msg);
    printMsgLine(71);
    printf("\n");
}
void printMsgLine(){
    for (int i = 0; i < 71; ++i) {
        printf("*");
    }
    printf("\n");
}
void printMsgString(char *msg){
    static char txt[256];
    //CALCULA O ESPAÇO RESTANTE NA LINHA APÓS A INSERÇÃO DA MENSAGEM PRETENDIDA
    int spaceLeft = 70 - strlen(msg);

    //IMPRIME A PRIMEIRA PARTE DA LINHA COM O 1 CARACTER *
    txt[0] = '*';
    for (int i = 1; i < spaceLeft/2; ++i) {
        txt[i] = ' ';
    }
    //IMPRIME A MSG NO MEIO DA STRING
    for (int i = 0; i < strlen(msg); ++i) {
        txt[i+(spaceLeft/2)] = msg[i];
    }
    //IMPRIME A ULTIMA PARTE DA STRING COM O ULTIMO CARACTER *
    int value=(spaceLeft/2)+strlen(msg);
    for (int i = value; i < 70; ++i) {
        txt[i] = ' ';
    }
    txt[70] = '*';
    txt[71] = '\n';
    txt[72] = '\0';
    printf("%s", txt);
}
void printOptionString(char *msg){
    static char txt[256];
    //CALCULA O ESPAÇO RESTANTE NA LINHA APÓS A INSERÇÃO DA MENSAGEM PRETENDIDA
    int spaceLeft = 70 - strlen(msg);

    //IMPRIME A PRIMEIRA PARTE DA LINHA COM O 1 CARACTER *
    txt[0] = '*';
    for (int i = 1; i < spaceLeft/2; ++i) {
        txt[i] = ' ';
    }
    //IMPRIME A MSG NO MEIO DA STRING
    for (int i = 0; i < strlen(msg); ++i) {
        txt[i+(spaceLeft/2)] = msg[i];
    }
    txt[(spaceLeft/2)+strlen(msg)] = '\0';
    printf("%s", txt);
}

//Menus do jogo (welcome - mode selection - level & rules & new/load)
void initProg(){
    clearConsole();
    printBanner();
    printMsgString("Press ENTER to continue...");
    printMsgLine();

    getchar();
    fflush(stdin);

    clearConsole();
    modeMenu();
}
void modeMenu(){
    int opt;
    char c;
    printMsgLine();
    printMsgString("CHOOSE YOUR GAME");
    printMsgLine();
    printMsgString("(1) Classic Sudoku - 4x4");
    printMsgString("(2) Classic Sudoku - 9x9");
    printMsgString("(3) Hex Sudoku - 16x16");
    printMsgString("(4) X Sudoku - 9x9");
    printMsgString("(5) Alpha Sudoku - 9x9");
    printMsgString("(0) Go back..");
    printMsgLine();
    do {
        printOptionString("Option: ");
        scanf("%c", &c);
        fflush(stdin);
        if(c < '0' || c > '5')
            printMsgString("Option Invalid");
    } while (c < '0' || c > '5');
    printMsgLine();

    opt = (int)c - 48;
    switch (opt) {
        case 1:
            levelMenu(opt, "WELCOME TO CLASSIC SUDOKU 4x4");
            break;
        case 2:
            levelMenu(opt, "WELCOME TO CLASSIC SUDOKU 9x9");
            break;
        case 3:
            levelMenu(opt, "WELCOME TO HEX SUDOKU 16x16");
            break;
        case 4:
            levelMenu(opt, "WELCOME TO X SUDOKU 9x9");
            break;
        case 5:
            levelMenu(opt, "WELCOME TO ALPHA SUDOKU 9x9");
            break;
        case 0:
            initProg();
            break;
    }
}
void levelMenu(int mode, char *msg){
    int opt;
    char c;
    clearConsole();
    printMsgLine();
    printMsgString(msg);
    printMsgString("MENU");
    printMsgLine();
    printMsgString("(1) New Beginner Puzzle");
    printMsgString("(2) New Veteran Puzzle");
    printMsgString("(3) Show Rules");
    printMsgString("(4) Load Saved Puzzle");
    printMsgString("(0) Go Back..");
    printMsgLine();
    do {
        printOptionString("Option: ");
        scanf("%c", &c);
        fflush(stdin);
        if(c < '0' || c > '4')
            printMsgString("Option Invalid");
    } while (c < '0' || c > '4');
    printMsgLine();

    clearConsole();

    opt = (int)c - 48;
    switch (opt) {
        case 1:
            gameSelection(mode, opt);
            break;
        case 2:
            gameSelection(mode, opt);
            break;
        case 3:
            showRules(mode);
            break;
        case 4:
            loadGameSelection(mode);
            break;
        case 0:
            modeMenu();
            break;
    }
}
void showRules(int mode){
    switch (mode) {
        case 1:
            clearConsole();
            printMsgLine();
            printMsgString("Fill in each space with numbers from 1-4");
            printMsgString("Numbers can only be used once per line, row and internal grid");
            printMsgString(" ");
            printMsgString("Press ENTER to go back..");
            printMsgLine();

            getchar();
            fflush(stdin);

            levelMenu(mode, "WELCOME TO CLASSIC SUDOKU 4x4");
            break;
        case 2:
            clearConsole();
            printMsgLine();
            printMsgString("Fill in each space with numbers from 1-9");
            printMsgString("Numbers can only be used once per line, row and internal grid");
            printMsgString(" ");
            printMsgString("Press ENTER to go back..");
            printMsgLine();

            getchar();
            fflush(stdin);

            levelMenu(mode, "WELCOME TO CLASSIC SUDOKU 9x9");
            break;
        case 3:
            clearConsole();
            printMsgLine();
            printMsgString("Fill in each space with hexadecimal values");
            printMsgString("(Numbers from 0-9 & Letteres from A-F)");
            printMsgString("Numbers can only be used once per line, row and internal grid");
            printMsgString(" ");
            printMsgString("Press ENTER to go back..");
            printMsgLine();

            getchar();
            fflush(stdin);

            levelMenu(mode, "WELCOME TO HEX SUDOKU 16x16");
            break;
        case 4:
            clearConsole();
            printMsgLine();
            printMsgString("Fill in each space with numbers from 1-9");
            printMsgString("Classic Sudoku rules with the addition of one more constraint");
            printMsgString("In this style, numbers can only be used once on each main diagonal");
            printMsgString(" ");
            printMsgString("Press ENTER to go back..");
            printMsgLine();

            getchar();
            fflush(stdin);

            levelMenu(mode, "WELCOME TO X SUDOKU 9x9");
            break;
        case 5:
            clearConsole();
            printMsgLine();
            printMsgString("Fill in each space with letters from A-I");
            printMsgString("Letters can only be used once per line, row and internal grid");
            printMsgString(" ");
            printMsgString("Press ENTER to go back..");
            printMsgLine();

            getchar();
            fflush(stdin);

            levelMenu(mode, "WELCOME TO ALPHABET SUDOKU 9x9");
            break;
    }
}

//Ligação entre modo e nivel de jogo e bibliotecas a utilizar (classic, hex, x, alpha)
void gameSelection(int mode, int level){
    switch (mode) {
        case 1:
            initClassicGame(4, level, 0);
            break;
        case 2:
            initClassicGame(9, level, 0);
            break;
        case 3:
            initHexGame(level, 0);
            break;
        case 4:
            initXGame(9, level);
            break;
        case 5:
            initAlphaGame(level);
            break;
    }
}

//salva jogos classicos em ficheiros txt
void saveGame(matrix board) {

    char path[200] = "Saved/";
    char filename[20];
    char ext[10] = ".txt";

    printOptionString("NAME YOUR SAVEFILE: ");
    fgets(filename, sizeof(filename), stdin);
    fflush(stdin);

    strcat(path, filename);
    strcat(path, ext);

    FILE *fp = NULL;
    fp = fopen(path, "w");

    if(fp == NULL){
        printf("error opening the file..");
        return;
    }

    for (int i = 0; i < board.size; ++i) {
        for (int j = 0; j < board.size; ++j) {
            fprintf(fp, "%d ", board.matrix[i][j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}
//salva jogos hex em ficheiros binários
void saveGameHex(cMatrix board){
    char path[200] = "Saved/";
    char filename[20];
    char ext[10] = ".dat";

    printOptionString("NAME YOUR SAVEFILE: ");
    fgets(filename, sizeof(filename), stdin);
    fflush(stdin);

    strcat(path, filename);
    strcat(path, ext);

    FILE *fp = NULL;
    fp = fopen(path, "wb"); //em binario

    if(fp == NULL){
        printf("error opening the file..");
        return;
    }
    fwrite(&board, sizeof(board), 1, fp); //gravar o jogo
    //gavar o tabuleiro
    for (int y = 0; y < board.size; ++y) {
        for (int x = 0; x < board.size; ++x) {
            fwrite(&board.matrix[y][x], sizeof(board.matrix[y][x]), 1, fp); //gravar o jogo
        }

    }
    fclose(fp);
}
//salva jogos alpha em ficheiros txt
void saveGameHexAlpha(cMatrix board){
    char path[200] = "Saved/";
    char filename[20];
    char ext[10] = ".txt";

    printOptionString("NAME YOUR SAVEFILE: ");
    fgets(filename, sizeof(filename), stdin);
    fflush(stdin);

    strcat(path, filename);
    strcat(path, ext);

    FILE *fp = NULL;
    fp = fopen(path, "w");

    if(fp == NULL){
        printf("error opening the file..");
        return;
    }

    for (int i = 0; i < board.size; ++i) {
        for (int j = 0; j < board.size; ++j) {
            fprintf(fp, "%c ", board.matrix[i][j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}

//faz a ligação apartir do menu "Load" e a respectiva biblioteca do tipo de puzzle
void loadGameSelection(int mode){
    int level = 1;
    switch (mode) {
        case 1:
            initClassicGame(4, level, 1);
            break;
        case 2:
            initClassicGame(9, level, 1);
            break;
        case 3:
            initHexGame(level, 1);
            break;
        case 4:
            initClassicGame(9, level, 1);
            break;
        case 5:
            loadAlphaGame();
            break;
    }
}

