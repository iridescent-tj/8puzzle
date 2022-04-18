#include "utils.h"
#include "puzzlesolver.h"
#include "QDebug"
#include "ui_puzzlesolver.h"
#include <QRegExpValidator>

/**
 * constructor
 */
PuzzleSolver::PuzzleSolver(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PuzzleSolver)
{
    ui->setupUi(this);

    // Limit input to a single number(regular expression).
    QRegExp regx("[0-9]$");
    QValidator *validator = new QRegExpValidator(regx);
    ui->init1 ->setValidator( validator );
    ui->init2 ->setValidator( validator );
    ui->init3 ->setValidator( validator );
    ui->init4 ->setValidator( validator );
    ui->init5 ->setValidator( validator );
    ui->init6 ->setValidator( validator );
    ui->init7 ->setValidator( validator );
    ui->init8 ->setValidator( validator );
    ui->init9 ->setValidator( validator );
    ui->final1->setValidator( validator );
    ui->final2->setValidator( validator );
    ui->final3->setValidator( validator );
    ui->final4->setValidator( validator );
    ui->final5->setValidator( validator );
    ui->final6->setValidator( validator );
    ui->final7->setValidator( validator );
    ui->final8->setValidator( validator );
    ui->final9->setValidator( validator );

    // choose heuristic function in heuristic.cpp
    // After conducting tests, h2_manhattan_distance performs better than h1_misplaced_tiles in most circumstances.
    this->h_ptr = &PuzzleSolver::h2_manhattan_distance;
}

/**
 * destructor
 */
PuzzleSolver::~PuzzleSolver()
{
    delete root;
    delete_linklist(open);
    delete_linklist(closed);
    delete ui;
}


/**
 * @brief  initialize the eight-dight-puzzle
 * @return nullptr or rootptr
 */
NodePtr PuzzleSolver::init()
{
    // create root of a search tree
    NodePtr root = new Node;
    if(!root) {
        QMessageBox::warning(NULL, "warning", "Memory allocate failed!", QMessageBox::Yes, QMessageBox::Yes);
        return nullptr;
    }

    // get input data from the nine QLineEdit
    root->matrix[0][0] = ui->init1->text().toInt();
    root->matrix[0][1] = ui->init2->text().toInt();
    root->matrix[0][2] = ui->init3->text().toInt();
    root->matrix[1][0] = ui->init4->text().toInt();
    root->matrix[1][1] = ui->init5->text().toInt();
    root->matrix[1][2] = ui->init6->text().toInt();
    root->matrix[2][0] = ui->init7->text().toInt();
    root->matrix[2][1] = ui->init8->text().toInt();
    root->matrix[2][2] = ui->init9->text().toInt();

    // simply check input data's completeness (fully cover 1-8??)
    bool flag[Matrix_N2] = {0};
    bool complete = 0;
    for (int i = 0; i < Matrix_N; ++i) {
        for (int j = 0; j < Matrix_N; ++j) {
            flag[root->matrix[i][j]] = 1;
        }
    }
    for (int i = 0; i < Matrix_N * Matrix_N; ++i) {
        if (!flag[i]) {
            complete = 1;
        }
    }
    if (complete) {
        QMessageBox::information(NULL, "Waring", "The initial state's number is incomplete!", QMessageBox::Yes, QMessageBox::Yes);
        return nullptr;
    }

    // initialize other attributes
    root->gn = 0;
    root->hn = hval(root,this->h_ptr);
    root->fn = root->hn + root->gn;
    root->next = nullptr;
    root->prior = nullptr;
    root->parent = nullptr;
    root->layer = 1;
    root->path = true;

    return root;
}


/**
 * check target matrix
 * @param
 *  NodeListPtr list : list consists the initial state of matrix
 * @return
 *  -1  : incomplete
 *  0   : unsolvable
 *  1   : valid
 */
