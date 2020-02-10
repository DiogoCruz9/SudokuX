//
// Created by Vítor on 11/12/2019.
//

#ifndef SUDOKUX_FASE2_H
#define SUDOKUX_FASE2_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define UNASSIGNED 0
#define TAM_N 9

typedef struct cell{
    int number, line, col;
    struct cell *N, *NE, *E, *SE, *S, *SO, *O, *NO;
    int possibilities[TAM_N];
}CELL;

typedef struct grid{
    int lines, cols;
    CELL *pfirst;
    struct grid *pnext_solutions;
}GRID;

typedef struct solvedgrids{ //queue
    GRID* pfirst;
}SOLVEDGRIDS;

int main_fase2();
void connect_cells(GRID *grid, int **mat);
//void connect_cells(GRID *grid, int mat[][TAM_N]);
void init_cell_null(CELL *ptr);
void print_struct_grid(GRID g);
GRID* read_grid_from_file_f2(char *filename);
void writeToFile2(GRID *g, char *filename);
void writeToFileBinary(GRID *g, char *filename);
void read_grid_from_file_binary(char *filename,GRID *grid);
void writeFileSoluGrids2(int numeroTabs, int *tipo);
int* readNumberAndTypeOfSolutions2(int *numeroTabs);
char* concatStrings2(char* path ,char* path2);
bool UsedInBox_f2(GRID *grid, int boxStartRow, int boxStartCol, int num);
bool isSafe_f2(GRID *grid, int row, int col, int num);
bool UsedInCol_f2(GRID *grid, int col, int num);
bool UsedInRow_f2(GRID *grid, int row, int num);
bool FindUnassignedLocation_f2(GRID *grid,int *row, int *col);
bool SolveSudoku_f2(GRID *grid, int row, int col);
bool UsedInDiagonals_f2(GRID *grid, int row, int col, int num);
GRID *read_grids_from_file(char *filename);


void cellPossibilities(GRID *grid, int row, int col);
void singleNumber_f2(GRID *grid);
void hiddenSingleNumber_f2(GRID *grid);
bool onlyPossibilityInRow(GRID *grid, int row, int col, int num);
bool onlyPossibilityInCol(GRID *grid, int row, int col, int num);
bool onlyPossibilityInBox(GRID *grid, int boxStartRow, int boxStartCol, int row, int col, int num);
bool onlyPossibilityInDiagonal(GRID *grid, int row, int col, int num);
void nakedPair(GRID *grid);
bool nakedPairRow(GRID *grid, int row, int col, int num1, int num2);
bool nakedPairDiagonals(GRID *grid, int row, int col, int num1, int num2);
bool nakedPairBox(GRID *grid, int boxStartRow, int boxStartCol, int row, int col, int num1, int num2);
bool nakedPairCol(GRID *grid, int row, int col, int num1, int num2);



#endif //SUDOKUX_FASE2_H
