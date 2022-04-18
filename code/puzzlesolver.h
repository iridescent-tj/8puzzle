#ifndef PUZZLESOLVER_H
#define PUZZLESOLVER_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QLabel>
#include <QElapsedTimer>
#include <iostream>
#include "utils.h"

enum direction{Up,Down,Left,Right};
enum inversion{Odd,Even};


namespace Ui {
class PuzzleSolver;
}

class PuzzleSolver : public QWidget
{
    Q_OBJECT

private:
    // Termination status (saved as a matrix
    int target_matrix[Matrix_N][Matrix_N];

    // The open table saves all nodes that have been generated but not investigated
    // The closed table records the visited nodes
    NodeListPtr open = NULL, closed = NULL;

    // Root of a search tree
    STree root = NULL;

    // Pointer to different heuristic functions
    int (PuzzleSolver::*h_ptr)(Node *);

    // Pointer to ui
    Ui::PuzzleSolver *ui;


public:
    explicit PuzzleSolver(QWidget *parent = nullptr);
    ~PuzzleSolver();

    // Initilization and check
    NodePtr init();
    NodeListPtr init_open();
    NodeListPtr init_closed();
    int check_target_matrix(NodeListPtr);

    // Comparison between two state (save as node or matrix)
    bool matrix_compare(MATRIX, MATRIX);
    bool node_compare  (NodePtr, NodePtr);

    // Maintain a open table and a closed table
    NodePtr get_open_head(NodeListPtr open);
    bool search_closed(NodeListPtr closed, NodePtr expand_node);
    void add_to_open(NodeListPtr open, NodePtr node);
    void add_to_closed(NodeListPtr closed, NodePtr node);

    // Differnet heuristic functions and unified interface
    int h1_misplaced_tiles(NodePtr node);
    int h2_manhattan_distance(NodePtr node);
    int hval(NodePtr node, int (__thiscall PuzzleSolver::*fptr)(Node *));

    // Searching process
    void move(int row, int col, direction dir, NodePtr node, NodeListPtr open, NodeListPtr closed);
    void search(int row, int col, NodePtr node, NodeListPtr open, NodeListPtr closed);
    void expand(NodeListPtr open, NodeListPtr closed, NodePtr to_expand);

    // Entrance function!
    NodePtr solve();

    // Functions for UI exhibition
    int ui_move_blocks(NodePtr node);
    int ui_output_path(NodePtr node);
    QLabel *find_Qlabel(int i);
    int find_zero(NodePtr node);

    // Automatic generate and solution
    void shuffle_matrix(int *matrix, inversion inv = Even);


private slots:
    // slot function for buttons
    void on_Solve_clicked();
    void on_Test_clicked();
    void on_showtree_clicked();
};

#endif
