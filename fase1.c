//
// Created by Vítor on 31/10/2019.
//

#include "fase1.h"

int count=0;

/* Driver Program to test above functions */
int mainSudokuX()
{
    int *row, *col;
    int** grid=createMatrix();
    int** gridporresolver;  //matrix random (está a alocar mem dentro da funcao)
    int countnumberoftries=0;

    srand(time(NULL));

//----------------------------------Matriz fiLe----------------------------------------------

    char* path="C:\\SudokuX\\";

    char *inputFile = "C:/SudokuX/InitGrid9x9.txt";
    //char *inputFile = "C:/SudokuX/InitGrid16x16.txt";
    //char *inputFile = "C:/SudokuX/InitGrid25x25.txt";
    //char *inputFile = "C:/SudokuX/InitGrid36x36.txt";
    getGridFromFile(inputFile, TAM_N, TAM_N, grid);


    //ir buscar ao txt numeroTabsResolvidas o numero de solucoes e os respetivos tipos
    int numeroSolucoes=0;
    int *tipoDeTabs=readNumberAndTypeOfSolutions(&numeroSolucoes);

    //verificar se ja foram verificadas todas as solucoes
    int flag=0;

    if(check_consistency(grid) == true)
    {
        printf("The grid verifies all the conditions!\n");
    }
    else if(check_consistency(grid) == false){
        printf("The grid violates the conditions necessary for the sudokuX!\n");
    }

    for(int i=0;i<numeroSolucoes;i++) {

        //Concatenar
        char numeroDaTab[100];
        sprintf(numeroDaTab, "%d", i + 1);
        char *path2 = concatStrings(path, numeroDaTab);
        path2 = concatStrings(path2, ".txt");

        //verificar se ja existe a solucao
        int **resolvida = createMatrix();
        getGridFromFile(path2, TAM_N, TAM_N, resolvida);

        if (tipoDeTabs[i] == TAM_N && compareWithSolvedGrids(resolvida, grid) == 0) {
            grid = resolvida;
            printf("\nSolucao Ja em Memoria\n");
            printGrid(grid);
        } else {
            flag++;
        }
    }

    // caso flag seja igual ao numeroSolucoes significa que nao existe resolucoes nas solucoes em txt
    if(flag== numeroSolucoes){
        grid = singleNumber(grid);
        //grid = hiddenSingleNumber(grid);
        if (SolveSudoku(grid, &row, &col) == true) {
            printf("\n Resolucao");
            printGrid(grid);

            //concatenar para a nova solucao
            char numeroDaTab[100];
            sprintf(numeroDaTab, "%d", numeroSolucoes+1);
            char *path2 =concatStrings(path ,numeroDaTab);
            path2=concatStrings(path2,".txt");

            numeroSolucoes++;

            writeFileSoluGrids(numeroSolucoes,tipoDeTabs);
            printf("%s", path2);
            writeToFile(grid, path2);
    }else{
            printf("No solution exists");
        }
    }




//----------------------------------Matriz Random----------------------------------------------
/*
    //ciclo infinito ate gerar uma matriz que tenha solucao
    while(true){
        printf("%d",countnumberoftries);
        countnumberoftries++;

        gridporresolver=fillGridRandom(grid,row, col, 10);
        if(gridporresolver!=NULL) {
            printf("%d",countnumberoftries);
            printf("\n\tMatriz Encontrada");
            printGrid(gridporresolver);
            printGrid(grid);
            break;
        }
        freeMemory(grid);
        grid=createMatrix();
    }

*/
    return 0;
}

/* Concatenates the 2 paths */
char* concatStrings(char* path ,char* path2)
{
    char *result = malloc(strlen(path) + strlen(path2) + 1);
    strcpy(result, path);
    strcat(result, path2);
    return result;

}

/* Reads from the numeroTabsResolvidas the number of solutions there are and of which type */
int* readNumberAndTypeOfSolutions(int *numeroTabs)
{
    FILE *fp;
    fp = fopen("C:\\SudokuX\\numeroTabsResolvidas.txt", "r");

    if (fp == NULL) {
        printf("Nao conseguiu abrir/encontrar o ficheiro\n");
        return NULL;
    }


    fscanf(fp, "%d", &(*numeroTabs)); //Numero  de tabs (inicio do ficheiro)

    int *tipoTabs = malloc(*numeroTabs * sizeof(int )); //Array com o tipo de tabs

    for (int i = 0; i < *numeroTabs; i++) {

        fscanf(fp, "%d", &tipoTabs[i]);

    }

    fclose(fp);

    return tipoTabs;
}

