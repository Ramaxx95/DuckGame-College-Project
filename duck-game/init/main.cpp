#include <QApplication>

#include "initwindow.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    InitWindow w;
    w.show();
    return a.exec();
}
