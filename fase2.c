//
// Created by Vítor on 11/12/2019.
//

#include "fase2.h"

int count2 = 0;


int main_fase2() {
    int *row, *col;
    GRID grid = {TAM_N, TAM_N, NULL, NULL};

    char *filename = NULL;



    //read_grid_from_file_binary(filenamebinary, &grid);
    GRID *head = NULL;
    GRID *p = NULL;


    char *path = "C:\\SudokuX\\";

    int numeroSolucoes = 0;
    int *tipoDeTabs = readNumberAndTypeOfSolutions2(&numeroSolucoes);

    //connect_cells(&grid, mat);
    //print_struct_grid(grid);

    switch (TAM_N) {
        case 9:
            filename = "C:/SudokuX/InitGrid9x9.txt";
            break;
        case 16:
            filename = "C:/SudokuX/InitGrid16x16.txt";
            break;
        case 25:
            filename = "C:/SudokuX/InitGri25x25.txt";
            break;
        case 36:
            filename = "C:/SudokuX/InitGrid36x36.txt";
            break;
        default:
            printf("Nao ha ficheiros");
    }

    GRID *grid_por_fazer = NULL;

    grid_por_fazer = read_grid_from_file_f2(filename);

    for(int i=0; i<TAM_N; i++){
        for(int j=0; j<TAM_N; j++){
            cellPossibilities(grid_por_fazer, i, j);
        }
    }

     GRID *grid_solucao = NULL;
     for (int i = 0; i < numeroSolucoes; i++) { // LER TODAS A SOLUÇOES
         //printf("\npeim");
         char numeroDaTab[100];
         sprintf(numeroDaTab, "%d", i + 1);
         char *path2 = concatStrings2(path, numeroDaTab);
         //char *path3 = concatStrings2(path, numeroDaTab);
         path2 = concatStrings2(path2, ".txt");
         //path3 = concatStrings2(path3, ".bin");
         printf("\n");

         grid_solucao = malloc(sizeof(GRID));
         grid_solucao->lines = tipoDeTabs[i];
         grid_solucao->cols = tipoDeTabs[i];
         grid_solucao->pnext_solutions = NULL;
         grid_solucao->pfirst = NULL;
         grid_solucao=read_grid_from_file_f2(path2);
         grid_solucao->pnext_solutions=NULL;

         if (head == NULL) {
             head = grid_solucao;
         } else {
             p = head;
             while (p->pnext_solutions != NULL) {
                 //printf("pei2m");
                 p = p->pnext_solutions;
             }
             p->pnext_solutions = grid_solucao;
         }

     }
     grid_solucao=head;


    GRID *grid_fazer_next = grid_por_fazer;

    while (grid_fazer_next != NULL) { // percorrer todas as matrizes (LISTA LIGADA)

        //print_struct_grid(*grid_fazer_next);


        //singleNumber_f2(grid_fazer_next);
        // hiddenSingleNumber_f2(grid_fazer_next);
        //nakedPair(grid_fazer_next);

        if (SolveSudoku_f2(grid_fazer_next, &row, &col) == true) {
            printf("\n Resolucao:\n");
            print_struct_grid(*grid_fazer_next);

            //concatenar para a nova solucao
            char numeroDaTab[100];
            sprintf(numeroDaTab, "%d", numeroSolucoes + 1);
            char *path2 = concatStrings2(path, numeroDaTab);
            char *path3 = concatStrings2(path, numeroDaTab);

            path2 = concatStrings2(path2, ".txt");
            path3 = concatStrings2(path3, ".bin");
            printf("\n");

            numeroSolucoes++;
            tipoDeTabs[numeroSolucoes - 1] = TAM_N;
            writeFileSoluGrids2(numeroSolucoes, tipoDeTabs);
            printf("%s", path2);
            writeToFile2(grid_fazer_next, path2);

            printf("%s", path3);
            writeToFileBinary(grid_fazer_next, path3);


        } else
            printf("No solution exists");

        grid_fazer_next = grid_fazer_next->pnext_solutions;
    }

    return 0;
}

