//
// Created by lfb on 09/04/2021.
//

#ifndef SUDOKU_3_SUDOKUCLASSIC_H
#define SUDOKU_3_SUDOKUCLASSIC_H

typedef struct{
    int size;
    int **matrix;
}matrix;

char SUDOKUFILE[30];

void initClassicGame(int size, int level, int load);
void loadClassicGame(int size);

matrix createClassicMatrix(int size);
void clear(matrix game);
void freeMemory(matrix game);
void printClassic(matrix game);

void playClassic(matrix board, matrix template, int vector[], int sizeVector);
int zeroCount(matrix board);


void readGameFile(matrix mtx, const char* filename);

int showHint(matrix board, matrix template, int hintsUsed);

void setPieces(matrix gameFile, matrix newGame, int vector[], int index);
void saveCoordinates(int n, int vector[], int index);

int randomFile();
int randomLocation(int min, int max);

void filePath(char id[]);

int guessVerify(matrix board, int line, int col);
int lineVerify(matrix board, int line, int col);
int colVerify(matrix board, int line, int col);
int gridVerify(matrix board, int line, int col);
int coordinateVerify(int vector[], int sizeVector, int line, int col);


#endif //SUDOKU_3_SUDOKUCLASSIC_H
