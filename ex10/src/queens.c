#include "../include/queens.h"
#include "stdio.h"
#include "stdlib.h"

int canPut(Cell **table,int size, int row, int column){
    int i, j;

    for(i =1; i < size; i++)
        if(table[i][column].figure == 'Q'){
            return 1;
        }
    
    for(i = row, j = column; i >= 0 && j >=0; i--, j--){
        if(table[i][j].figure == 'Q'){
            return 1;
        }
    }

    for(i = row, j = column; i < size && j < size; i++, j++){
        if(table[i][j].figure == 'Q'){
            return 1;
        }
    }
    
    for(i = row, j = column; i >= 0 && j < size; i--, j++){
        if(table[i][j].figure == 'Q'){
            return 1;
        } 
    }
    
    for(i = row, j = column; i < size && j >= 0; i++, j--){
        if(table[i][j].figure == 'Q'){
            return 1;
        }
    }
   
    return 0;
}

int NQueen(Cell **table,int size, int row){
    if(row >= size){
        exit(0);
    }
    
    for(int i =0; i < size; i++){
        if(!canPut(table, size, row, i)){
            table[row][i].figure = 'Q';

            if(!NQueen(table, size, row + 1)){
                table[row][i].figure = 'E';
            } else
                return 1;
        }
    }

    return 0;
}

void place_queens(Cell **table, int size){
    NQueen(table, size, 1);
}
