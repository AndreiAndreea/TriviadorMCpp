#pragma once

#include <QtWidgets/QMainWindow>
#include<array>
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

    void CheckMultipleChoiceAnswer(QString chosenAnswer,bool & answer);

private slots:
    void on_checkToDisplayQuestionsInFile_released();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_mc_ans1_released();
    void on_mc_ans2_released();
    void on_mc_ans3_released();
    void on_mc_ans4_released();

private:
    Questions m_questions;
    std::string m_currentAnswer;
    Ui::TriviadorClass ui;
};