int PuzzleSolver::check_target_matrix(NodeListPtr list)
{
    // 1. simply check input data's completeness (fully cover 1-8??)
    bool flag[Matrix_N * Matrix_N] = {};
    bool complete = 0;
    for (int i = 0; i < Matrix_N; ++i) {
        for (int j = 0; j < Matrix_N; ++j) {
            flag[target_matrix[i][j]] = 1;
        }
    }
    for (int i = 0; i < Matrix_N * Matrix_N; ++i) {
        if (!flag[i]) {
            complete = 1;
        }
    }
    if (complete) {
        QMessageBox::information(NULL, "Waring", "The target state's number is incomplete!", QMessageBox::Yes, QMessageBox::Yes);
        return -1;
    }

    // 2. Solvable or Unsolvable
    // flaten the matrix and delete 0 for calculating inversion number
    int sol[Matrix_N * Matrix_N] = {},*p = sol;
    for (int i = 0; i < Matrix_N; ++i) {
        for (int j = 0; j < Matrix_N; ++j) {
            if(target_matrix[i][j])
                *p++ = target_matrix[i][j];
        }
    }
    int inv1 = get_inversion_number(sol,Matrix_N * Matrix_N - 1);
    p = sol;
    for (int i = 0; i < Matrix_N; ++i) {
        for (int j = 0; j < Matrix_N; ++j) {
            if(list->head->matrix[i][j])
                *p++ = list->head->matrix[i][j];
        }
    }
    int inv2 = get_inversion_number(sol,Matrix_N * Matrix_N - 1);
    //qDebug()<<inv1<<" "<<inv2<<"\n";

    if (inv1%2 != inv2%2)
        return 0; // unsolvable

    // successfully pass all checks
    return 1;
}

/**
 */
NodeListPtr PuzzleSolver::init_open()
{
    NodeListPtr otable = new NodeList;
    NodePtr head = init();
    if (otable) {
        if (head != nullptr) {
            otable->head = head;
            otable->tail = head;
        }
        else {
            delete otable;
            return nullptr;
        }
    }
    else {
        QMessageBox::warning(NULL, "warning", "Memory allocate failed!", QMessageBox::Yes, QMessageBox::Yes);
        return nullptr;
    }
    return otable;
}


NodeListPtr PuzzleSolver::init_closed()
{
    NodeListPtr closed = new NodeList;
    if (!closed) {
        QMessageBox::warning(NULL, "warning", "Memory allocate failed!", QMessageBox::Yes, QMessageBox::Yes);
        return nullptr;
    }
    else {
        closed->head = nullptr;
        closed->tail = nullptr;
    }
    return closed;
}


/**
 * Get the head node of open table
 */
NodePtr PuzzleSolver::get_open_head(NodeListPtr open)
{
    NodePtr head = open->head;
    if (open->head == open->tail) {
        open->head = nullptr;
        open->tail = nullptr;
    }
    else {
        open->head = open->head->next;
    }
    return head;
}


/**
 * Judge whether two MTTRIXS are representing a same state
 */
bool PuzzleSolver::matrix_compare(MATRIX ls, MATRIX rs)
{
    return !memcmp(ls,rs,sizeof(int)*Matrix_N2);
}


/**
 * Judge whether two nodes are representing a same state
 * @param
 * NodePtr ls,rs : two node with status info
 */
bool PuzzleSolver::node_compare(NodePtr ls, NodePtr rs)
{
    return matrix_compare(ls->matrix,rs->matrix);
    // return !memcmp(ls->matrix,rs->matrix,sizeof(int)*Matrix_N2);
}


/**
 * Travel the closed table to judge whether the node is visited
 */
bool PuzzleSolver::search_closed(NodeListPtr closed, NodePtr expand_node)
{
    NodePtr tmp = closed->head;
    while (tmp) { // travel the linklist
        if (node_compare(tmp, expand_node)) { // in same status
            if (tmp->gn > expand_node->gn) { // abandon less optimal node to update closed table
                tmp->gn = expand_node->gn;
                tmp->fn = tmp->hn + tmp->gn;
                tmp->prior = expand_node->prior;
            }
            break;
        }
        tmp = tmp->next;
    }
    if (tmp == nullptr)
        return false; // not found
    else
        return true; // found
}


/**
 * get the position of zero and encode it into 0-8
 */
int PuzzleSolver::find_zero(NodePtr node)
{
    int pos = -1;
    for (int i = 0; i < Matrix_N; ++i) {
        for (int j = 0; j < Matrix_N; ++j) {
            if (node->matrix[i][j] == 0) {
                return pos = i*Matrix_N+j;
            }
        }
    }
    return pos;
}


/**
 * Add the node to open table, maintain the incremental sequence of the table at the same time
 */
void PuzzleSolver::add_to_open(NodeListPtr open, NodePtr node)
{
    NodePtr ptr = nullptr, pre = nullptr;
    if (open->head == nullptr) { // empty table
        open->head = node;
        open->tail = node;
    }
    else {
        ptr = open->head;
        while (ptr && ptr->fn <= node->fn) {
            pre = ptr;
            ptr = ptr->next;
        }
        if (ptr == nullptr) { // node->fn greater than all existing values in the table
            open->tail->next = node;
            open->tail = node;
        }
        else { // pre->fn <= node->fn <= ptr, a suitable location for this node!
            if (pre == nullptr) {
                node->next = open->head;
                open->head = node;
            }
            else {
                pre->next = node;
                node->next = ptr;
            }
        }
    }
}


