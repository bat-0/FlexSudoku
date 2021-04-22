//
// Created by lfb on 17/04/2021.
//

#ifndef FLEXSUDOKU_SUDOKUX_H
#define FLEXSUDOKU_SUDOKUX_H

#include "sudokuClassic.h"


void initXGame(int size, int level);
void playX(matrix board, matrix template, int vector[], int sizeVector);
int xVerify(matrix board, int line, int col);
int guessVerifyX(matrix board, int line, int col);

#endif //FLEXSUDOKU_SUDOKUX_H
