#include<iostream>
#include "utils.h"
#include "QDebug"
#include "drawgraph.h"
using namespace std;

static void merge_sort(int left, int right,int arr[],int sub[],int &ret) {
    if (left >= right)
        return;
    int mid = (left + right) / 2;

    merge_sort(left, mid, arr, sub, ret);
    merge_sort(mid + 1, right, arr, sub, ret);

    int p1 = left, p2 = mid + 1, i = left;
    while (p1 <= mid && p2 <= right) {
        if (arr[p1] <= arr[p2])
            sub[i++] = arr[p1++];
        else if (arr[p1] > arr[p2]) {
            sub[i++] = arr[p2++];
            ret += mid - p1 + 1;
        }
    }

    while (p2 <= right)
        sub[i++] = arr[p2++];
    while (p1 <= mid)
        sub[i++] = arr[p1++];
    for (int i = left; i <= right; i++)
        arr[i] = sub[i];
}

int get_inversion_number(const int * const arr,const int len)
{
    int *sub = new int[len+1];
    int *mas = new int[len+1];
    if(!sub || !mas){
        qDebug()<<"memory allocate failed!";
    }
    memcpy(mas,arr,sizeof(int)*len);
    int ret = 0;
    merge_sort(0, len - 1, mas, sub, ret);
    return ret;
}

void delete_linklist(NodeListPtr list)
{
    NodePtr tmp = nullptr;
    if (list) {
        while (list->head) {
            tmp = list->head;
            list->head = list->head->next;
            delete tmp;
        }
        delete list;
        list = nullptr;
    }
}

void print_matrix(MATRIX matrix)
{
    QString str;
    for(int i =0;i<Matrix_N;i++){
        for(int j =0;j<Matrix_N;j++){
            str+=QString::number(matrix[i][j]);
            str+=" ";
        }
        str+="\n";
    }
   std::cout<<str.toStdString()<<std::endl;
}

string print_matrix_tostr(MATRIX matrix)
{
    QString str;
    for(int i =0;i<Matrix_N;i++){
        for(int j =0;j<Matrix_N;j++){
            str+=QString::number(matrix[i][j]);
            //str+=" ";
        }
        str+="\\n";
    }
    return str.toStdString();
}

void print_closed(NodeListPtr closed)
{

    NodePtr tmp = closed->head;
    while (tmp) { // travel the linklist
        std::cout<<tmp->parent<<" "<<tmp->layer<<std::endl;
        print_matrix(tmp->matrix);
        tmp = tmp->next;
    }
}

void bfs(NodePtr root)
{
    std::cout<<"start bfs.....\n";
    DrawGraph d;
    d.Start(DrawGraph::DiGraph);

    int cnt = 1;
    QList<NodePtr> ls;
    if(root)
        ls.push_back(root);

    NodePtr tmp=nullptr;
    std::string tmpmatrix;
    while(!ls.isEmpty()){
        tmp = ls.front();
        ls.pop_front();

        tmp->no = cnt++;
        tmpmatrix = print_matrix_tostr(tmp->matrix);
        tmpmatrix += "f:"+QString::number(tmp->fn).toStdString()+" ";
        tmpmatrix += "g:"+QString::number(tmp->gn).toStdString()+" ";
        tmpmatrix += "h:"+QString::number(tmp->hn).toStdString()+" ";

        if(tmp->path)
            d.Create_Node(tmp->no, tmpmatrix,true);
        else
            d.Create_Node(tmp->no, tmpmatrix,false);
        if(tmp->parent){
            if(tmp->path)
                d.Create_Relationship(tmp->parent->no, tmp->no,DrawGraph::DiGraph,true);
            else
                d.Create_Relationship(tmp->parent->no, tmp->no,DrawGraph::DiGraph,false);
        }
        for(auto i = tmp->child.begin();i<tmp->child.end();i++){
            ls.push_back(*i);
        }
    }

    d.End();
    d.Get_Png("../graph/test.png");

    std::cout<<"总结点数："<<cnt<<std::endl;
}