/**
 * To initialize the grid
 * @param ptr
 */
void init_cell_null(CELL *ptr) {
    ptr->number = 0;
    ptr->line = 0;
    ptr->col = 0;
    ptr->N = NULL;
    ptr->NE = NULL;
    ptr->E = NULL;
    ptr->SE = NULL;
    ptr->S = NULL;
    ptr->SO = NULL;
    ptr->O = NULL;
    ptr->NO = NULL;

    for (int i = 0; i < TAM_N; ++i) {
        ptr->possibilities[i] = 0;
    }
}


/**
 * To print the grid
 * @param g
 */
void print_struct_grid(GRID g) {
    CELL *paux = g.pfirst;
    CELL *pline = g.pfirst;

    for (int i = 0; i < g.lines; i++) {
        for (int j = 0; j < g.cols; j++) {
            printf("%d ", paux->number);
            paux = paux->E;
        }
        printf("\n");
        pline = pline->S;
        paux = pline;
    }
    printf("\nNumero de iteracoes: %d", count2);
    printf("\n\n");
}


/**
 * Takes a partially filled-in grid and attempts to assign values to
all unassigned locations in such a way to meet the requirements
for Sudoku solution (non-duplication across rows, columns, and boxes)
 * @param grid
 * @param row
 * @param col
 * @return
 */
bool SolveSudoku_f2(GRID *grid, int row, int col) {
    CELL *current = grid->pfirst;

    // If there is no unassigned location, we are done
    if (!FindUnassignedLocation_f2(grid, &row, &col))
        return true; // success!


    // consider digits 1 to N
    for (int num = 1; num <= TAM_N; num++) {
        if (count2 % 50000000 == 0) {
            //printGrid(grid);
        }
        count2++;
        //printf("%d",row);
        //printf("%d\n",col);

        //print_struct_grid(*grid);
        // if looks promising
        if (isSafe_f2(grid, row, col, num)) {
            // make tentative assignment
            while (current->line != row) {
                current = current->S;
            }
            while (current->col != col) {
                current = current->E;
            }
            current->number = num;

            // return, if success, yay!
            if (SolveSudoku_f2(grid, &row, &col))
                return true;

            // failure, unmake & try again
            current->number = UNASSIGNED;
        }
    }
    return false; // this triggers backtracking
}

/**
 *  Searches the grid to find an entry that is still unassigned. If
found, the reference parameters row, col will be set the location
that is unassigned, and true is returned. If no unassigned entries
remain, false is returned.
 * @param grid
 * @param row
 * @param col
 * @return
 */
bool FindUnassignedLocation_f2(GRID *grid, int *row, int *col) {
    CELL *current = grid->pfirst;
    CELL *aux = grid->pfirst;

    *row = 0;

    while (*row != TAM_N) {
        *col = 0;

        while (*col != TAM_N) {
            if (current->number == UNASSIGNED) {
                return true;
            }
            (*col)++;
            current = current->E;
        }
        (*row)++;
        aux = aux->S;
        current = aux;
    }

    return false;
}

/**
 * Returns a boolean which indicates whether an assigned entry
in the specified row matches the given number.
 * @param grid
 * @param row
 * @param num
 * @return
 */
bool UsedInRow_f2(GRID *grid, int row, int num) {
    CELL *current = grid->pfirst;

    while (current->line != row) {
        current = current->S;
    }
    while (current->col != TAM_N - 1) {
        if (current->number == num) {
            return true;
        }
        current = current->E;
    }

    return false;
}

/**
 * Returns a boolean which indicates whether an assigned entry
in the specified column matches the given number.
 * @param grid
 * @param col
 * @param num
 * @return
 */
bool UsedInCol_f2(GRID *grid, int col, int num) {
    CELL *current = grid->pfirst;

    while (current->col != col) {
        current = current->E;
    }
    while (current->line != TAM_N - 1) {
        if (current->number == num) {
            return true;
        }
        current = current->S;
    }

    return false;
}

/**
 * Returns a boolean which indicates whether an assigned entry
within the specified 3x3 box matches the given number.
 * @param grid
 * @param boxStartRow
 * @param boxStartCol
 * @param num
 * @return
 */
