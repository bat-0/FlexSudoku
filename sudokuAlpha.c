//
// Created by lfb on 19/04/2021.
//

#include "sudokuAlpha.h"
#include "sudokuHex.h"
#include "menus.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
const char ALPHADICT[] = "ABCDEFGHI";

void initAlphaGame(int level){
    srand(time(NULL));
    //criar a matrix onde se irá jogar
    cMatrix board = createMatrixHex(9);
    clearHex(board);
    //cria a matrix onde se irá carregar os dados do ficheiro contendo a solução
    cMatrix template = createMatrixHex(9);
    clearHex(template);

    filePath("93");

    printMsgLine();
    //importa a matrix guardada no ficheiro txt escolhido aleatoriamente para a matrix "template"
    readGameFileHex(template, SUDOKUFILE);


    //vector que irá armazenar as coordenadas de todos os numeros obtidos da matrix "template"

    int index = 0;
    int setNumbers = 9 * 5 - 9 * level - 6;
    int sizeVector = 2*setNumbers;
    int vector[sizeVector];
    for (int i = 0; i < setNumbers; ++i) {
        setPiecesHex(template, board, vector, index);
        index+=2;
    }

    printAlpha(board);
    printf("\n");
    printMessage("TYPE IN ANY PARAMETER (Q) QUIT GAME (S) SAVE GAME (H) SHOW HINT");

    while(zeroCountHex(board) == 0) {
        playAlpha(board, template, vector, sizeVector);
    }

    printMessage("GAME OVER - CONGRATULATIONS");
    printMessage("Press ENTER to leave this game..");
    printMsgLine();
    fflush(stdin);
    getchar();
    fflush(stdin);


    freeMemoryHex(board);
    freeMemoryHex(template);

    initProg();
}
void loadAlphaGame(){
    char path[200] = "Saved/";
    char filename[20];
    char ext[10] = ".txt";

    clearConsole();
    printMsgLine();
    printOptionString("FILE TO LOAD: ");
    fgets(filename, sizeof(filename), stdin);

    //concatenação do pasta, com o nome dado pelo utilizador mais extensão do ficheiro
    strcat(path, filename);
    strcat(path, ext);


    cMatrix board = createMatrixHex(9);
    clearHex(board);

    /*TODO CORRIGIR LEITURA DE FICHEIRO BINARIO - ERRO: 20475 segmentation fault
    readBinFile(board, path);*/

    //leitura do ficheiro txt (saved game)
    readGameFileHex(board, path);


    //dummy vector
    int vector[2] = {99,99};

    clearConsole();
    printMessage("Game Successfuly Loaded");
    printAlpha(board);
    printf("\n");
    printMessage("TYPE IN ANY PARAMETER (Q) QUIT GAME (S) SAVE GAME (H) SHOW HINT");

    //while loop ate ao jogo terminar, verificando se existem casas vazias
    while(zeroCountHex(board) == 0) {
        playHex(board, board, vector, 2);
    }

    printMessage("GAME OVER - CONGRATULATIONS");
    printMessage("Press ENTER to leave this game..");
    fflush(stdin);
    getchar();
    fflush(stdin);

    //liberta a memora alocada a matriz "board"
    freeMemoryHex(board);

    initProg();



}

//imprime as matrizes na consola
void printAlpha(cMatrix game){
    printf("     ");
    for (int j = 0; j < game.size; ++j)
        printf("%3d ", j+1);
    printf("\n     ");
    for (int j = 1; j <= game.size; ++j)
        printf("****");
    printf("\n    |\n");
    for (int y = 0; y < game.size; y++) {
        printf("%3d |", y+1);
        for (int x = 0; x < game.size; x++) {
            printf("%3c ", game.matrix[y][x]);
        }
        printf("\n    |\n");
    }
    printf("     ");
    for (int y = 0; y < game.size; y++)
        printf("****");
    printf("\n");
}

