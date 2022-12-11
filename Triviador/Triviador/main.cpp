#include "Triviador.h"

#include "LoginRegister.h"
#include "Game.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Triviador w;
    //w.show();
	
    LoginRegister r;
    r.show();
	
    //Game game;
    //game.show();

    return a.exec();
}