bool UsedInBox_f2(GRID *grid, int boxStartRow, int boxStartCol, int num) {
    CELL *current = grid->pfirst;
    CELL *aux = grid->pfirst;

    while (current->line != boxStartRow) {

        current = current->S;
    }
    while (current->col != boxStartCol) {
        current = current->E;
    }
    aux = current;

    for (int i = 0; i < (int) sqrt(TAM_N); i++) {
        for (int j = 0; j < (int) sqrt(TAM_N); j++) {
            if (current->number == num) {
                return true;
            }
            current = current->E;
        }
        aux = aux->S;
        current = aux;
    }

    return false;
}

/**
 * Returns a boolean which indicates whether an assigned entry
within the diagonals matches the given number.
 * @param grid
 * @param row
 * @param col
 * @param num
 * @return
 */
bool UsedInDiagonals_f2(GRID *grid, int row, int col, int num) {
    CELL *primary = grid->pfirst;
    CELL *secondary = grid->pfirst;

    if (row == col) {// if(primary->line == primary->col)
        while (primary->line != TAM_N - 1) {
            if (primary->number == num) {
                return true;
            }
            primary = primary->SE;
        }
    }

    if (col == (TAM_N - 1) - row) {// if(secondary->col == (TAM_N - 1) - secondary->line)
        while (secondary->col != TAM_N - 1) {
            secondary = secondary->E;
        }
        while (secondary->line != TAM_N - 1) {
            if (secondary->number == num) {
                return true;
            }
            secondary = secondary->SO;
        }
    }

    return false;
}

/**
 * Returns a boolean which indicates whether it will be legal to assign
num to the given row,col location.
 * @param grid
 * @param row
 * @param col
 * @param num
 * @return
 */
bool isSafe_f2(GRID *grid, int row, int col, int num) {
    /* Check if 'num' is not already placed in current row,
    current column, diagonals and current 3x3 box */
    CELL *current = grid->pfirst;

    while (current->line != row) {

        current = current->S;
    }
    while (current->col != col) {
        current = current->E;
    }

    return !UsedInRow_f2(grid, row, num) &&
           !UsedInCol_f2(grid, col, num) &&
           !UsedInBox_f2(grid, row - row % (int) sqrt(TAM_N), col - col % (int) sqrt(TAM_N), num) &&
           !UsedInDiagonals_f2(grid, row, col, num) &&
           current->number == UNASSIGNED;
}

/**
 *  reads the numeroTabsResolvidas file
 * @param numeroTabs
 * @return
 */
int *readNumberAndTypeOfSolutions2(int *numeroTabs) {
    FILE *fp;
    fp = fopen("C:\\SudokuX\\numeroTabsResolvidas.txt", "r");

    if (fp == NULL) {
        printf("Nao conseguiu abrir/encontrar o ficheiro\n");
        int *tipotabs = malloc(1000 * sizeof(int));
        tipotabs[0] = 0;
        return tipotabs;
    }


    fscanf(fp, "%d", &(*numeroTabs)); //Numero  de tabs (inicio do ficheiro)

    int *tipoTabs = malloc(1000 * sizeof(int)); //Array com o tipo de tabs

    for (int i = 0; i < *numeroTabs; i++) {

        fscanf(fp, "%d", &tipoTabs[i]);

    }
    fclose(fp);

    return tipoTabs;
}


/**
 * For the creation of files
 * @param path
 * @param path2
 * @return
 */
char *concatStrings2(char *path, char *path2) {
    char *result = malloc(strlen(path) + strlen(path2) + 1);
    strcpy(result, path);
    strcat(result, path2);
    return result;
}

/**
 * To read the grids from the files
 * @param filename
 * @return
 */
