#include "Triviador.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Triviador w;
    w.show();
    return a.exec();
}
