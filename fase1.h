//
// Created by Vítor on 31/10/2019.
//

#ifndef SUDOKUX_FASE1_H
#define SUDOKUX_FASE1_H

// A Backtracking program in C to solve Sudoku problem
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include<conio.h>


// UNASSIGNED is used for empty cells in sudoku grid
#define UNASSIGNED 0

// N is used for the size of Sudoku grid. Size will be NxN
#define TAM_N 9

#define RESET 0

//-------------------------BRUTE FORCE--------------------------------
bool SolveSudoku(int **, int row, int col);
bool FindUnassignedLocation(int**, int *row, int *col);
bool UsedInRow(int**, int row, int num);
bool UsedInCol(int**, int col, int num);
bool UsedInBox(int **, int boxStartRow, int boxStartCol, int num);
bool UsedInDiagonals(int**, int row, int col, int num);
bool isSafe(int**, int row, int col, int num);

//------------------------------------RANDOM GRID--------------------------
int** fillGridRandom(int **,int row, int col, int num_values);
int countAssigned(int **);

//--------------------------------PRINT-----------------------------
void printGrid(int**);

//------------------------READ FROM FILE AND WRITE TO FILE--------------------
int **getGridFromFile(char *filename, int lines, int columns, int **mat);
void writeToFile(int **grid, char *filename);


int** createMatrix();
void initGridWithZero(int ** , int );
void freeMemory(int ** max);

int compareWithSolvedGrids(int** SolvedMat, int** UnsolvedMat);

void writeFileSoluGrids(int numeroTabs, int *tipo);
char* concatStrings(char* path ,char* path2);
int * readNumberAndTypeOfSolutions(int *numeroTabs);

//------------------------STRATEGIES-----------------------------------
int** singleNumber(int **grid);
int** hiddenSingleNumber(int **grid);

//---------------------CHECK CONSISTENCY-----------------------------------
bool numberExistsAgainInRow(int **grid, int row, int col, int num);
bool numberExistsAgainInCol(int **grid, int row, int col, int num);
bool numberExistsAgainInBox(int **grid, int row, int col,  int num);
bool numberExistsAgainInDiagonals(int **grid, int row, int col, int num);
bool check_consistency(int **grid);



//--------------------------------MAIN--------------------------------------
int mainSudokuX();
#endif //SUDOKUX_FASE1_H
