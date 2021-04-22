//
// Created by lfb on 09/04/2021.
//

#include "sudokuHex.h"
#include "sudokuClassic.h"
#include "menus.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <ctype.h>

const char HEXDICT[] = "0123456789ABCDEF";


void initHexGame(int level){
    srand(time(NULL));
    //criar a matrix onde se irá jogar
    cMatrix board = createMatrixHex(16);
    clearHex(board);
    //cria a matrix onde se irá carregar os dados do ficheiro contendo a solução
    cMatrix template = createMatrixHex(16);
    clearHex(template);

    filePath("16");

    printMsgLine();
    //importa a matrix guardada no ficheiro txt escolhido aleatoriamente para a matrix "template"
    readGameFileHex(template, SUDOKUFILE);


    //vector que irá armazenar as coordenadas de todos os numeros obtidos da matrix "template"

    int index = 0;
    int setNumbers = 16 * 5 - 8 * level;
    int sizeVector = 2*setNumbers;
    int vector[sizeVector];
    for (int i = 0; i < setNumbers; ++i) {
        setPiecesHex(template, board, vector, index);
        index+=2;
    }

    printHex(board);
    printf("\n");
    printMessage("TYPE IN ANY PARAMETER (Q) QUIT GAME (S) SAVE GAME (H) SHOW HINT");

    while(zeroCountHex(board) == 0) {
        playHex(board, template, vector, sizeVector);
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
void loadHexGame(){
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


    cMatrix board = createMatrixHex(16);
    clearHex(board);

    /*TODO CORRIGIR LEITURA DE FICHEIRO BINARIO - ERRO: 20475 segmentation fault
    readBinFile(board, path);*/

    //leitura do ficheiro txt (saved game)
    readGameFileHex(board, path);


    //dummy vector
    int vector[2] = {99,99};

    clearConsole();
    printMessage("Game Successfuly Loaded");
    printHex(board);
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

cMatrix createMatrixHex(int size){
    //criar uma matrix de jogo (novo jogo)
    cMatrix newBoard;

    //alocar memoria à matrix de jogo
    newBoard.size = size;
    newBoard.matrix = (char **)malloc(newBoard.size * sizeof(char *));
    //alocar memória aos vectores da matrix
    for (int i = 0; i < newBoard.size; i++) {
        newBoard.matrix[i] = (char *)malloc(newBoard.size * sizeof(char));
    }

    return newBoard;
}
void freeMemoryHex(cMatrix game){
    // libertar todos os vector da matrix
    for (int k = 0; k < game.size; ++k) {
        free(game.matrix[k]);
    }
    // libertar o ponteiro
    free(game.matrix);
}

cMatrix readBinFile(cMatrix mtx, char *path){
    FILE *f = fopen(path,"rb"); //em binario
    if( f == NULL){
        printf("Erro ao abrir o ficheiro");
    }
    fread(&mtx, sizeof(mtx),1,f);
    //ler o tabuleiro
    for (int y = 0; y < mtx.size; ++y) {
        for (int x = 0; x < mtx.size; ++x) {
            fread(&mtx.matrix[y][x], sizeof(mtx.matrix[y][x]),1,f); //ler o tabuleiro
        }
    }
    fclose(f);
    return mtx;
}
void readGameFileHex(cMatrix mtx, const char* filename){

    FILE *gamefile;
    gamefile = fopen (filename, "r");
    if (gamefile == NULL)
        printf("unable to open matrix file");

    for (int i = 0; i < mtx.size; i++) {
        for (int j = 0; j < mtx.size; j++)
            fscanf(gamefile, "%s", mtx.matrix[i]+j);
    }

    fclose (gamefile);
}

void setPiecesHex(cMatrix gameFile, cMatrix newGame, int vector[], int index){
    int x = randomLocation(0,gameFile.size);
    int y = randomLocation(0,gameFile.size);
    saveCoordinates(x, vector, index); //colunas
    saveCoordinates(y, vector, index+1); // linhas
    newGame.matrix[y][x] = gameFile.matrix[y][x];
}

void playHex(cMatrix board, cMatrix template, int vector[], int sizeVector){
    char line, col, num;
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
            printHex(board);
            continue;
        }
        if(line == 'H'){
            hints++;
            clearConsole();
            showHintHex(board, template, hints);
            printHex(board);
            continue;
        }
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
            printHex(board);
            continue;
        }
        if(col == 'H'){
            hints++;
            clearConsole();
            showHintHex(board, template, hints);
            printHex(board);
            continue;
        }
        printf("Number/Letter: ");
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
            printHex(board);
            continue;
        }
        if(num == 'H'){
            hints++;
            clearConsole();
            showHintHex(board, template, hints);
            printHex(board);
            continue;
        }
        printf("\n");
    }while(validateInput(line) || validateInput(col) || validateInput(num));

    int lineINT = convertHex2Dec(line);
    int colINT = convertHex2Dec(col);

    //verifica se as coordenadas coincidem com um dos numeros dados pelo puzzle
    if(coordinateVerify(vector, sizeVector, lineINT, colINT) > 0){
        clearConsole();
        printMessage("YOU CANNOT CHANGE THIS LOCATION");
        printHex(board);
        playHex(board, template, vector, sizeVector);
    }

    //verifica se a casa escolhida ja foi alterada previamente, se sim pede confirmação para alterar novamente
    if(board.matrix[lineINT][colINT] != '_'){
        char reply = 0;
        clearConsole();
        printHex(board);
        printf("\n*                 (Play - Line: %c Column: %c  Value: %c)                *\n\n", line, col, num);
        printOptionString("ARE YOU SURE YOU WANT TO CHANGE THIS CELL? (y/n)");
        fflush(stdin);
        scanf("%c", &reply);
        fflush(stdin);
        printMsgLine();
        if(reply == 'y')
            board.matrix[lineINT][colINT] = num;
        else{
            clearConsole();
            printf("\n*           (Play canceled - Line: %c Column: %c  Number: %c)            *\n\n", line, col, num);
            printHex(board);
            playHex(board, template, vector, sizeVector);
        }
    }else{board.matrix[lineINT][colINT] = num;}

    //verifica se jogada é valida - caso não, indica a regra quebrada + alteração efectuada
    clearConsole();
    if(guessVerifyHex(board, lineINT, colINT) == 1) {
        printMsgLine();
        printf("*                   (Play undone - Line: %c Column: %c)                 *\n\n", line, col);
        board.matrix[lineINT][colINT] = '_';
        printHex(board);
        playHex(board, template, vector, sizeVector);
    }
    
    clearConsole();
    printf("\n*            (Previous play - Line: %c Column: %c  Value: %c)            *\n\n", line, col, num);
    printHex(board);
}

