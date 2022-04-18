#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <QVector>

#define Matrix_N 3
#define Matrix_N2 9

typedef int MATRIX[Matrix_N][Matrix_N];
typedef int VECTOR[Matrix_N*Matrix_N];

struct Node {
    // Used for currently expanded node in the graph/tree.

    // src-->now-->tar
    int hn; // Estimated minimum path cost from "n" to "t"
    int gn; // Actual path cost from "s" to "n"
    int fn; // The value of the valuation function at this node: fn = gn + hn
    MATRIX matrix;  // current status
    // Generate a linklist to save a search path
    Node *next;
    Node *prior;
    // Search Tree Structure
    QVector<Node*> child;
    Node *parent;
    int layer;
    bool path;
    int no;
};
typedef Node* STree;
typedef Node* NodePtr;

struct NodeList{
    NodePtr head;
    NodePtr tail;
};
typedef NodeList* NodeListPtr;

// get inversion number (based on merge sort)
int get_inversion_number(const int * const arr,const int len);
// mermory recovery
void delete_linklist(NodeListPtr list);

void print_matrix(MATRIX matrix);
std::string print_matrix_tostr(MATRIX matrix);

void print_closed(NodeListPtr closed);

void bfs(NodePtr);
#endif
