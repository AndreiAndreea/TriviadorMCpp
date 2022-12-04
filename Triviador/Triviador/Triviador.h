#pragma once

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include <QTime>
#include <QElapsedTimer>
#include <array>

#include "ui_Triviador.h"

#include "DataBase.h"

#include "Questions.h" //need to be deleted in future

class Triviador : public QMainWindow
{
    Q_OBJECT

public:
    Triviador(QWidget *parent = nullptr);
    ~Triviador();
    bool GetCanChooseTerritory();
    void SetCanChooseTerritory(bool canChooseTerritory);
    void SetNumberOfPlayers(uint16_t numberOfPlayers);

private:
    void SaveSingleChoiceQuestionsToFile(const QString fileName);
    void SaveMultipleChoiceQuestionsToFile(const QString fileName);
    void StartTimer();
    void ToggleAnswers(bool toggleAnswer);
    void CheckMultipleChoiceAnswer(const QString chosenAnswer, bool& answer);

private slots:
    void on_saveQuestionsInFileButton_released();

    void on_getRandomQuestionButton_released();

    void on_submitAnswerButton_released();

    void on_multipleChoiceAnswer1Button_released();
    void on_multipleChoiceAnswer2Button_released();
    void on_multipleChoiceAnswer3Button_released();
    void on_multipleChoiceAnswer4Button_released();

    void OnTimerTick();
	
private:
    bool m_answerHasBeenSelected;

    Questions m_questions;
    std::string m_currentAnswer;

    Ui::TriviadorClass ui;

    bool m_canChooseTerritory;
    uint16_t m_numberOfPlayers=0;
    
    QTimer* timer;
    QElapsedTimer elapsedTime;
};
