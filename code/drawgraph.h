#ifndef DRAWGRAPH_H
#define DRAWGRAPH_H

#include <map>
#include <string>
#include <fstream>

class DrawGraph
{
private:
    std::string tmp_file = "../graph/tmp.tp";
    std::string png_file = "../graph/test.png";

public:
    static const int DiGraph = 1, Graph = 0;

    void Start(int mode = 0);
    void Create_Node(int no, std::string lable, bool color = false);
    void Create_Relationship(int, int, int mode = 1, bool color = false);
    void End();
    void Get_Png(std::string file_path);
};


#endif