//transforma input Hexadecimal em valores Decimais para correr as funções de verificação (line, col, grid)
int convertHex2Dec (char input){
    char *position = strchr(HEXDICT, input);
    return (int)(position-HEXDICT);
}

//verifica se o input introduzido pelo utilizador esta dentro das respostas aceitaveis [0-9 & A-F]
int validateInput(char c){
    for (int i = 0; i < strlen(HEXDICT); ++i) {
        if(c==HEXDICT[i])
            return 0;
    }
    return 1;
}

//limpa as matrizes
void clearHex(cMatrix game){
    //iniciar a matrix com todos os seus elementos a 0
    for (int i = 0; i < game.size; i++) {
        for (int j = 0; j < game.size; j++)
            game.matrix[i][j] = '_';
    }
}
//imprime as matrizes na consola
void printHex(cMatrix game){
    printf("     ");
    for (int j = 0; j < game.size; ++j)
        printf("%3c ", HEXDICT[j]);
    printf("\n     ");
    for (int j = 1; j <= game.size; ++j)
        printf("****");
    printf("\n    |\n");
    for (int y = 0; y < game.size; y++) {
        printf("%3c |", HEXDICT[y]);
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

//verifica se o jogo terminou
int zeroCountHex(cMatrix board) {
    int count = 0;
    for (int i = 0; i < board.size; ++i) {
        for (int j = 0; j < board.size; ++j) {
            if (board.matrix[i][j] == '_')
                count++;
        }
    }
    if (count > 0)
        return 0;
    return 1;
}

int guessVerifyHex(cMatrix board, int line, int col){
    int count = 0;
    if(lineVerifyHex(board, line, col)) {
        printMsgString("MOVE NOT ALLOWED, CHECK YOUR LINE");
        count++;
    }
    if(colVerifyHex(board, line, col)) {
        printMsgString("MOVE NOT ALLOWED, CHECK YOUR COLUMN");
        count++;
    }
    if(gridVerifyHex(board, line, col)) {
        printMsgString("MOVE NOT ALLOWED, CHECK THE INNER GRID  ");
        count++;
    }
    if(count>0)
        return 1;
    return 0;
}
int lineVerifyHex(cMatrix board, int line, int col){
    int equal = 0;

    for (int k = 0; k < board.size; ++k) {
        if(board.matrix[line][k] == board.matrix[line][col])
            equal++;
    }

    if(equal>1)
        return 1;
    return 0;
}
int colVerifyHex(cMatrix board, int line, int col){
    int equal = 0;

    for (int k = 0; k < board.size; ++k) {
        if(board.matrix[k][col] == board.matrix[line][col])
            equal++;
    }

    if(equal>1)
        return 1;
    return 0;
}
int gridVerifyHex(cMatrix board, int line, int col) {
    int factor = sqrt(board.size);
    int gridX = 0, gridY = 0;
    int equal = 0;

    for (int k = 0; k < board.size; k+=factor) {
        if (line < k + factor) {
            gridX = k;
            break;
        }
    }
    for (int k = 0; k < board.size; k+=factor) {
        if (col < k + factor) {
            gridY = k;
            break;
        }
    }

    for (int i = gridX; i < gridX + factor; ++i) {
        for (int j = gridY; j < gridY + factor; ++j) {
            if (board.matrix[line][col] == board.matrix[i][j])
                equal++;
        }
    }

    if(equal>1)
        return 1;
    return 0;
}
int hintVerifyHex(cMatrix board, int line, int col){
    int count = 0;

    if(lineVerifyHex(board, line, col))
        count++;
    if(colVerifyHex(board, line, col))
        count++;
    if(gridVerifyHex(board, line, col))
        count++;

    if(count > 0)
        return 1;
    return 0;
}

int showHintHex(cMatrix board, cMatrix template, int hintsUsed){
    if (hintsUsed > 5) {
        printMessage("YOU HAVE NO HINTS LEFT");
        return 1;
    }


    for (int line = board.size-1; line >= 0; --line) {
        for (int col = board.size-1; col >= 0; --col) {
            if (board.matrix[line][col] == '_') {
                board.matrix[line][col] = template.matrix[line][col];
                printf("\n");
                printMsgLine();
                printf("*             (Hints %d/5 - Line: %c Column: %c   Number: %c)         *\n", hintsUsed, HEXDICT[line], HEXDICT[col], template.matrix[line][col]);
                printMsgLine();
                return 0;
            }
        }
    }
    return 0;
}