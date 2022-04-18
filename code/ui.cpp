#include "utils.h"
#include "puzzlesolver.h"
#include "ui_puzzlesolver.h"
#include "string.h"
#include "QDebug"
#include "utils.h"
#include <sys/time.h>


/**
 * slot function for the solve button in UI
 */
void PuzzleSolver::on_Solve_clicked()
{
    if (root) {
        free(root);
        root = nullptr;
    }
    if (open) {
        delete_linklist(open);
        open = nullptr;
    }
    if (closed) {
        delete_linklist(closed);
        closed = nullptr;
    }

    open = init_open();
    closed = init_closed();
    target_matrix[0][0] = ui->final1->text().toInt();
    target_matrix[0][1] = ui->final2->text().toInt();
    target_matrix[0][2] = ui->final3->text().toInt();
    target_matrix[1][0] = ui->final4->text().toInt();
    target_matrix[1][1] = ui->final5->text().toInt();
    target_matrix[1][2] = ui->final6->text().toInt();
    target_matrix[2][0] = ui->final7->text().toInt();
    target_matrix[2][1] = ui->final8->text().toInt();
    target_matrix[2][2] = ui->final9->text().toInt();


    if(open && closed) {
        int val = check_target_matrix(open);
        if(val == 1) {
            qDebug()<<"Start solving......";

            double time_Start = (double)clock();
            root = solve();
            double time_End = (double)clock();

            qDebug()<<(time_End - time_Start)<<"ms";
            qDebug()<<"Successfully solve the puzzle!";

            ui_move_blocks(root);
            qDebug()<<"Successfully finish moving blocks!";

            ui->result->document()->setDefaultTextOption(QTextOption(Qt::AlignHCenter));
            ui_output_path(root);
            ui->result->append("finish!");

            bfs(closed->head);
        }
        else if(val == 0){
            QMessageBox::critical(this, "Unsolvable puzzle!", "Reconsider the inversion number of puzzle matrix", QMessageBox::Yes, QMessageBox::Yes);
        }
    }
}


/**
 * Recursively move blocks
 */
int PuzzleSolver::ui_move_blocks(NodePtr node)
{
    if (node) {
        int pre = ui_move_blocks(node->prior);
        if (pre == -1) { // reaching the final state and get return value as -1
            // initialize blocks
            ui->block1->setText(QString::number(node->matrix[0][0]));
            ui->block2->setText(QString::number(node->matrix[0][1]));
            ui->block3->setText(QString::number(node->matrix[0][2]));
            ui->block4->setText(QString::number(node->matrix[1][0]));
            ui->block5->setText(QString::number(node->matrix[1][1]));
            ui->block6->setText(QString::number(node->matrix[1][2]));
            ui->block7->setText(QString::number(node->matrix[2][0]));
            ui->block8->setText(QString::number(node->matrix[2][1]));
            ui->block9->setText(QString::number(node->matrix[2][2]));
            int pos0 = find_zero(node);
            find_Qlabel(pos0)->setText(""); // empty the position of 0 (for better demonstration effect
            return pos0;
        }
        else {
            int pos0 = find_zero(node);
            int cur_row = pre / Matrix_N;
            int cur_col = pre % Matrix_N;
            int num = node->matrix[cur_row][cur_col];
            QLabel* label_pre = find_Qlabel(pre);
            QLabel* label_cur = find_Qlabel(pos0);

            QElapsedTimer t; // set delay
            t.start();
            while(t.elapsed() < 1000)
                QCoreApplication::processEvents();

            label_pre->setText(QString::number(num)); // fill number
            label_cur->setText("");                   // empty

            return pos0;
        }
    }
    else
        return -1;
}


/**
 * Recursively output search path
 */
int PuzzleSolver::ui_output_path(NodePtr node)
{
    if (node) {
        int rec = ui_output_path(node->prior);
        QString str;
        for (int i = 0; i < Matrix_N; ++i) {
            for (int j = 0; j < Matrix_N; ++j) {
            str += QString::number(node->matrix[i][j]) + " ";
            if(j == Matrix_N-1)
                str += "\n";
            }
        }
        str+="fn:"+QString::number(node->fn)+"  ";
        str+="gn:"+QString::number(node->gn)+"  ";
        str+="hn:"+QString::number(node->hn)+"  ";
        str+="\n";
        ui->result->append(str);
        if(rec == -1) // flag of the end of recursion
            ui->result->setText(str);

        node->path = true;
        return 1;
    }
    else
        return -1;
}


/**
 * slot function for the solve button in UI
 */
void PuzzleSolver::on_Test_clicked()
{
    srand(unsigned(time(NULL)));
    qDebug()<<"on testing";
    if (root) {
        free(root);
        root = nullptr;
    }
    if (open) {
        delete_linklist(open);
        open = nullptr;
    }
    if (closed) {
        delete_linklist(closed);
        closed = nullptr;
    }

    shuffle_matrix((int*)target_matrix);
    ui->init1->setText(QString::number(target_matrix[0][0]));
    ui->init2->setText(QString::number(target_matrix[0][1]));
    ui->init3->setText(QString::number(target_matrix[0][2]));
    ui->init4->setText(QString::number(target_matrix[1][0]));
    ui->init5->setText(QString::number(target_matrix[1][1]));
    ui->init6->setText(QString::number(target_matrix[1][2]));
    ui->init7->setText(QString::number(target_matrix[2][0]));
    ui->init8->setText(QString::number(target_matrix[2][1]));
    ui->init9->setText(QString::number(target_matrix[2][2]));

    shuffle_matrix((int*)target_matrix);
    ui->final1->setText(QString::number(target_matrix[0][0]));
    ui->final2->setText(QString::number(target_matrix[0][1]));
    ui->final3->setText(QString::number(target_matrix[0][2]));
    ui->final4->setText(QString::number(target_matrix[1][0]));
    ui->final5->setText(QString::number(target_matrix[1][1]));
    ui->final6->setText(QString::number(target_matrix[1][2]));
    ui->final7->setText(QString::number(target_matrix[2][0]));
    ui->final8->setText(QString::number(target_matrix[2][1]));
    ui->final9->setText(QString::number(target_matrix[2][2]));

    open = init_open();
    closed = init_closed();

    if(open && closed) {
        int val = check_target_matrix(open);
        if(val == 1) {
            qDebug()<<"Start solving......";

            double time_Start = (double)clock();
            root = solve();
            double time_End = (double)clock();

            qDebug()<<(time_End - time_Start)<<"ms";
            qDebug()<<"Successfully solve the puzzle!";

            ui_move_blocks(root);
            qDebug()<<"Successfully finish moving blocks!";

            ui->result->document()->setDefaultTextOption(QTextOption(Qt::AlignHCenter));
            ui_output_path(root);
            ui->result->append("finish!");

            bfs(closed->head);
        }
        else if(val == 0){
            QMessageBox::critical(this, "Unsolvable puzzle!", "Reconsider the inversion number of puzzle matrix", QMessageBox::Yes, QMessageBox::Yes);
        }
    }
}


void PuzzleSolver::on_showtree_clicked()
{
    std::string filepath = "D:/Desktop/Qt/8puzzle/code2/graph/test.png";
    FILE* fp = fopen(filepath.c_str(),"a");
    if(!fp){
        QMessageBox::critical(this, "File not exists", ".png file not exists!", QMessageBox::Yes, QMessageBox::Yes);
        fclose(fp);
    }
    else{
        fclose(fp);
        filepath = "\"" + filepath + "\"";
        system(filepath.c_str());
    }
}
