#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Triviador.h"
#include "Questions.h"

class Triviador : public QMainWindow
{
    Q_OBJECT

public:
    Triviador(QWidget *parent = nullptr);
    ~Triviador();

private:
    void DisplaySCQuestionsInFile(QString fileName);
    void DisplayMCQuestionsInFile(QString fileName);

private slots:
    void on_checkToDisplayQuestionsInFile_released();

private:
    Questions questions;
    Ui::TriviadorClass ui;
};