GRID *read_grid_from_file_f2(char *filename) {
    FILE *fp;
    fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Nao conseguiu abrir/encontrar o ficheiro\n");
        return NULL;
    }

    int ch = NULL;
    int flag = 0;
    GRID *Grid_next = NULL;
    GRID *first_Grid = NULL;

    while (ch != EOF) { // Chegar ao final do ficheiro


        GRID *grid = malloc((sizeof(GRID)));
        grid->cols = TAM_N;
        grid->lines = TAM_N;
        grid->pfirst = NULL;
        grid->pnext_solutions = NULL;
        if (flag == 0) {
            first_Grid = grid;
            Grid_next = grid;
        }
        if (flag != 0) {
            Grid_next->pnext_solutions = grid;
            Grid_next = grid;
        }

        int numero = 0;
        CELL *pline = NULL;//apontador para a primeira celula da linha anterior

        CELL *pprevious_line = NULL;//apontador para a celula da mesma coluna e linha anterior
        CELL *pprevious_col = NULL;//apontador para a celula da mesma linha e coluna anterior

        for (int i = 0; i < grid->lines; i++) {
            for (int j = 0; j < grid->cols; j++) {
                fscanf(fp, "%d", &numero);
                if (grid->pfirst == NULL) {//caso não exista nenhuma célula
                    CELL *pnew = malloc(sizeof(CELL));//aloca memória para uma célula
                    init_cell_null(pnew);//inicializa o pnew
                    pnew->number = numero;//number fica com o valor da matriz na linha i coluna j
                    pnew->line = i;//line fica com o valor de i
                    pnew->col = j;//col fica com o valor de j
                    pprevious_col = pnew;
                    grid->pfirst = pnew;
                } else {
                    if (i == 0) {//caso seja a primeira linha
                        if (j == (grid->cols) - 1) {//caso seja a ultima coluna
                            CELL *pnew = malloc(sizeof(CELL));
                            init_cell_null(pnew);
                            pnew->number = numero;
                            pnew->line = i;
                            pnew->col = j;
                            pnew->O = pprevious_col;
                            pprevious_col->E = pnew;
                            pprevious_line = grid->pfirst;
                            pprevious_col = pline;
                            pline = grid->pfirst;
                        } else {//caso seja uma coluna intermédia
                            CELL *pnew = malloc(sizeof(CELL));
                            init_cell_null(pnew);
                            pnew->number = numero;
                            pnew->line = i;
                            pnew->col = j;
                            pnew->O = pprevious_col;
                            pprevious_col->E = pnew;
                            pprevious_col = pprevious_col->E;//atualização do pprevious_col
                        }
                    } else {//caso seja uma linha intermédia/ultima
                        if (j == 0) {//caso seja a primeira coluna
                            CELL *pnew = malloc(sizeof(CELL));
                            init_cell_null(pnew);
                            pnew->number = numero;
                            pnew->line = i;
                            pnew->col = j;
                            pnew->N = pline;
                            pnew->NE = pline->E;
                            pline->S = pnew;
                            pline->E->SO = pnew;
                            pprevious_col = pnew;//atualização do pprevious_col
                            pprevious_line = pline->E;
                        } else if (j == (grid->cols) - 1) {//caso seja a ultima coluna
                            CELL *pnew = malloc(sizeof(CELL));
                            init_cell_null(pnew);
                            pnew->number = numero;
                            pnew->line = i;
                            pnew->col = j;
                            pnew->N = pprevious_line;
                            pnew->NO = pprevious_line->O;
                            pnew->O = pprevious_col;
                            pprevious_line->S = pnew;
                            pprevious_line->O->SE = pnew;
                            pprevious_col->E = pnew;
                            pline = pline->S;
                            pprevious_col = pline;
                        } else {//caso seja uma coluna intermédia
                            CELL *pnew = malloc(sizeof(CELL));
                            init_cell_null(pnew);
                            pnew->number = numero;
                            pnew->line = i;
                            pnew->col = j;
                            pnew->N = pprevious_line;
                            pnew->NO = pprevious_line->O;
                            pnew->O = pprevious_col;
                            pnew->NE = pprevious_line->E;
                            pprevious_col->E = pnew;
                            pprevious_line->S = pnew;
                            pprevious_line->O->SE = pnew;
                            pprevious_line->E->SO = pnew;
                            pprevious_line = pprevious_line->E;//atualização do pprevious_line
                            pprevious_col = pprevious_col->E;//atualização do pprevious_col
                        }
                    }
                }
            }
        }
        ch = getc(fp);

        flag++;
    }


    fclose(fp);
    return first_Grid;
}


