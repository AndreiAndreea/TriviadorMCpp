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

#include "QuestionSingleChoice.h"
#include "QuestionMultipleChoice.h"

#include <curl/curl.h>
#include <cpr/cpr.h>
#include <crow.h>

#include "Player.h"

class GameElementsGenerator : public QDialog
{
	Q_OBJECT

private:
	Ui::GameElementsGenerator ui;

public:
	GameElementsGenerator(const std::string& ip, const std::string& playerUsername, int roomID);
	~GameElementsGenerator();
    
    //setters and gettters
    bool GetIsBaseSelectionStageActive();
	void SetIsBaseSelectionStageActive(bool isBaseSelectionStageActive);

	bool GetIsRegionSelectionStageActive();
	void SetIsRegionSelectionStageActive(bool isRegionSelectionStageActive);
    
	bool GetIsDuelStageActive();
	void SetIsDuelStageActive(bool isDuelStageActive);
    
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

    void GetSingleChoiceQuestion();

public:
    void TimerMethodToRequestDataFromServer(int time);

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
    
    void TickMethodToRequestDataFromServer();

private:
    bool gotQuestion;
    bool gotSingleQuestion, gotMultipleQuestion;
    
    bool m_gotFirstQuestion;
    bool m_baseSelectionStageActive;
    bool m_regionSelectionStageActive;
	bool m_duelStageActive;
    
    bool m_answerHasBeenSelected;
    bool m_canChooseTerritory;

    uint16_t m_numberOfPlayers = 0;
    //uint16_t m_amountOfRandomQuestionsFromDatabase;
    
    int m_roomID;
    
    std::string m_ip;
    std::string m_playerUsername;
    std::string m_currentAnswer;
    
    QTimer* timer;
    QTimer* timerToCheckServer = new QTimer(this);
    QElapsedTimer elapsedTime;

    QuestionSingleChoice m_singleChoiceQuestion;
    QuestionMultipleChoice m_multipleChoiceQuestion;
    
    //std::vector<QuestionSingleChoice> m_randomSingleChoiceQuestionsVector;
	//std::vector<QuestionMultipleChoice> m_randomMultipleChoiceQuestionsVector;
};