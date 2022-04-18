#include "utils.h"
#include "puzzlesolver.h"

int PuzzleSolver::h1_misplaced_tiles(NodePtr node)
{
    int misplaced_counter = 0;
    for (int i = 0; i < Matrix_N; ++i) {
        for (int j = 0; j < Matrix_N; ++j) {
            if (node->matrix[i][j] != target_matrix[i][j]) {
                ++misplaced_counter;
            }
        }
    }
    return misplaced_counter;
}

int PuzzleSolver::h2_manhattan_distance(NodePtr node)
{
    int distance_counter = 0;
    // for each position in current matrix
    for (int row = 0; row < Matrix_N; ++row) {
        for (int col = 0; col < Matrix_N; ++col) {
            int key = node->matrix[row][col];
            int correct_row = -1, correct_col = -1;
            // for each position in target matrix
            // calculate the norm between "right posotion" and "current position"
            bool find = 0;
            for (int i = 0; i < Matrix_N && !find; ++i) {
                for (int j = 0; j < Matrix_N && !find; ++j) {
                    if(key == target_matrix[i][j]){
                        correct_row = i;
                        correct_col = j;
                        find = true; // stop loop
                    }
                }
            }
            distance_counter += abs(row - correct_row) + abs(col - correct_col);
        }
    }
    return distance_counter;
}

int PuzzleSolver::hval(NodePtr node, int (__thiscall PuzzleSolver::*fptr)(Node *))
{
    return (*this.*fptr)(node);
}
