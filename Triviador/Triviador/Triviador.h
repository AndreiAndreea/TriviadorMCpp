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
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Questions m_questions;
    std::string m_currentAnswer;
    Ui::TriviadorClass ui;
};
