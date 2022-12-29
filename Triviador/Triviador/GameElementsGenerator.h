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
#include <string>
#include <cstdlib>

#include "Questions.h"

#include "DataBase.h"

class GameElementsGenerator : public QDialog
{
	Q_OBJECT

private:
	Ui::GameElementsGenerator ui;

public:
	explicit GameElementsGenerator();
	~GameElementsGenerator();
    
    //setters and gettters
    bool GetCanChooseTerritory();
    void SetCanChooseTerritory(bool canChooseTerritory);
    void SetNumberOfPlayers(uint16_t numberOfPlayers);

private:
    void StartTimer();
    void ToggleAnswers(bool toggleAnswer);
    void DisableAdvantageOfferedAnswers(bool toggleAnswer);

    void CheckMultipleChoiceAnswer(const QString chosenAnswer, bool& answer);

    bool CheckQStringToAnswer(const QString text);
    
	void HideOfferedAnswers(bool hide);

private slots:
    void on_getRandomQuestionButton_released();

    void on_submitAnswerButton_released();

    void on_multipleChoiceAnswer1Button_released();
    void on_multipleChoiceAnswer2Button_released();
    void on_multipleChoiceAnswer3Button_released();
    void on_multipleChoiceAnswer4Button_released();
    
    void on_fifty_fiftyAdvantageButton_released();
    void on_suggestAnswerAdvantageButton_released();
    void on_offerAnswersAdvantageButton_released();

    void on_offeredAnswer1Button_released();
    void on_offeredAnswer2Button_released();
    void on_offeredAnswer3Button_released();
    void on_offeredAnswer4Button_released();

    void SubmitSingleChoiceAnswer(uint16_t inputAnswer, uint16_t currentAnswer);

    void OnTimerTick();

private:
    bool m_answerHasBeenSelected;

    uint16_t m_amountOfRandomQuestionsFromDatabase;

    Questions m_questions;
    std::string m_currentAnswer;

    bool m_canChooseTerritory;
    uint16_t m_numberOfPlayers = 0;

    QTimer* timer;
    QElapsedTimer elapsedTime;

    std::vector<QuestionSingleChoice> m_randomSingleChoiceQuestionsVector;
	std::vector<QuestionMultipleChoice> m_randomMultipleChoiceQuestionsVector;
};