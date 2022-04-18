#include "utils.h"
#include "puzzlesolver.h"
#include "ui_puzzlesolver.h"
#include "random"
#include "iostream"

#include <QRegExpValidator>
#include "QDebug"

void PuzzleSolver::shuffle_matrix(int *matrix, inversion inv)
{
    int shuffled[Matrix_N2] = {};
    for(int i = 0;i < Matrix_N2 - 1;i++){ //range 1-8
        shuffled[i] = i+1;
    }

    while(true){
        for(int i = 0; i < Matrix_N2-1; i++){
            std::swap(shuffled[i], shuffled[rand()%(i+1)]);
        }
        int tmpinv = get_inversion_number(shuffled,Matrix_N2-1);
        if((inv == Even && tmpinv % 2 == 0) || (inv == Odd && tmpinv % 2 != 0))
            break;
    }

    // find a random position and insert 0
    int pos0 = rand()%(Matrix_N2);
    memcpy(matrix,shuffled,sizeof(int)*pos0);
    memset(matrix+pos0,0,sizeof(int));
    memcpy(matrix+pos0+1,shuffled+pos0,sizeof(int)*(Matrix_N2-1-pos0));

    return;
}
