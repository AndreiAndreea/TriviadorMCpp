#include "GameElementsGenerator.h"

GameElementsGenerator::GameElementsGenerator(const std::string& ip, const std::string& playerUsername)
{
	ui.setupUi(this);

	m_ip = ip;
	m_playerUsername = playerUsername;

	/*
	m_amountOfRandomQuestionsFromDatabase = 5;

	DatabaseStorage storage("SingleChoiceQuestions.txt", "MultipleChoiceQuestions.txt");
	if (!storage.Initialize())
	{
		std::cout << "Failed to initialize the database!";
	}

	m_randomSingleChoiceQuestionsVector = m_questions.GetAFewRandomSingleChoiceQuestions(m_amountOfRandomQuestionsFromDatabase);
	m_randomMultipleChoiceQuestionsVector = m_questions.GetAFewRandomMultipleChoiceQuestions(m_amountOfRandomQuestionsFromDatabase);
	*/
	
	ui.titleLabel->setText(" ");

	ui.errorLabel->hide();
	ui.displayProximityCorrectAnswerLabel->hide();
	ui.displayAnswerVerdictSingleChoiceQuestionLabel->hide();
	ui.submitAnswerButton->hide();
	ui.inputAnswerLineEdit->hide();

	ui.multipleChoiceAnswer1Button->hide();
	ui.multipleChoiceAnswer2Button->hide();
	ui.multipleChoiceAnswer3Button->hide();
	ui.multipleChoiceAnswer4Button->hide();
	ui.elapsedTimeLabel->hide();

	ui.fifty_fiftyAdvantageButton->hide();
	ui.suggestAnswerAdvantageButton->hide();
	ui.suggestAnswerAdvantageLabel->hide();
	ui.offerAnswersAdvantageButton->hide();
	HideOfferedAnswers(true);

	ui.chooseTerritoryLabel->hide();
	ui.checkAnswerSelection->hide();
	ui.displayAnswerVerdictMultipleChoiceQuestionLabel->hide();
	ui.multipleChoiceAnswer1Button->setChecked(false);

	m_answerHasBeenSelected = false;

	m_canChooseTerritory = false;
}

GameElementsGenerator::~GameElementsGenerator()
{
}

void GameElementsGenerator::OnTimerTick()
{
	ui.progressBar->setValue(ui.progressBar->value() + 1);

	if (ui.progressBar->value() >= 100)
	{
		ui.getRandomQuestionButton->setDisabled(false);

		ToggleAnswers(true);

		timer->disconnect();
		ui.offerAnswersAdvantageButton->setDisabled(true);
		ui.suggestAnswerAdvantageButton->setDisabled(true);
	}
}

void GameElementsGenerator::ToggleAnswers(bool toggleAnswer)
{
	ui.multipleChoiceAnswer1Button->setDisabled(toggleAnswer);
	ui.multipleChoiceAnswer2Button->setDisabled(toggleAnswer);
	ui.multipleChoiceAnswer3Button->setDisabled(toggleAnswer);
	ui.multipleChoiceAnswer4Button->setDisabled(toggleAnswer);

	ui.inputAnswerLineEdit->setDisabled(toggleAnswer);
	ui.submitAnswerButton->setDisabled(toggleAnswer);
}

void GameElementsGenerator::DisableAdvantageOfferedAnswers(bool toggleAnswer)
{
	ui.offeredAnswer1Button->setDisabled(toggleAnswer);
	ui.offeredAnswer2Button->setDisabled(toggleAnswer);
	ui.offeredAnswer3Button->setDisabled(toggleAnswer);
	ui.offeredAnswer4Button->setDisabled(toggleAnswer);
}

bool GameElementsGenerator::GetCanChooseTerritory()
{
	return m_canChooseTerritory;
}

void GameElementsGenerator::SetCanChooseTerritory(bool canChooseTerritory)
{
	m_canChooseTerritory = canChooseTerritory;
}

void GameElementsGenerator::SetNumberOfPlayers(uint16_t numberOfPlayers)
{
	m_numberOfPlayers = numberOfPlayers;
}