/**
 * To write to a file the solution of a grid
 * @param g
 * @param filename
 */
void writeToFile2(GRID *g, char *filename) {

    FILE *fp = fopen(filename, "w");

    CELL *paux = g->pfirst;
    CELL *pline = g->pfirst;

    for (int i = 0; i < g->lines; i++) {
        for (int j = 0; j < g->cols; j++) {
            fprintf(fp, "%i ", paux->number);
            paux = paux->E;
        }
        fprintf(fp, "\n");
        pline = pline->S;
        paux = pline;
    }

    fclose(fp);
}

/**
 * To write to a binary file the solution of a grid
 * @param g
 * @param filename
 */
void writeToFileBinary(GRID *g, char *filename) {
    FILE *fp = fopen(filename, "wb");

    CELL *paux = g->pfirst;
    CELL *pline = g->pfirst;

    for (int i = 0; i < g->lines; i++) {
        for (int j = 0; j < g->cols; j++) {
            fwrite(&paux->number, sizeof(GRID), 1, fp);
            //fprintf(fp, "%i ", paux->number);
            paux = paux->E;
        }
        //fprintf(fp, "\n");
        pline = pline->S;
        paux = pline;
    }

    fclose(fp);
}

/**
 * Função para ler de ficheiro binario
 * @param filename
 * @param grid
 */
void read_grid_from_file_binary(char *filename, GRID *grid) {
    FILE *fp;
    fp = fopen(filename, "rb");

    if (fp == NULL) {
        printf("Nao conseguiu abrir/encontrar o ficheiro\n");
    }

    int numero = 0;

    CELL *pline = NULL;//apontador para a primeira celula da linha atual
    CELL *pprevious_line = NULL;//apontador para a celula da mesma coluna e linha anterior
    CELL *pprevious_col = NULL;//apontador para a celula da mesma linha e coluna anterior
    for (int i = 0; i < TAM_N; i++) {
        for (int j = 0; j < TAM_N; j++) {
            fread(&numero, sizeof(GRID), 1, fp);
            if (grid->pfirst == NULL) {//caso não exista nenhuma célula
                CELL *pnew = malloc(sizeof(CELL));//aloca memória para uma célula
                init_cell_null(pnew);//inicializa o pnew
                pnew->number = numero;//number fica com o valor da matriz na linha i coluna j
                pnew->line = i;//line fica com o valor de i
                pnew->col = j;//col fica com o valor de j
                pprevious_col = pnew;
                grid->pfirst = pnew;
            } else {
                if (i == 0) {//caso seja a primeira linha
                    if (j == (grid->cols) - 1) {//caso seja a ultima coluna
                        CELL *pnew = malloc(sizeof(CELL));
                        init_cell_null(pnew);
                        pnew->number = numero;
                        pnew->line = i;
                        pnew->col = j;
                        pnew->O = pprevious_col;
                        pprevious_col->E = pnew;
                        pprevious_line = grid->pfirst;
                        pprevious_col = pline;
                        pline = grid->pfirst;
                    } else {//caso seja uma coluna intermédia
                        CELL *pnew = malloc(sizeof(CELL));
                        init_cell_null(pnew);
                        pnew->number = numero;
                        pnew->line = i;
                        pnew->col = j;
                        pnew->O = pprevious_col;
                        pprevious_col->E = pnew;
                        pprevious_col = pprevious_col->E;//atualização do pprevious_col
                    }
                } else {//caso seja uma linha intermédia/ultima
                    if (j == 0) {//caso seja a primeira coluna
                        CELL *pnew = malloc(sizeof(CELL));
                        init_cell_null(pnew);
                        pnew->number = numero;
                        pnew->line = i;
                        pnew->col = j;
                        pnew->N = pline;
                        pnew->NE = pline->E;
                        pline->S = pnew;
                        pline->E->SO = pnew;
                        pprevious_col = pnew;//atualização do pprevious_col
                        pprevious_line = pnew->E;//atualização do pprevious_line
                        //pline = pline->S;//atualização do pline
                        pprevious_line = pline->E;
                    } else if (j == (grid->cols) - 1) {//caso seja a ultima coluna
                        CELL *pnew = malloc(sizeof(CELL));
                        init_cell_null(pnew);
                        pnew->number = numero;
                        pnew->line = i;
                        pnew->col = j;
                        pnew->N = pprevious_line;
                        pnew->NO = pprevious_line->O;
                        pnew->O = pprevious_col;
                        pprevious_line->S = pnew;
                        pprevious_line->O->SE = pnew;
                        pprevious_col->E = pnew;
                        pline = pline->S;
                        pprevious_col = pline;
                    } else {//caso seja uma coluna intermédia
                        CELL *pnew = malloc(sizeof(CELL));
                        init_cell_null(pnew);
                        pnew->number = numero;
                        pnew->line = i;
                        pnew->col = j;
                        pnew->N = pprevious_line;
                        pnew->NO = pprevious_line->O;
                        pnew->O = pprevious_col;
                        pnew->NE = pprevious_line->E;
                        pprevious_col->E = pnew;
                        pprevious_line->S = pnew;
                        pprevious_line->O->SE = pnew;
                        pprevious_line->E->SO = pnew;
                        pprevious_line = pprevious_line->E;//atualização do pprevious_line
                        pprevious_col = pprevious_col->E;//atualização do pprevious_col
                    }
                }
            }
        }
    }

    fclose(fp);

}

