//
// Created by lfb on 19/04/2021.
//

#ifndef FLEXSUDOKU_SUDOKUALPHA_H
#define FLEXSUDOKU_SUDOKUALPHA_H
#include "sudokuHex.h"

void initAlphaGame(int level);
void loadAlphaGame();
void playAlpha(cMatrix board, cMatrix template, int vector[], int sizeVector);
int validateInputAlpha(char c);
int convertHex2DecAlpha (char input);
void printAlpha(cMatrix game);

#endif //FLEXSUDOKU_SUDOKUALPHA_H