void GameElementsGenerator::StartTimer()
{
	ui.progressBar->setValue(0);

	timer = new QTimer(this);

	timer->setInterval(50);//timer->setInterval(100); - for actual gameplay
	timer->setTimerType(Qt::PreciseTimer);

	connect(timer, SIGNAL(timeout()), this, SLOT(OnTimerTick()));

	timer->start();
}

void GameElementsGenerator::CheckMultipleChoiceAnswer(const QString chosenAnswer, bool& isCorrectAnswer)
{
	std::string correctAnswer = chosenAnswer.toStdString();
	QString text = QString::fromStdString(m_currentAnswer);
	if (correctAnswer == m_currentAnswer)
	{
		if (!m_answerHasBeenSelected)
		{
			ui.displayAnswerVerdictMultipleChoiceQuestionLabel->setText("The answer is correct!");
			ui.chooseTerritoryLabel->setText("Now you can choose a territory!");
			ui.chooseTerritoryLabel->show();
		}

		else
			ui.displayAnswerVerdictMultipleChoiceQuestionLabel->setText("");
		isCorrectAnswer = 1;
	}
	else
	{
		if (!m_answerHasBeenSelected)
			ui.displayAnswerVerdictMultipleChoiceQuestionLabel->setText("The correct answer is: " + text);
		else
			ui.displayAnswerVerdictMultipleChoiceQuestionLabel->setText("");
		isCorrectAnswer = 0;
	}
	ui.displayAnswerVerdictMultipleChoiceQuestionLabel->show();
}

void GameElementsGenerator::HideOfferedAnswers(bool hide)
{
	if (hide == true)
	{
		ui.offeredAnswer1Button->hide();
		ui.offeredAnswer2Button->hide();
		ui.offeredAnswer3Button->hide();
		ui.offeredAnswer4Button->hide();
	}
	else
	{
		ui.offeredAnswer1Button->show();
		ui.offeredAnswer2Button->show();
		ui.offeredAnswer3Button->show();
		ui.offeredAnswer4Button->show();
	}
}

