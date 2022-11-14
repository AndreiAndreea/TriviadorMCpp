#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Triviador.h"

class Triviador : public QMainWindow
{
    Q_OBJECT

public:
    Triviador(QWidget *parent = nullptr);
    ~Triviador();

private:
    Ui::TriviadorClass ui;
};
