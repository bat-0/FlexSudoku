//
// Created by lfb on 17/04/2021.
//

#include "sudokuX.h"
#include "sudokuClassic.h"
#include "menus.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void initXGame(int size, int level){
    srand(time(NULL));
    //criar a matrix onde se irá jogar
    matrix board = createClassicMatrix(size);
    clear(board);
    //cria a matrix onde se irá carregar os dados do ficheiro contendo a solução
    matrix template = createClassicMatrix(size);
    clear(template);

    //escolhe aleatoriamente o ficheiro template e importa o caminho para a variavel templateSrc
    filePath("92");

    printMsgLine();
    //importa a matrix guardada no ficheiro txt escolhido aleatoriamente para a matrix "template"
    readGameFile(template, SUDOKUFILE);


    int index = 0;
    int setNumbers = size * 5 - size * level - 6;
    int sizeVector = 2*setNumbers;
    //vector que irá armazenar as coordenadas de todos os numeros obtidos da matrix "template"
    int vector[sizeVector];
    for (int i = 0; i < setNumbers; ++i) {
        setPieces(template, board, vector, index);
        index+=2;
    }

    printClassic(board);
    printf("\n");
    printMessage("TYPE IN ANY PARAMETER (Q) QUIT GAME (S) SAVE GAME (H) SHOW HINT");

    while(zeroCount(board) == 0) {
        playX(board, template, vector, sizeVector);
    }

    printMessage("GAME OVER - CONGRATULATIONS");
    printMessage("Press ENTER to leave this game..");
    printMsgLine();
    fflush(stdin);
    getchar();
    fflush(stdin);


    freeMemory(board);
    freeMemory(template);

    initProg();
}

void playX(matrix board, matrix template, int vector[], int sizeVector){
    int line, col, num;
    char lineChar, colChar, numC;
    int hints = 0;

    //recolhe user input (coordenadas & valor a submeter) - o utlizador pode ainda usar Q para sair e S para guardar
    do {
        printf("\n");
        printMessage("NEW MOVE");
        printf("Line: ");
        scanf("%c", &lineChar);
        lineChar = toupper(lineChar);
        fflush(stdin);
        if(lineChar == 'Q') {
            initProg();
            break;
        }
        if(lineChar == 'S'){
            saveGame(board);
            clearConsole();
            printMessage("GAME SAVED");
            printClassic(board);
            continue;
        }
        if(lineChar == 'H'){
            hints++;
            clearConsole();
            showHint(board, template, hints);
            printClassic(board);
            continue;
        }
        line = (int)lineChar - 48;
        printf("Column: ");
        scanf("%c", &colChar);
        colChar = toupper(colChar);
        fflush(stdin);
        if(colChar == 'Q'){
            initProg();
            break;
        }
        if(colChar == 'S') {
            saveGame(board);
            clearConsole();
            printMessage("GAME SAVED");
            printClassic(board);
            continue;
        }
        if(colChar == 'H'){
            hints++;
            clearConsole();
            showHint(board, template, hints);
            printClassic(board);
            continue;
        }
        col = (int)colChar - 48;
        printf("Number: ");
        scanf("%c", &numC);
        numC = toupper(numC);
        fflush(stdin);
        if(numC == 'Q'){
            initProg();
            break;
        }
        if(numC == 'S'){
            saveGame(board);
            clearConsole();
            printMessage("GAME SAVED");
            printClassic(board);
            continue;
        }
        if(numC == 'H'){
            hints++;
            clearConsole();
            showHint(board, template, hints);
            printClassic(board);
            continue;
        }
        num = (int)numC - 48;
        printf("\n");
    }while(line < 1 || line > board.size || col < 1 || col > board.size && num < 1 || num > board.size);

    //verifica se as coordenadas coincidem com um dos numeros dados pelo puzzle
    if(coordinateVerify(vector, sizeVector, line - 1, col - 1) > 0){
        clearConsole();
        printMessage("YOU CANNOT CHANGE THIS LOCATION");
        printClassic(board);
        playClassic(board, template, vector, sizeVector);
    }

    //verifica se a casa escolhida ja foi alterada previamente, se sim pede confirmação para alterar novamente
    if(board.matrix[line - 1][col - 1] != 0){
        char reply = 0;
        printMsgLine();
        clearConsole();
        printClassic(board);
        printf("\n*                (Play - Line: %d Column: %d  Number: %d)                *\n\n", line, col, num);
        printOptionString("ARE YOU SURE YOU WANT TO CHANGE THIS CELL? (y/n)");
        fflush(stdin);
        scanf("%c", &reply);
        fflush(stdin);
        printMsgLine();
        if(reply == 'y')
            board.matrix[line - 1][col - 1] = num;
        else{
            clearConsole();
            printf("\n*            (Play canceled - Line: %d Column: %d  Number: %d)             *\n\n", line, col, num);
            printClassic(board);
            playClassic(board, template, vector, sizeVector);
        }
    }else{board.matrix[line - 1][col - 1] = num;}

    //verifica se jogada é valida - caso não, indica a regra quebrada + alteração efectuada
    clearConsole();
    if(guessVerifyX(board, line - 1, col - 1) == 1) {
        printMsgLine();
        printf("*                   (Play undone - Line: %d Column: %d)                 *\n\n", line, col);
        board.matrix[line - 1][col - 1] = 0;
        printClassic(board);
        playClassic(board, template, vector, sizeVector);
    }
    
    clearConsole();
    printf("\n*           (Previous play - Line: %d Column: %d  Number: %d)            *\n\n", line, col, num);
    printClassic(board);
}

int guessVerifyX(matrix board, int line, int col){
    int count = 0;
    if(lineVerify(board, line, col)) {
        printMsgString("MOVE NOT ALLOWED, CHECK YOUR LINE");
        count++;
    }
    if(colVerify(board, line, col)) {
        printMsgString("MOVE NOT ALLOWED, CHECK YOUR COLUMN");
        count++;
    }
    if(gridVerify(board, line, col)) {
        printMsgString("MOVE NOT ALLOWED, CHECK THE INNER GRID  ");
        count++;
    }
    if(xVerify(board, line, col)){
        printMsgString("MOVE NOT ALLOWED, CHECK THE DIAGONAL  ");
        count++;
    }
    if(count>0)
        return 1;
    return 0;
}

int xVerify(matrix board, int line, int col){
    int x1[][2] = {
            {0,0},
            {1,1},
            {2,2},
            {3,3},
            {4,4},
            {5,5},
            {6,6},
            {7,7},
            {8,8},
    };
    int x2[][2] = {
            {0,8},
            {1,7},
            {2,6},
            {3,5},
            {4,4},
            {5,3},
            {6,2},
            {7,1},
            {8,0},
    };

    int diag1 = 0;
    int diag2 = 0;

    for (int i = 0; i < 9; ++i) {
        if(line == x1[i][0] && col == x1[i][1])
            diag1++;
        if(line == x2[i][0] && col == x2[i][1])
            diag2++;
    }


    int equal = 0;

    if(diag1 > 0){
        for (int k = 0; k < board.size; ++k) {
            if(board.matrix[k][k] == board.matrix[line][col])
                equal++;
        }
    }

    if(diag2 > 0){
        for (int k = 0; k < board.size; ++k) {
            if(board.matrix[k][board.size - 1 - k] == board.matrix[line][col])
                equal++;
        }
    }


    if(equal>1)
        return 1;
    return 0;
}