void GameElementsGenerator::on_getRandomQuestionButton_released()
{
	if (m_numberOfPlayers >= 2 && m_numberOfPlayers <= 4)
		if (ui.getRandomQuestionButton->isEnabled())
		{
			srand(time(0));

			uint8_t randomValue = rand() % 2;

			ui.elapsedTimeLabel->hide();

			if (randomValue == 1)
			{
				ui.inputAnswerLineEdit->clear();
				ui.displayProximityCorrectAnswerLabel->clear();

				ui.inputAnswerLineEdit->setDisabled(false);
				ui.submitAnswerButton->setDisabled(false);

				ui.submitAnswerButton->show();
				ui.inputAnswerLineEdit->show();

				//radio buttons from mc choice are hidden
				ui.multipleChoiceAnswer1Button->hide();
				ui.multipleChoiceAnswer2Button->hide();
				ui.multipleChoiceAnswer3Button->hide();
				ui.multipleChoiceAnswer4Button->hide();

				ui.fifty_fiftyAdvantageButton->hide();
				ui.suggestAnswerAdvantageButton->show();
				ui.suggestAnswerAdvantageLabel->hide();
				ui.offerAnswersAdvantageButton->show();
				HideOfferedAnswers(true);

				ui.displayAnswerVerdictMultipleChoiceQuestionLabel->hide();
				ui.displayAnswerVerdictSingleChoiceQuestionLabel->hide();

				ui.checkAnswerSelection->hide();
				ui.chooseTerritoryLabel->hide();

				ui.offerAnswersAdvantageButton->setDisabled(false);
				ui.suggestAnswerAdvantageButton->setDisabled(false);

				/*uint16_t randomPosition = rand() % m_randomSingleChoiceQuestionsVector.size();

				QuestionSingleChoice SCQuestion = m_randomSingleChoiceQuestionsVector.at(randomPosition);
				m_randomSingleChoiceQuestionsVector.erase(m_randomSingleChoiceQuestionsVector.begin() + randomPosition);

				m_currentAnswer = std::to_string(SCQuestion.GetAnswer());

				QString scq = QString::fromStdString(SCQuestion.GetQuestionText());
				ui.titleLabel->setText(scq);*/

				std::string link = m_ip + "/getRandomSingleQuestion";

				cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

				if (responseFromServer.status_code == 200)
				{
					auto questionDetails = crow::json::load(responseFromServer.text);

					std::string quetionText = questionDetails["question"].s();
					ui.titleLabel->setText(QString::fromStdString(quetionText));
					m_currentAnswer = std::to_string(questionDetails["correctAnswer"].i());
				}

				StartTimer();

				update();
			}
			else
			{
				ui.submitAnswerButton->hide();
				ui.inputAnswerLineEdit->hide();
				ui.errorLabel->hide();
				ui.displayProximityCorrectAnswerLabel->hide();
				ui.displayAnswerVerdictMultipleChoiceQuestionLabel->hide();
				ui.displayAnswerVerdictSingleChoiceQuestionLabel->hide();
				ui.chooseTerritoryLabel->hide();

				ui.multipleChoiceAnswer1Button->show();
				ui.multipleChoiceAnswer2Button->show();
				ui.multipleChoiceAnswer3Button->show();
				ui.multipleChoiceAnswer4Button->show();

				ui.fifty_fiftyAdvantageButton->show();
				ui.suggestAnswerAdvantageButton->hide();
				ui.suggestAnswerAdvantageLabel->hide();
				ui.offerAnswersAdvantageButton->hide();
				HideOfferedAnswers(true);

				ui.checkAnswerSelection->setText("");
				ui.checkAnswerSelection->show();
				m_answerHasBeenSelected = false;

				/*
				uint16_t randomPosition = rand() % m_randomMultipleChoiceQuestionsVector.size();

				QuestionMultipleChoice MCQuestion = m_randomMultipleChoiceQuestionsVector.at(randomPosition);
				m_randomMultipleChoiceQuestionsVector.erase(m_randomMultipleChoiceQuestionsVector.begin() + randomPosition);

				m_currentAnswer = MCQuestion.GetAnswers()[0];

				QString mcq = QString::fromStdString(MCQuestion.GetQuestionText());
				ui.titleLabel->setText(mcq);

				std::stringstream ss;
				ui.multipleChoiceAnswer1Button->setStyleSheet("background-color:light;");
				ui.multipleChoiceAnswer2Button->setStyleSheet("background-color:light;");
				ui.multipleChoiceAnswer3Button->setStyleSheet("background-color:light;");
				ui.multipleChoiceAnswer4Button->setStyleSheet("background-color:light;");
				ui.multipleChoiceAnswer1Button->setText(QString::fromStdString(MCQuestion.GetAnswers()[1]));
				ui.multipleChoiceAnswer2Button->setText(QString::fromStdString(MCQuestion.GetAnswers()[2]));
				ui.multipleChoiceAnswer3Button->setText(QString::fromStdString(MCQuestion.GetAnswers()[3]));
				ui.multipleChoiceAnswer4Button->setText(QString::fromStdString(MCQuestion.GetAnswers()[4]));
				QString answers = QString::fromStdString(ss.str());
				*/
				
				std::string link = m_ip + "/getRandomMultipleQuestion";

				cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

				if (responseFromServer.status_code == 200)
				{
					auto questionDetails = crow::json::load(responseFromServer.text);

					std::string quetionText = questionDetails["question"].s();
					ui.titleLabel->setText(QString::fromStdString(quetionText));
					
					m_currentAnswer = questionDetails["correctAnswer"].s();

					std::stringstream ss;
					ui.multipleChoiceAnswer1Button->setStyleSheet("background-color:light;");
					ui.multipleChoiceAnswer2Button->setStyleSheet("background-color:light;");
					ui.multipleChoiceAnswer3Button->setStyleSheet("background-color:light;");
					ui.multipleChoiceAnswer4Button->setStyleSheet("background-color:light;");
					ui.multipleChoiceAnswer1Button->setText(QString::fromStdString(questionDetails["answer1"].s()));
					ui.multipleChoiceAnswer2Button->setText(QString::fromStdString(questionDetails["answer2"].s()));
					ui.multipleChoiceAnswer3Button->setText(QString::fromStdString(questionDetails["answer3"].s()));
					ui.multipleChoiceAnswer4Button->setText(QString::fromStdString(questionDetails["answer4"].s()));
					QString answers = QString::fromStdString(ss.str());
				}
				
				StartTimer();

				update();
			}

			ui.getRandomQuestionButton->setDisabled(true);
			ToggleAnswers(false);
			DisableAdvantageOfferedAnswers(false);

			elapsedTime.start();
		}
}

