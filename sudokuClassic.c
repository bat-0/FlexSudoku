//
// Created by lfb on 09/04/2021.
//

#include "sudokuClassic.h"
#include "menus.h"
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

//inicia um novo jogo, de tamanho 'size' e dificuldade 'level' (novo ou carregado de um ficheiro através de load = 1 ou 0)
void initClassicGame(int size, int level, int load){
    srand(time(NULL));

    //criar a matrix onde se irá jogar
    matrix board = createClassicMatrix(size);
    clear(board);
    //cria a matrix onde se irá carregar os dados do ficheiro contendo a solução
    matrix template = createClassicMatrix(size);
    clear(template); 
    
    int sizeVector;
    int vector[20];

    //determina se o utilizador pretende começar um novo jogo ou recomeçar um jogo gravado, correndo assim a função consoante a escolha
    if(load==0){
        //escolhe aleatoriamente o ficheiro template e importa o caminho para a variavel templateSrc
        switch (size) {
            case 4:
                filePath("41");
                break;
            case 9:
                filePath("91");
                break;
        }

        printMsgLine();
        //importa a matrix guardada no ficheiro txt escolhido aleatoriamente para a matrix "template"
        readGameFile(template, SUDOKUFILE);


        int index = 0;
        //algoritmo que define o numero de iterações para a geração de casas pre-definidas consoante o nível de dificuldade(1, 2) e o tamanho do jogo (4, 9, 16)
        int setNumbers = size * 5 - size * level;
        sizeVector = 2*setNumbers;

        //vector que irá armazenar as coordenadas de todos os numeros obtidos da matrix "template"
        int vector[sizeVector];

        //atribuição de casas pre-definidas na matrix template, na matrix board
        for (int i = 0; i < setNumbers; ++i) {
            setPieces(template, board, vector, index);
            index+=2;
        }
    }else if(load == 1){
        char path[200] = "Saved/";
        char filename[20];
        char ext[10] = ".txt";

        clearConsole();
        printMsgLine();
        printOptionString("FILE TO LOAD: ");
        fgets(filename, sizeof(filename), stdin);

        strcat(path, filename);
        strcat(path, ext);

        FILE * fp;
        fp = fopen(path,"r");

        //verifica se o ficheiro foi carregado, sn volta ao ecra principal
        if (fp == NULL) {
            printf(".... erro a carregar ficheiro\n");
            getchar();
            fflush(stdin);
            initProg();
        }

        //copia os valores encontrados no ficheiro para a matrix "board"
        for (int i = 0; i < board.size; ++i) {
            for (int j = 0; j < board.size; ++j) {
                fscanf(fp,"%d ", &board.matrix[i][j]);
            }
        }

        //dummy vector
        int vector[2] = {99,99};
    }

    printClassic(board);
    printf("\n");
    printMessage("TYPE IN ANY PARAMETER (Q) QUIT GAME (S) SAVE GAME (H) SHOW HINT");

    //implementação da função "jogada", verificando o termino do jogo para a continuação do loop
    while(zeroCount(board) == 0) {
        playClassic(board, template, vector, sizeVector);
    }

    //mensagem de fim jogo
    printMessage("GAME OVER - CONGRATULATIONS");
    printMessage("Press ENTER to leave this game..");
    printMsgLine();
    fflush(stdin);
    getchar();
    fflush(stdin);

    //libertação da memória atribuida as matrizes criadas para o jogo
    freeMemory(board);
    freeMemory(template);

    //regresso ao menu principal
    initProg();

}

//cria um novo objecto (struct matrix)
matrix createClassicMatrix(int size){
    //criar uma matrix de jogo (novo jogo)
    matrix newBoard;

    //alocar memoria à matrix de jogo
    newBoard.size = size;
    newBoard.matrix = (int **)malloc(newBoard.size * sizeof(int *));
    //alocar memória aos vectores da matrix
    for (int i = 0; i < newBoard.size; i++) {
        newBoard.matrix[i] = (int *)malloc(newBoard.size * sizeof(int));
    }

    return newBoard;
}
//liberta a memória alocada aos arrays/matrizes
void freeMemory(matrix game){
    // libertar todos os vector da matrix
    for (int k = 0; k < game.size; ++k) {
        free(game.matrix[k]);
    }
    // libertar o ponteiro
    free(game.matrix);
}

//lê 1 ficheiro de um sudoku resolvido guardado em txt (from-> ./Templates/"randomfile".txt)
void readGameFile(matrix mtx, const char* filename){

    FILE *gamefile;
    gamefile = fopen (filename, "r");
    if (gamefile == NULL)
        printf("unable to open matrix file");

    for (int i = 0; i < mtx.size; i++) {
        for (int j = 0; j < mtx.size; j++)
            fscanf(gamefile, "%d", mtx.matrix[i]+j);
    }

    fclose (gamefile);
}

//limpa as matrizes
void clear(matrix game){
    //iniciar a matrix com todos os seus elementos a 0
    for (int i = 0; i < game.size; i++) {
        for (int j = 0; j < game.size; j++)
            game.matrix[i][j] = 0;
    }
}
//imprime as matrizes na consola
void printClassic(matrix game){
    printf("     ");
    for (int j = 1; j <= game.size; ++j)
        printf("%3d ", j);
    printf("\n     ");
    for (int j = 1; j <= game.size; ++j)
        printf("****");
    printf("\n    |\n");
    for (int y = 0; y < game.size; y++) {
        printf("%3d |", y+1);
        for (int x = 0; x < game.size; x++) {
            printf("%3d ", game.matrix[y][x]);
        }
        printf("\n    |\n");
    }
    printf("     ");
    for (int y = 0; y < game.size; y++)
        printf("****");
    printf("\n");
}