void playAlpha(cMatrix board, cMatrix template, int vector[], int sizeVector){
    char line, col, num;
    int lineINT, colINT;
    int hints = 0;

    //recolhe user input (coordenadas & valor a submeter) - o utlizador pode ainda usar Q para sair e S para guardar
    do {
        printf("\n");
        printMessage("NEW MOVE");
        printf("Line: ");
        scanf("%c", &line);
        line = toupper(line);
        fflush(stdin);
        if(line == 'Q') {
            initProg();
            break;
        }
        if(line == 'S'){
            saveGameHexAlpha(board);
            clearConsole();
            printMessage("GAME SAVED");
            printAlpha(board);
            continue;
        }
        if(line == 'H'){
            hints++;
            clearConsole();
            showHintHex(board, template, hints);
            printAlpha(board);
            continue;
        }
        lineINT = (int)line - 48;
        printf("Column: ");
        scanf("%c", &col);
        col = toupper(col);
        fflush(stdin);
        if(col == 'Q') {
            initProg();
            break;
        }
        if(col == 'S'){
            saveGameHexAlpha(board);
            clearConsole();
            printMessage("GAME SAVED");
            printAlpha(board);
            continue;
        }
        if(col == 'H'){
            hints++;
            clearConsole();
            showHintHex(board, template, hints);
            printAlpha(board);
            continue;
        }
        colINT = (int)col - 48;
        printf("Letter: ");
        scanf("%c", &num);
        num = toupper(num);
        fflush(stdin);
        if(num == 'Q') {
            initProg();
            break;
        }
        if(num == 'S'){
            saveGameHexAlpha(board);
            clearConsole();
            printMessage("GAME SAVED");
            printAlpha(board);
            continue;
        }
        printf("\n");
    }while(lineINT < 1 || lineINT > board.size || colINT < 1 || colINT > board.size  || validateInputAlpha(num));



    //verifica se as coordenadas coincidem com um dos numeros dados pelo puzzle
    if(coordinateVerify(vector, sizeVector, lineINT-1, colINT-1) > 0){
        clearConsole();
        printMessage("YOU CANNOT CHANGE THIS LOCATION");
        printAlpha(board);
        playHex(board, template, vector, sizeVector);
    }

    //verifica se a casa escolhida ja foi alterada previamente, se sim pede confirmação para alterar novamente
    if(board.matrix[lineINT-1][colINT-1] != '_'){
        char reply;
        printMsgLine();
        clearConsole();
        printf("\n*                (Play - Line: %c Column: %c  Number: %c)                *\n\n", line, col, num);
        printOptionString("ARE YOU SURE YOU WANT TO CHANGE THIS CELL? (y/n)");
        fflush(stdin);
        scanf("%c", &reply);
        fflush(stdin);
        printMsgLine();
        if(reply == 'y' || reply == 'Y')
            board.matrix[lineINT][colINT] = num;
        else if(reply == 'n' || reply == 'N'){
            clearConsole();
            printf("\n*            (Play canceled - Line: %c Column: %c  Number: %c)             *\n\n", line, col, num);
            printAlpha(board);
            playAlpha(board, template, vector, sizeVector);
        }
    }else{board.matrix[lineINT-1][colINT-1] = num;}

    //verifica se jogada é valida - caso não, indica a regra quebrada + alteração efectuada
    clearConsole();
    if(guessVerifyHex(board, lineINT-1, colINT-1) == 1) {
        printMsgLine();
        printf("*                   (Play undone - Line: %c Column: %c)                 *\n\n", line, col);
        board.matrix[lineINT][colINT] = '_';
        printAlpha(board);
        playAlpha(board, template, vector, sizeVector);
    }
    clearConsole();
    printf("\n*           (Previous play - Line: %c Column: %c  Number: %c)            *\n\n", line, col, num);
    printAlpha(board);
}

int validateInputAlpha(char c){
    for (int i = 0; i < strlen(ALPHADICT); ++i) {
        if(c==ALPHADICT[i])
            return 0;
    }
    return 1;
}