void GameElementsGenerator::on_submitAnswerButton_released()
{
	ui.errorLabel->hide();

	if (ui.submitAnswerButton->isEnabled())
	{
		if (ui.inputAnswerLineEdit->text().isEmpty())
		{
			ui.errorLabel->setText("Please enter an answer!");
			ui.errorLabel->show();
		}
		else
		{
			QString text = ui.inputAnswerLineEdit->text();

			bool isValidAnswer = true;

			for (int c = 0; c < text.length() && isValidAnswer == true; c++)
				if (!text[c].isDigit())
					isValidAnswer = false;

			if (isValidAnswer == false)
			{
				ui.errorLabel->setText("Please enter a valid answer! (only digits)");
				ui.errorLabel->show();

				ui.displayProximityCorrectAnswerLabel->hide();
			}
			else
			{
				uint16_t inputAnswer = text.split(" ")[0].toInt();
				uint16_t currentAnswer = std::stoi(m_currentAnswer);

				SubmitSingleChoiceAnswer(inputAnswer, currentAnswer);
			}
		}
	}
}

void GameElementsGenerator::on_multipleChoiceAnswer1Button_released()
{
	bool isCorrectAnswer = false;

	if (ui.multipleChoiceAnswer1Button->isEnabled())
	{
		CheckMultipleChoiceAnswer(ui.multipleChoiceAnswer1Button->text(), isCorrectAnswer);

		if (m_answerHasBeenSelected == false)
		{
			if (isCorrectAnswer)
			{
				ui.multipleChoiceAnswer1Button->setStyleSheet("background-color:green;");
				m_canChooseTerritory = true;

				ui.elapsedTimeLabel->show();
				ui.elapsedTimeLabel->setText(QString::number(elapsedTime.elapsed() / 1000) + "," + QString::number(elapsedTime.elapsed() % 1000) + " s");
			}
			else
			{
				ui.multipleChoiceAnswer1Button->setStyleSheet("background-color:red;");
			}
			m_answerHasBeenSelected = true;
		}
		else
			ui.checkAnswerSelection->setText("You have already chosen an answer!");
	}
}

void GameElementsGenerator::on_multipleChoiceAnswer2Button_released()
{
	bool isCorrectAnswer = false;

	if (ui.multipleChoiceAnswer2Button->isEnabled())
	{
		CheckMultipleChoiceAnswer(ui.multipleChoiceAnswer2Button->text(), isCorrectAnswer);
		if (m_answerHasBeenSelected == false)
		{
			if (isCorrectAnswer)
			{
				ui.multipleChoiceAnswer2Button->setStyleSheet("background-color:green;");
				m_canChooseTerritory = true;

				ui.elapsedTimeLabel->show();
				ui.elapsedTimeLabel->setText(QString::number(elapsedTime.elapsed() / 1000) + "," + QString::number(elapsedTime.elapsed() % 1000) + " s");
			}
			else
			{
				ui.multipleChoiceAnswer2Button->setStyleSheet("background-color:red;");
			}
			m_answerHasBeenSelected = true;
		}
		else
			ui.checkAnswerSelection->setText("You have already chosen an answer!");
	}
}

