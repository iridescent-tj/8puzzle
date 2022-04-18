# A*搜索八数码问题

## 1 总体架构

![img](assets\wps11.jpg)

## 2 环境说明	

操作系统：Win10

开发语言：C++

开发环境：MinGw 7.3.0 64-bit for C++

​			 Qt 5.14.2

​			 Qt Creator 4.11.1

​			 Cmake 3.22.3

核心使用库：Qt的Core模块以及GUI模块

​				Graphviz 3.0.0

## 3 源代码文件清单

8puzzle.pro 项目文件：提供qmake关于为应用程序创建makefile所需要的细节。

puzzlesolver.h 头文件：与八数码问题求解相关的宏定义、类定义与函数声明。

drawgraph.h 头文件：搜索树的绘制。

utils.h 头文件：搜索树的定义，以及项目中用到的一些通用函数（如求逆序数）的声明。

main.cpp 源文件：main函数，程序入口。

heuristic_fun.cpp 源文件：实现两种启发函数（错位数与曼哈顿距离），以及调用的统一接口。

puzzlesolver.cpp 源文件：八数码问题求解的函数实现（如初始化、表维护、搜索、扩展等）。

test.cpp 源文件：自动生成与自动求解模式。随机生成初始状态与目标状态，然后求解。

ui.cpp 源文件：与UI界面中数据输入、动画演示、路径生成等相关函数的实现。

drawgraph.cpp 源文件：搜索树的绘制函数实现。

puzzlesolver.ui UI文件：程序运行的整个UI界面。

## 4 主要函数清单

```c++
// Initilization
    NodePtr init();
    NodeListPtr init_open();
    NodeListPtr init_closed();
	int check_target_matrix(NodeListPtr);
// Comparison between two states (save as node or matrix)
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
// get inversion number (based on merge sort)
	int get_inversion_number(const int * const arr,const int len);
// mermory recovery
	void delete_linklist(NodeListPtr list);
// debug
	void print_matrix(MATRIX matrix);
	std::string print_matrix_tostr(MATRIX matrix);
	void print_closed(NodeListPtr closed);
// draw the search tree
	void bfs(NodePtr);
```



## 5 结果

##### （1）双击code目录下8puzzle.pro文件，进入Qt Ctreator配置界面。

![img](assets\wps1.jpg) 

（2）目录结构

![img](assets\wps2.jpg) 

（3）运行，呈现UI界面如下

![img](assets\wps3.jpg) 

（4）求解：在两个框中分别随机输入数字1-8（空出数字0的位置即可），例如（课本P91例子）

![img](assets\wps4.jpg) 

程序自动进行初始状态的检查

![img](assets\wps5.jpg) 

然后进行目标状态的检查

![img](assets\wps6.jpg) 

点击Solve按钮，程序会检查是否有解（基于逆序数），若无解，给出提示，要求用户调整。

![img](assets\wps7.jpg) 

若有解，自动求解，然后在下框内以动画演示求解过程。

![img](assets\wps8.jpg) 

演示结束，右侧框内展示搜索的路径

![img](assets\wps9.jpg) 

（5）随机生成：点击Test按钮，随机生成一组测试的初始状态与目标转换，然后自动求解，演示流程同上。

![img](assets\wps10.jpg) # 8puzzle
