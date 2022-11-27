#include "Triviador.h"
#include "LoginRegister.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Triviador w;
    //w.show();
	
    LoginRegister r;
    r.show();
	
    return a.exec();
}