/**
 * Add the node to the closed table and end the expansion process of the node
 */
void PuzzleSolver::add_to_closed(NodeListPtr closed, NodePtr node)
{
    if (closed->head == nullptr) {
        closed->head = node;
        closed->tail = node;
    }
    else {
        node->next = nullptr;
        closed->tail->next = node;
        closed->tail = node;
    }
}


/**
 * Try a move of elem"0" in a designated direction
 * @param
 * row,col : position of 0 in current matrix
 */
void PuzzleSolver::move(int row, int col, direction dir, NodePtr node, NodeListPtr open, NodeListPtr closed)
{
    NodePtr new_node = new Node;
    if(!new_node){
        QMessageBox::warning(NULL, "warning", "Memory allocate failed!", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    for (int i = 0; i < Matrix_N; ++i) {
        for (int j = 0; j <Matrix_N; ++j) {
            new_node->matrix[i][j] = node->matrix[i][j];
        }
    }
    switch (dir) {
        case Up:
            new_node->matrix[row][col] = new_node->matrix[row+1][col];
            new_node->matrix[row+1][col] = 0;
            break;
        case Down:
            new_node->matrix[row][col] = new_node->matrix[row-1][col];
            new_node->matrix[row-1][col] = 0;
            break;
        case Left:
            new_node->matrix[row][col] = new_node->matrix[row][col+1];
            new_node->matrix[row][col+1] = 0;
            break;
        case Right:
            new_node->matrix[row][col] = new_node->matrix[row][col-1];
            new_node->matrix[row][col-1] = 0;
            break;
        default:
            break;
    }
    new_node->gn = node->gn + 1; // step++
    new_node->prior = node;      // save the current node as a prior
    new_node->next = nullptr;
    new_node->hn = hval(new_node,this->h_ptr); // calculate h(n) for new node
    new_node->fn = new_node->hn + new_node->gn;
    new_node->parent = node;
    new_node->layer = node->layer+1;
    new_node->path = false;

    if (!search_closed(closed, new_node)){ // unvisted
        node->child.push_back(new_node);
        add_to_open(open, new_node);
    }
    else { // visited
        delete new_node;
    }
}


/**
 * @brief
 * Move and probe according to the position of 0.
 * Perform weight judgment, put the better one or the only one into open table.
 * @param
 * row,col : position of 0 in current matrix
 *
 */
void PuzzleSolver::search(int row, int col, NodePtr node, NodeListPtr open, NodeListPtr closed)
{
    if(row!=Matrix_N-1)
        move(row, col, Up, node, open, closed);
    if(row!=0)
        move(row, col, Down, node, open, closed);
    if(col!=Matrix_N-1)
        move(row, col, Left, node, open, closed);
    if(col!=0)
        move(row, col, Right, node, open, closed);
}


/**
 * Expand the next state of the current node
 */
void PuzzleSolver::expand(NodeListPtr open, NodeListPtr closed, NodePtr to_expand)
{
    int pos, row, col;
    pos = find_zero(to_expand);
    row = pos / Matrix_N;
    col = pos % Matrix_N;
    search(row, col, to_expand, open, closed);
}


/**
 * Recursive output search path
 * @param pos: encoded position(0-8) of number zero
 */
QLabel* PuzzleSolver::find_Qlabel(int pos)
{
    QLabel* showlist[Matrix_N2] = {ui->block1,ui->block2,ui->block3,ui->block4,ui->block5,ui->block6,ui->block7,ui->block8,ui->block9};
    return showlist[pos];
}


/**
 * Entrance function
 */
NodePtr PuzzleSolver::solve()
{
    bool succ = 0;
    NodePtr tmp = nullptr;
    while (!succ && open->head) {
        tmp = get_open_head(open);
        if (matrix_compare(tmp->matrix,target_matrix)) {
            succ = true;
            break;
        }
        add_to_closed(closed, tmp);
        expand(open, closed, tmp);
    }
    if (!succ) {
        QMessageBox::critical(this, "Error", "Unsolved for unknown reasons.", QMessageBox::Yes, QMessageBox::Yes);
        return nullptr;
    }
    return tmp;
}
