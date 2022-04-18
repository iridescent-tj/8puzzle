#include "drawgraph.h"

void DrawGraph::Start(int mode)
{
    FILE* fp = fopen(tmp_file.c_str(), "w");
    if (mode == DiGraph)
        fprintf(fp, "digraph my_graph{\n");
    else if (mode == Graph)
        fprintf(fp, "graph my_graph{\n");
    fclose(fp);
}

void DrawGraph::Create_Node(int no, std::string label, bool color)
{
    FILE* fp = fopen(tmp_file.c_str(), "a");
    if(color)
        fprintf(fp, "\tn%s [label=\"%s\" color=red];\n", std::to_string(no).c_str(),label.c_str());
    else
        fprintf(fp, "\tn%s [label=\"%s\"];\n", std::to_string(no).c_str(),label.c_str());
    fclose(fp);
}

void DrawGraph::Create_Relationship(int start_no, int end_no, int mode, bool color)
{
    FILE* fp = fopen(tmp_file.c_str(), "a");

    fprintf(fp, "\tn%s ", std::to_string(start_no).c_str());

    if (mode == DiGraph) {
        fprintf(fp, " -> ");
    }
    else if (mode == Graph) {
        fprintf(fp, " -- ");
    }

    fprintf(fp, " n%s ", std::to_string(end_no).c_str());

    if (color) {
        fprintf(fp, "[color=red]");
    }

    fprintf(fp, ";\n");

    fclose(fp);
}

void DrawGraph::End()
{
    FILE* fp = fopen(tmp_file.c_str(), "a");

    fprintf(fp, "}");
    fclose(fp);
}

void DrawGraph::Get_Png(std::string file_path)
{
    std::string command = "dot -Tpng " + tmp_file + " -o " + file_path;
    system(command.c_str());
}