/**
 * Puts on the numeroTabsResolvidas the number of solutions that are available
 * and specifies the type of solved grids there are (if they are size 9, 16, 25 or 36)
 * @param numeroTabs
 * @param tipo
 */
void writeFileSoluGrids2(int numeroTabs, int *tipo) {

    FILE *fp = fopen("C:\\SudokuX\\numeroTabsResolvidas.txt", "w");

    fprintf(fp, "%d", numeroTabs); //mete no topo do ficheiro o num de tabs resolvidas tem agora

    for (int j = 0; j < numeroTabs - 1; ++j) {
        fprintf(fp, "\n%i", tipo[j]); //mete no ficheiro o tipo de tabs resolvidas que tem agora
    }

    if (numeroTabs != 0) {
        fprintf(fp, "\n%i", TAM_N);     //para resolver bug que quando insere a primeira solução, nao mete o tipo
                                        // ficava apenas o num de solucao
    }

    fclose(fp);
}


void cellPossibilities(GRID *grid, int row, int col){
    CELL *current = grid->pfirst;

    while(current->line != row){
        current = current->S;
    }
    while(current->col != col){
        current = current->E;
    }
    for(int i=1; i<=TAM_N; i++){
        if(isSafe_f2(grid, row, col, i)){
            current->possibilities[i-1] = i;
        }
    }
}

void singleNumber_f2(GRID *grid){
    int count1=0, num, aux_num=0;
    CELL *current = grid->pfirst;
    CELL *aux = grid->pfirst;

    while(current->S != NULL){
        while(current->E != NULL){
            if(current->number == UNASSIGNED){
                for(num=1; num<=TAM_N; num++){
                    if(isSafe_f2(grid, current->line , current->col ,num)){
                        count1++;
                        if(count1==1){
                            aux_num = num;
                        }
                    }
                }
                if(count1==1){
                    current->number = aux_num;
                }
                count1=0;
            }
            current = current->E;
        }
        aux = aux->S;
        current = aux;
    }
}