void GameElementsGenerator::on_multipleChoiceAnswer3Button_released()
{
	bool isCorrectAnswer = false;

	if (ui.multipleChoiceAnswer3Button->isEnabled())
	{
		CheckMultipleChoiceAnswer(ui.multipleChoiceAnswer3Button->text(), isCorrectAnswer);

		if (m_answerHasBeenSelected == false)
		{
			if (isCorrectAnswer)
			{
				ui.multipleChoiceAnswer3Button->setStyleSheet("background-color:green;");
				m_canChooseTerritory = true;

				ui.elapsedTimeLabel->show();
				ui.elapsedTimeLabel->setText(QString::number(elapsedTime.elapsed() / 1000) + "," + QString::number(elapsedTime.elapsed() % 1000) + " s");
			}
			else
			{
				ui.multipleChoiceAnswer3Button->setStyleSheet("background-color:red;");
			}
			m_answerHasBeenSelected = true;
		}
		else
			ui.checkAnswerSelection->setText("You have already chosen an answer!");
	}
}

void GameElementsGenerator::on_multipleChoiceAnswer4Button_released()
{
	bool isCorrectAnswer = false;

	if (ui.multipleChoiceAnswer4Button->isEnabled())
	{
		CheckMultipleChoiceAnswer(ui.multipleChoiceAnswer4Button->text(), isCorrectAnswer);

		if (m_answerHasBeenSelected == false)
		{
			if (isCorrectAnswer)
			{
				ui.multipleChoiceAnswer4Button->setStyleSheet("background-color:green;");
				m_canChooseTerritory = true;

				ui.elapsedTimeLabel->show();
				ui.elapsedTimeLabel->setText(QString::number(elapsedTime.elapsed() / 1000) + "," + QString::number(elapsedTime.elapsed() % 1000) + " s");
			}
			else
			{
				ui.multipleChoiceAnswer4Button->setStyleSheet("background-color:red;");
			}
			m_answerHasBeenSelected = true;
		}
		else
			ui.checkAnswerSelection->setText("You have already chosen an answer!");
	}
}

bool GameElementsGenerator::CheckQStringToAnswer(QString text)
{
	QString answer = QString::fromStdString(m_currentAnswer);
	
	if (answer == text)
		return true;
	return false;
}

void GameElementsGenerator::on_fifty_fiftyAdvantageButton_released()
{
	if (ui.fifty_fiftyAdvantageButton->isEnabled())
	{
		srand(time(0));
		int disabledAnswersCounter = 0;
		bool isCorrectAnswer;
		bool oneIsDisabled = false, twoIsDisabled = false, threeIsDisabled = false, fourIsDisabled = false;

		while (disabledAnswersCounter != 2)
		{
			int randomAnswerNumber = rand() % 4 + 1;

			switch (randomAnswerNumber)
			{
			case 1:
				isCorrectAnswer = CheckQStringToAnswer(ui.multipleChoiceAnswer1Button->text());
				if (!isCorrectAnswer && !oneIsDisabled)
				{
					ui.multipleChoiceAnswer1Button->setDisabled(true);
					disabledAnswersCounter++;
					oneIsDisabled = true;
				}
				break;
			case 2:
				isCorrectAnswer = CheckQStringToAnswer(ui.multipleChoiceAnswer2Button->text());
				if (!isCorrectAnswer && !twoIsDisabled)
				{
					ui.multipleChoiceAnswer2Button->setDisabled(true);
					disabledAnswersCounter++;
					twoIsDisabled = true;
				}
				break;
			case 3:
				isCorrectAnswer = CheckQStringToAnswer(ui.multipleChoiceAnswer3Button->text());
				if (!isCorrectAnswer && !threeIsDisabled)
				{
					ui.multipleChoiceAnswer3Button->setDisabled(true);
					disabledAnswersCounter++;
					threeIsDisabled = true;
				}
				break;
			case 4:
				isCorrectAnswer = CheckQStringToAnswer(ui.multipleChoiceAnswer4Button->text());
				if (!isCorrectAnswer && !fourIsDisabled)
				{
					ui.multipleChoiceAnswer4Button->setDisabled(true);
					disabledAnswersCounter++;
					fourIsDisabled = true;
				}
				break;
			}
		}

	}
}

void GameElementsGenerator::on_suggestAnswerAdvantageButton_released()
{
	srand(time(0));
	uint16_t randomValue = std::stoi(m_currentAnswer) + rand() % 10 - rand() % 10;

	ui.suggestAnswerAdvantageLabel->setText(QString::number(randomValue));
	ui.suggestAnswerAdvantageLabel->show();
	ui.offerAnswersAdvantageButton->setDisabled(true);
}

