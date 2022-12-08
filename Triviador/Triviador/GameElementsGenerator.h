#pragma once

#include <QDialog>

#include "ui_GameElementsGenerator.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QtWidgets/QMainWindow>
#include <QTimer>
#include <QTime>
#include <QElapsedTimer>
#include <array>
#include <sstream>
#include <ctime>
#include <cstdlib>

#include "Questions.h"
#include "LoginRegister.h"

#include "DataBase.h"

class GameElementsGenerator : public QDialog
{
	Q_OBJECT

private:
	Ui::GameElementsGenerator ui;

public:
	explicit GameElementsGenerator();
	~GameElementsGenerator();
    
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

    //Ui::GameElementsGenerator ui;

    bool m_canChooseTerritory;
    uint16_t m_numberOfPlayers = 0;

    QTimer* timer;
    QElapsedTimer elapsedTime;
};