void hiddenSingleNumber_f2(GRID *grid){
    CELL *current = grid->pfirst;
    CELL *aux = grid->pfirst;
    //CELL* pos = grid->pfirst;
    //int pos[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    while(current->S != NULL){
        while(current->E != NULL){
            if(current->number == UNASSIGNED){
                for(int i=0; i<TAM_N; i++){
                    if(current->possibilities[i] != UNASSIGNED){
                        if( onlyPossibilityInRow(grid, current->line, current->col, current->possibilities[i]) ||
                            onlyPossibilityInCol(grid, current->line, current->col, current->possibilities[i]) ||
                            onlyPossibilityInBox(grid, current->line - current->line % (int) sqrt(TAM_N), current->col - current->col % (int) sqrt(TAM_N), current->line, current->col, current->possibilities[i]) ||
                            onlyPossibilityInDiagonal(grid, current->line, current->col, current->possibilities[i])){
                            current->number = current->possibilities[i];
                        }
                    }
                }

            }
            current = current->E;
        }
        aux = aux->S;
        current = aux;
    }
}


bool onlyPossibilityInRow(GRID *grid, int row, int col, int num){
    CELL *current = grid->pfirst;
    //int aux = 0;

    while(current->line != row){
        current = current->S;
    }

    while(current->E != NULL){
        if(current->number == UNASSIGNED){
            if(current->col != col){
                if(current->possibilities[num-1] == num){
                    return false;
                }
            }
        }
        current = current->E;
    }
    return true;
}

bool onlyPossibilityInCol(GRID *grid, int row, int col, int num){
    CELL *current = grid->pfirst;
    //int aux = 0;

    while(current->col != col){
        current = current->E;
    }

    while(current->S != NULL){
        if(current->number == UNASSIGNED){
            if(current->line != row){
                if(current->possibilities[num-1] == num){
                    return false;
                }
            }
        }
        current = current->S;
    }
    return true;
}

bool onlyPossibilityInBox(GRID *grid, int boxStartRow, int boxStartCol, int row, int col, int num){
    CELL *current = grid->pfirst;
    CELL *aux = grid->pfirst;

    while (current->line != boxStartRow) {
        current = current->S;
    }
    while (current->col != boxStartCol) {
        current = current->E;
    }
    aux = current;

    for (int i = 0; i < (int)sqrt(TAM_N); i++){
        for (int j = 0; j < (int)sqrt(TAM_N); j++){
            if (current->line != row || current->col != col) {
                if(current->possibilities[num-1] == num){
                    return false;
                }
            }
            current = current->E;
        }
        aux = aux->S;
        current = aux;
    }

    return true;
}

bool onlyPossibilityInDiagonal(GRID *grid, int row, int col, int num){
    CELL *primary = grid->pfirst;
    CELL *secondary = grid->pfirst;

    if (row == col) {
        while (primary->S != NULL) {
            if (primary->line != row) {
                if(primary->possibilities[num-1] == num){
                    return false;
                }
            }
            primary = primary->SE;
        }
    }

    if (col == (TAM_N - 1) - row) {
        while (secondary->E != NULL) {
            secondary = secondary->E;
        }
        while (secondary->S != NULL) {
            if (secondary->line != row) {
                if(secondary->possibilities[num-1] == num){
                    return false;
                }
            }
            secondary = secondary->SO;
        }
    }

    return true;
}

void nakedPair(GRID *grid){
    CELL *current = grid->pfirst;
    CELL *aux = grid->pfirst;
    int count = 0;

    while(current->S != NULL){
        while(current->E != NULL){
            if(current->number == UNASSIGNED){
                for(int i=0; i<TAM_N; i++){
                    if(current->possibilities[i] != UNASSIGNED){
                        count++;
                    }
                }
                if(count == 2){

                }
            }
            current = current->E;
        }
        aux = aux->S;
        current = aux;
    }
}

