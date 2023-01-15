#include "LoginRegister.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	
    LoginRegister r;
    r.show();

    return a.exec();
}