/* Puts on the numeroTabsResolvidas the number of solutions that are available
 * and specifies the type of solved grids there are (if they are size 9, 16, 25 or 36) */
void writeFileSoluGrids(int numeroTabs, int *tipo)
{

    FILE *fp = fopen("C:\\SudokuX\\numeroTabsResolvidas.txt", "w");

    fprintf(fp, "%d",numeroTabs);

    for (int j = 0; j < numeroTabs-1 ; ++j) {

        fprintf(fp, "\n%i", tipo[j]);

    }

    fprintf(fp, "\n%i", TAM_N);

    fclose(fp);
}

/* Function that compares the filled positions of an initial sudoku with a solved one
 * and returns true if they're the same */
int compareWithSolvedGrids(int** solvedMat, int** unsolvedMat){

    for(int i=0; i<TAM_N;i++){
        for(int b=0; b<TAM_N;b++){
            if(unsolvedMat[i][b]!=0){
                if(unsolvedMat[i][b]!=solvedMat[i][b]){
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* Frees memory */
void freeMemory(int ** max){
    for(int i=0;i<TAM_N;i++){
        free(max[i]);
    }
}

/* Takes a partially filled-in grid and attempts to assign values to
all unassigned locations in such a way to meet the requirements
for Sudoku solution (non-duplication across rows, columns, and boxes) */
bool SolveSudoku(int **grid, int row, int col)
{
    // If there is no unassigned location, we are done
    if (!FindUnassignedLocation(grid, &row, &col))
        return true; // success!


    // consider digits 1 to N
    for (int num = 1; num <= TAM_N; num++){
        if(count%50000000==0){
            //printGrid(grid);
        }
        count++;

        // if looks promising
        if (isSafe(grid, row, col, num))
        {
            // make tentative assignment
            grid[row][col] = num;

            // return, if success, yay!
            if (SolveSudoku(grid, &row, &col))
                return true;

            // failure, unmake & try again
            grid[row][col] = UNASSIGNED;
        }
    }
    return false; // this triggers backtracking
}

/*When creating a grid, put all the positions in 0*/
void initGridWithZero(int ** matrix, int tipo){
    for(int i=0;i< tipo;i++){
        for(int b=0;b< tipo;b++){
            matrix[i][b]=0;
        }
    }
}

/*Allocates memory for the grid with the size we want*/
int** createMatrix(){
    int **grid = malloc(TAM_N * sizeof(int *));

    for (int i = 0; i < TAM_N; ++i) {
        grid[i] = malloc(TAM_N * sizeof(int));
    }

    initGridWithZero(grid, TAM_N);
    return grid;

}

/* Searches the grid to find an entry that is still unassigned. If
found, the reference parameters row, col will be set the location
that is unassigned, and true is returned. If no unassigned entries
remain, false is returned. */
bool FindUnassignedLocation(int** grid, int *row, int *col){


    for (*row = 0; *row < TAM_N; (*row)++){
        for (*col = 0; *col < TAM_N; (*col)++){
            if (grid[*row][*col] == UNASSIGNED){
                return true; // if 0 found
            }
        }
    }
    return false;
}

/* Returns a boolean which indicates whether an assigned entry
in the specified row matches the given number. */
bool UsedInRow(int** grid, int row, int num)
{
    for (int col = 0; col < TAM_N; col++)
        if (grid[row][col] == num)
            return true;
    return false;
}

/* Returns a boolean which indicates whether an assigned entry
in the specified column matches the given number. */
bool UsedInCol(int** grid, int col, int num)
{
    for (int row = 0; row < TAM_N; row++)
        if (grid[row][col] == num)
            return true;
    return false;
}

/* Returns a boolean which indicates whether an assigned entry
within the specified 3x3 box matches the given number. */
bool UsedInBox(int** grid, int boxStartRow, int boxStartCol, int num)
{
    for (int row = 0; row < (int)sqrt(TAM_N); row++)
        for (int col = 0; col < (int)sqrt(TAM_N); col++)
            if (grid[row+boxStartRow][col+boxStartCol] == num)
                return true;
    return false;
}

/* Returns a boolean which indicates whether an assigned entry
within the diagonals matches the given number. */
bool UsedInDiagonals(int** grid, int row, int col, int num){
    if(row == col){
        for(int i=0; i<TAM_N; i++){
            if(grid[i][i] == num){
                return true;
            }
        }
    }
    if(col == (TAM_N-1)-row){
        for(int j=0; j<TAM_N; j++){
            if(grid[j][(TAM_N-1)-j] == num){
                return true;
            }
        }
    }
    return false;
}

/* Returns a boolean which indicates whether it will be legal to assign
num to the given row,col location. */
bool isSafe(int** grid, int row, int col, int num)
{
    /* Check if 'num' is not already placed in current row,
    current column, diagonals and current 3x3 box */
    return !UsedInRow(grid, row, num) &&
           !UsedInCol(grid, col, num) &&
           !UsedInBox(grid, row - row % (int)sqrt(TAM_N) , col - col % (int)sqrt(TAM_N), num) &&
           !UsedInDiagonals(grid, row, col, num) &&
           grid[row][col]==UNASSIGNED;
}

/* Creates a matrix and inserts a desired amount of random values in random rows and cols if they are safe to insert there
   then tries to solve that grid */
int** fillGridRandom(int ** grid,int row, int col, int num_values){
    int num, l, c;

    int** gridPorResolver= createMatrix();

    while(countAssigned(grid) < num_values) {//Makes sure we get the amount of values asked for on the starting grid
        l = rand() % TAM_N;
        c = rand() % TAM_N;
        if (grid[l][c] == UNASSIGNED) {//We can insert a value in this position since it is unassigned
            num = rand() % TAM_N + 1;
            if (isSafe(grid, l, c, num)) {
                grid[l][c] = num;
                gridPorResolver[l][c]=num;
                //sao criadas 2 copias da grid onde 1 vai ser depois resolvida abaixo no solvegrid
                //e a outra (gridporresolver) nao vai ser resolvida, somente vai ser gerada
            }
        }
    }

    if (SolveSudoku(grid, &row, &col) == true){
        //grid é a solucao
        return gridPorResolver;
    }
    else
        return NULL;
}

/* Counts the positions in the grid that have been assigned a value */
int countAssigned(int** grid){
    int count=0;

    for(int i=0; i<TAM_N; i++){
        for(int j=0; j<TAM_N; j++){
            if(grid[i][j] != 0){
                count++;
            }
        }
    }
    return count;
}

/* A utility function to print grid */
void printGrid(int** grid){

    printf("\n\n");

    for (int row = 0; row < TAM_N; row++)
    {
        for (int col = 0; col < TAM_N; col++)

            printf("%d ", grid[row][col]);
        printf("\n");
    }
    printf("\nNumero de iteracoes: %d", count);

    printf("\n\n");
}

/* Reads the initial sudokuX grid from the file and stores it in memory */
int **getGridFromFile(char *filename, int lines, int columns, int **mat) {

    FILE *fp;
    fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Nao conseguiu abrir/encontrar o ficheiro\n");
        return NULL;
    }

    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < columns; j++) {
            fscanf(fp, "%d", &mat[i][j]);
        }
    }

    fclose(fp);
    return mat;
}

/* Writes the solved sudokuX grid to the desired txt file */
void writeToFile(int **grid, char *filename){

    int i;
    FILE *fp = fopen(filename, "w");

    for(i = 0; i<TAM_N; i++) {
        if(i!=0){
            fprintf(fp, "\n");
        }

        for (int j = 0; j < TAM_N ; ++j) {
            if(j!=TAM_N-1){
                fprintf(fp, "%i ", grid[i][j]);
            }
            else{
                fprintf(fp, "%i", grid[i][j]);
            }
        }
    }

    printf("\n");
    fclose(fp);
}

/* Checks if the same number exists more than 1 time in the row */
bool numberExistsAgainInRow(int **grid, int row, int col, int num){
    for(int i=0; i<TAM_N; i++){
        if(i == col){
            continue;
        }
        if(grid[row][i] == num){
            return true;
        }
    }
    return false;
}

/* Checks if the same number exists more than 1 time in the column */
bool numberExistsAgainInCol(int **grid, int row, int col, int num){
    for(int i=0; i<TAM_N; i++){
        if(i == row){
            continue;
        }
        if(grid[i][col] == num){
            return true;
        }
    }
    return false;
}

/* Checks if the same number exists more than 1 time in the box */
bool numberExistsAgainInBox(int **grid, int row, int col,  int num){
    int boxStartRow = row - row % (int)sqrt(TAM_N);
    int boxStartCol = col - col % (int)sqrt(TAM_N);

    for(int i=0; i < (int)sqrt(TAM_N); i++){
        for(int j=0; j < (int)sqrt(TAM_N); j++){
            if(i+boxStartRow == row && j+boxStartCol == col){
                continue;
            }
            if(grid[i+boxStartRow][j+boxStartCol] == num){
                return true;
            }
        }
    }
    return false;
}

/* Checks if the same number exists more than 1 time in the diagonals */
bool numberExistsAgainInDiagonals(int **grid, int row, int col, int num){
    if(row == col){
        for(int i=0; i<TAM_N; i++){
            if(i == row){
                continue;
            }
            if(grid[i][i] == num){
                return true;
            }
        }
    }
    if(col == (TAM_N-1)-row){
        for(int j=0; j<TAM_N; j++){
            if(j == row){
                continue;
            }
            if(grid[j][(TAM_N-1)-j] == num){
                return true;
            }
        }
    }
    return false;
}

/* Verifies the numberExistsAgainInRow, numberExistsAgainInCol, numberExistsAgainInBox and numberExistsAgainInDiagonals
 *  functions for all numbers that aren't unassigned */
bool check_consistency(int **grid){
    for(int i=0; i<TAM_N; i++){
        for(int j=0; j<TAM_N; j++){
            if(grid[i][j] != UNASSIGNED){
                if( numberExistsAgainInRow(grid, i, j, grid[i][j]) ||
                    numberExistsAgainInCol(grid, i, j, grid[i][j]) ||
                    numberExistsAgainInBox(grid, i, j, grid[i][j]) ||
                    numberExistsAgainInDiagonals(grid, i, j, grid[i][j])){

                    return false;
                }
            }
        }
    }
    return true;
}

int** singleNumber(int **grid){
    int count1=0, num, aux=0;

    for(int i=0; i<TAM_N; i++){
        for(int j=0; j<TAM_N; j++){
            if(grid[i][j] == UNASSIGNED){
                for(num=1; num<=TAM_N; num++){
                    if(isSafe(grid, i, j, num)){
                        count1++;
                        if(count1 == 1){
                            aux = num;
                        }
                    }
                }
                if(count1==1){
                    grid[i][j] = aux;
                }
                count1=0;
            }
        }
    }
    return grid;
}

int** hiddenSingleNumber(int **grid){
    int num, row=0, col=0, aux, countRow=0, countCol=0, countBox=0, countDiag=0;

    if(grid[row][col] == UNASSIGNED){
        for(num=1; num<=TAM_N; num++){
            if(isSafe(grid, row, col, num)){
                for(int i=0; i<TAM_N; i++){ //Checks if its the only possibility in cols of that row
                    if(grid[row][i] == UNASSIGNED){
                        if(isSafe(grid, row, i, num)){
                            countRow++;
                        }
                        if(countRow == 0){
                            aux = num;
                        }
                    }
                }
                if(countRow == 0){
                    grid[row][col] = aux;
                }
                for(int j=0; j<TAM_N; j++) {//Checks if its the only possibility in rows of that col
                    if(grid[j][col] == UNASSIGNED){
                        if(isSafe(grid, j, col, num)){
                            countCol++;
                        }
                        if(countCol == 0){
                            aux = num;
                        }
                    }
                }
                if(countCol == 0){
                    grid[row][col] = aux;
                }
            }
        }
    }
    return grid;
}