//
// Created by lfb on 09/04/2021.
//

#ifndef SUDOKU_3_SUDOKUHEX_H
#define SUDOKU_3_SUDOKUHEX_H



typedef struct{
    int size;
    char **matrix;
}cMatrix;


void initHexGame(int level);
void loadHexGame();

cMatrix createMatrixHex(int size);
void freeMemoryHex(cMatrix game);

void clearHex(cMatrix game);
void printHex(cMatrix game);

cMatrix readBinFile(cMatrix mtx, char *path);
void readGameFileHex(cMatrix mtx, const char* filename);

void playHex(cMatrix board, cMatrix template, int vector[], int sizeVector);

int convertHex2Dec (char input);
int validateInput(char c);

int zeroCountHex(cMatrix board);

void setPiecesHex(cMatrix gameFile, cMatrix newGame, int vector[], int index);

int guessVerifyHex(cMatrix board, int line, int col);
int lineVerifyHex(cMatrix board, int line, int col);
int colVerifyHex(cMatrix board, int line, int col);
int gridVerifyHex(cMatrix board, int line, int col);

int showHintHex(cMatrix board, cMatrix template, int hintsUsed);

#endif //SUDOKU_3_SUDOKUHEX_H