void GameElementsGenerator::on_offerAnswersAdvantageButton_released()
{
	srand(time(0));
	uint16_t randomValue;
	ui.suggestAnswerAdvantageButton->setDisabled(true);

	randomValue = std::stoi(m_currentAnswer) - rand() % 80;
	ui.offeredAnswer1Button->setText(QString::number(randomValue));

	randomValue = std::stoi(m_currentAnswer) - rand() % 20;
	ui.offeredAnswer2Button->setText(QString::number(randomValue));

	randomValue = std::stoi(m_currentAnswer) + rand() % 20;
	ui.offeredAnswer3Button->setText(QString::number(randomValue));

	randomValue = std::stoi(m_currentAnswer) + rand() % 80;
	ui.offeredAnswer4Button->setText(QString::number(randomValue));

	HideOfferedAnswers(false);
}

void GameElementsGenerator::on_offeredAnswer1Button_released()
{
	uint16_t inputAnswer = ui.offeredAnswer1Button->text().toInt();
	uint16_t currentAnswer = std::stoi(m_currentAnswer);

	SubmitSingleChoiceAnswer(inputAnswer, currentAnswer);
	DisableAdvantageOfferedAnswers(true);
}

void GameElementsGenerator::on_offeredAnswer2Button_released()
{
	uint16_t inputAnswer = ui.offeredAnswer2Button->text().toInt();
	uint16_t currentAnswer = std::stoi(m_currentAnswer);

	SubmitSingleChoiceAnswer(inputAnswer, currentAnswer);
	DisableAdvantageOfferedAnswers(true);
}

void GameElementsGenerator::on_offeredAnswer3Button_released()
{
	uint16_t inputAnswer = ui.offeredAnswer3Button->text().toInt();
	uint16_t currentAnswer = std::stoi(m_currentAnswer);

	SubmitSingleChoiceAnswer(inputAnswer, currentAnswer);
	DisableAdvantageOfferedAnswers(true);
}

void GameElementsGenerator::on_offeredAnswer4Button_released()
{
	uint16_t inputAnswer = ui.offeredAnswer4Button->text().toInt();
	uint16_t currentAnswer = std::stoi(m_currentAnswer);

	SubmitSingleChoiceAnswer(inputAnswer, currentAnswer);
	DisableAdvantageOfferedAnswers(true);
}

void GameElementsGenerator::SubmitSingleChoiceAnswer(uint16_t inputAnswer, uint16_t currentAnswer)
{
	std::stringstream ss;
	ss << "Correct answer is: " << m_currentAnswer << "\n"
		<< "close by: " << abs(currentAnswer - inputAnswer) << "\n";

	if (abs(currentAnswer - inputAnswer) == 0)
	{
		ui.displayAnswerVerdictSingleChoiceQuestionLabel->setText("<b><font color=\"green\">The answer is correct!</font></b>");
		ui.displayAnswerVerdictSingleChoiceQuestionLabel->show();
		m_canChooseTerritory = true;
		ui.chooseTerritoryLabel->setText("Now you can choose a territory!");
		ui.chooseTerritoryLabel->show();
	}
	else
	{
		ui.displayAnswerVerdictSingleChoiceQuestionLabel->setText("<b><font color=\"red\">The answer is wrong!</font></b>");
		ui.displayAnswerVerdictSingleChoiceQuestionLabel->show();
	}

	QString displayAnswer = QString::fromStdString(ss.str());
	ui.displayProximityCorrectAnswerLabel->setText(displayAnswer);
	ui.displayProximityCorrectAnswerLabel->show();

	ui.submitAnswerButton->setDisabled(true);
	ui.inputAnswerLineEdit->setDisabled(true);

	ui.elapsedTimeLabel->show();
	ui.elapsedTimeLabel->setText(QString::number(elapsedTime.elapsed() / 1000) + "," + QString::number(elapsedTime.elapsed() % 1000) + " s");
}