bool nakedPairRow(GRID *grid, int row, int col, int num1, int num2){
    CELL *current = grid->pfirst;
    int count=0, flag=0;

    while(current->line != row){
        current = current->S;
    }

    while(current->E != NULL){
        if(current->number == UNASSIGNED){
            if(current->col != col){
                for(int i=0; i<TAM_N; i++){
                    if(current->possibilities[i] != UNASSIGNED){
                        count++;
                    }
                }
                if(count == 2 && current->possibilities[num1 - 1] == num1 && current->possibilities[num2 - 1] == num2){
                    //return true;
                    flag = 1;
                }
                if( flag == 1 && ((current->possibilities[num1 - 1] == num1 && current->possibilities[num2 - 1] == UNASSIGNED) ||
                                  (current->possibilities[num1 - 1] == UNASSIGNED && current->possibilities [num2 - 1] == num2) ||
                                  (count != 2 && current->possibilities[num1 - 1] == num1 && current->possibilities[num2 - 1] == UNASSIGNED))){
                    current->possibilities[num1 - 1] = UNASSIGNED;
                    current->possibilities[num2 - 1] = UNASSIGNED;
                    flag = 0;
                }
            }
        }
        current = current->E;
    }
    //return false;
}

bool nakedPairCol(GRID *grid, int row, int col, int num1, int num2){
    CELL *current = grid->pfirst;
    int count=0, flag=0;

    while(current->col != col){
        current = current->E;
    }

    while(current->S != NULL){
        if(current->number == UNASSIGNED){
            if(current->line != row){
                for(int i=0; i<TAM_N; i++){
                    if(current->possibilities[i] != UNASSIGNED){
                        count++;
                    }
                }
                if(count == 2 && current->possibilities[num1 - 1] == num1 && current->possibilities[num2 - 1] == num2){
                    //return true;
                    flag = 1;
                }
                if( flag == 1 && ((current->possibilities[num1 - 1] == num1 && current->possibilities[num2 - 1] == UNASSIGNED) ||
                                  (current->possibilities[num1 - 1] == UNASSIGNED && current->possibilities [num2 - 1] == num2) ||
                                  (count != 2 && current->possibilities[num1 - 1] == num1 && current->possibilities[num2 - 1] == UNASSIGNED))){
                    current->possibilities[num1 - 1] = UNASSIGNED;
                    current->possibilities[num2 - 1] = UNASSIGNED;
                    flag = 0;
                }
            }
        }
        current = current->S;
    }
    //return false;
}

bool nakedPairBox(GRID *grid, int boxStartRow, int boxStartCol, int row, int col, int num1, int num2){
    CELL *current = grid->pfirst;
    CELL *aux = grid->pfirst;
    int count=0;

    while (current->line != boxStartRow) {
        current = current->S;
    }
    while (current->col != boxStartCol) {
        current = current->E;
    }
    aux = current;

    for (int i = 0; i < (int)sqrt(TAM_N); i++){
        for (int j = 0; j < (int)sqrt(TAM_N); j++){
            if (current->line != row || current->col != col) {
                for(int k=0; k<TAM_N; k++){
                    if(current->possibilities[k] != UNASSIGNED){
                        count++;
                    }
                }
                if(count == 2 && current->possibilities[num1 - 1] == num1 && current->possibilities[num2 - 1] == num2){
                    return true;
                }
            }
            current = current->E;
        }
        aux = aux->S;
        current = aux;
    }
    return false;
}

bool nakedPairDiagonals(GRID *grid, int row, int col, int num1, int num2){
    CELL *primary = grid->pfirst;
    CELL *secondary = grid->pfirst;
    int count=0;

    if (row == col) {
        while (primary->S != NULL) {
            if (primary->line != row) {
                for(int i=0; i<TAM_N; i++){
                    if(primary->possibilities[i] != UNASSIGNED){
                        count++;
                    }
                }
                if(count == 2 && primary->possibilities[num1 - 1] == num1 && primary->possibilities[num2 - 1] == num2){
                    return true;
                }
            }
            primary = primary->SE;
        }
    }

    if (col == (TAM_N - 1) - row) {
        while (secondary->E != NULL) {
            secondary = secondary->E;
        }
        while (secondary->S != NULL) {
            if (secondary->line != row) {
                for(int j=0; j<TAM_N; j++){
                    if(secondary->possibilities[j] != UNASSIGNED){
                        count++;
                    }
                }
                if(count == 2 && secondary->possibilities[num1 - 1] == num1 && secondary->possibilities[num2 - 1] == num2){
                    return true;
                }
            }
            secondary = secondary->SO;
        }
    }
    return false;
}


