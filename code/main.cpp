#include "puzzlesolver.h"
#include "utils.h"
#include <QApplication>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PuzzleSolver w;
    w.show();

    return a.exec();
}