//nova jogada - loop
void playClassic(matrix board, matrix template, int vector[], int sizeVector){
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
    }while(line < 1 || line > board.size || col < 1 || col > board.size || num < 1 || num > board.size);

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
        clearConsole();
        printClassic(board);
        printf("\n*                (Play - Line: %d Column: %d  Number: %d)                *\n\n", line, col, num);
        printOptionString("ARE YOU SURE YOU WANT TO CHANGE THIS CELL? (y/n)");
        fflush(stdin);
        scanf("%c", &reply);
        fflush(stdin);
        printMsgLine();
        if(reply == 'y' || reply == 'Y')
            board.matrix[line - 1][col - 1] = num;
        else{
            clearConsole();
            printf("\n*           (Play canceled - Line: %d Column: %d  Number: %d)            *\n\n", line, col, num);
            printClassic(board);
            playClassic(board, template, vector, sizeVector);
        }
    }else{board.matrix[line - 1][col - 1] = num;}

    //verifica se jogada é valida - caso não, indica a regra quebrada + alteração efectuada
    clearConsole();
    if(guessVerify(board, line - 1, col - 1) == 1) {
        printMsgLine();
        printf("\n*                 (Play undone - Line: %d Column: %d)                   *\n\n", line, col);
        board.matrix[line - 1][col - 1] = 0;
        printClassic(board);
        playClassic(board, template, vector, sizeVector);
    }
    
    clearConsole();
    printf("\n*           (Previous play - Line: %d Column: %d  Number: %d)            *\n\n", line, col, num);
    printClassic(board);
}

//valida se o jogador completou o jogo
int zeroCount(matrix board) {
    int count = 0;
    for (int i = 0; i < board.size; ++i) {
        for (int j = 0; j < board.size; ++j) {
            if (board.matrix[i][j] == 0)
                count++;
        }
    }
    if (count > 0)
        return 0;
    return 1;
}

//cria um novo puzzle - com base no ficheiro escolhido aleatoriamente em readGameFile
void setPieces(matrix gameFile, matrix newGame, int vector[], int index){
    int x = randomLocation(0,gameFile.size);
    int y = randomLocation(0,gameFile.size);
    saveCoordinates(x, vector, index); //colunas
    saveCoordinates(y, vector, index+1); // linhas
    newGame.matrix[y][x] = gameFile.matrix[y][x];
}

//guarda as coordenadas dos indices (setPieces) utilizados para popular o novo puzzle criado - utilizado mais a frente no codigo para prevenir a alteração destes elementos na matrix de jogo
void saveCoordinates(int n, int vector[], int index){
    vector[index] = n;
}

//geradores de valores random
int randomLocation(int min, int max){
    return (rand() % (max - min) + min);
}
int randomFile(){
    return (rand() % (4-1) + 1);
}

//gerador de string contendo path para o txt (alternativa filePath2)
void filePath(char id[]){
    srand(time(NULL));
    char folder[30] = "Templates/";
    char gameID[7];
    char number = randomFile() + 48;
    char fileNr[] = "0";
    fileNr[0] = number;
    char end[5] = ".txt";

    strcpy(gameID, id);
    strcat(gameID, fileNr);
    strcat(folder, gameID);
    strcat(folder, end);


    for (int i = 0; i < strlen(folder); ++i)
        SUDOKUFILE[i] = folder[i];
}

//função que agrupa todas as validações, dando o retorno true/false (1/0)
int guessVerify(matrix board, int line, int col){
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
    if(count>0)
        return 1;
    return 0;
}
//verificação das linhas
int lineVerify(matrix board, int line, int col){
    int equal = 0;

    for (int k = 0; k < board.size; ++k) {
        if(board.matrix[line][k] == board.matrix[line][col])
            equal++;
    }

    if(equal>1)
        return 1;
    return 0;
}
//verificação das colunas
int colVerify(matrix board, int line, int col){
    int equal = 0;

    for (int k = 0; k < board.size; ++k) {
        if(board.matrix[k][col] == board.matrix[line][col])
            equal++;
    }

    if(equal>1)
        return 1;
    return 0;
}
//verificação das grelhas internas
int gridVerify(matrix board, int line, int col) {
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
//verifica se as coordenadas dadas pelo utilizador correspondem a uma casa atribuida automaticamente pelo jogo, barrando o acesso à mesma
int coordinateVerify(int vector[], int sizeVector, int line, int col){
    for (int i = 0; i < sizeVector; i+=2) {
        if(vector[i] == col && vector[i + 1] == line)
            return 1;
    }
    return 0;
}


//mostra a solução a uma das casas ainda definadas a zero("empty")
int showHint(matrix board, matrix template, int hintsUsed) {
    if (hintsUsed > 2) {
        printMessage("YOU HAVE NO HINTS LEFT");
        return 0;
    }

    for (int line = board.size-1; line > 0; --line) {
        for (int col = board.size-1; col > 0; --col) {
            if (board.matrix[line][col] == 0) {
                board.matrix[line][col] = template.matrix[line][col];
                printf("\n*             (Hints %d/2 - Line: %d Column: %d   Number: %d)         *\n\n", hintsUsed, line + 1, col + 1, template.matrix[line][col]);
                return 1;
            }
        }
    }
    return 2;
}