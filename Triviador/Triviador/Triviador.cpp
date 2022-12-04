#include "Triviador.h"
#include "Questions.h"

#include "LoginRegister.h"

#include<QDebug>

#include <sstream>
#include <ctime>
#include <cstdlib>

#include "DataBase.h"

Triviador::Triviador(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	DatabaseStorage storage("SingleChoiceQuestions.txt", "MultipleChoiceQuestions.txt");
	if (!storage.Initialize())
	{
		std::cout << "Failed to initialize the database!";
	}

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

	ui.chooseTerritoryLabel->hide();
	ui.checkAnswerSelection->hide();
	ui.displayAnswerVerdictMultipleChoiceQuestionLabel->hide();
	ui.multipleChoiceAnswer1Button->setChecked(false);

	m_answerHasBeenSelected = false;
}

Triviador::~Triviador()
{
}

bool Triviador::GetCanChooseTerritory()
{
	return m_canChooseTerritory;
}

void Triviador::SetCanChooseTerritory(bool canChooseTerritory)
{
	m_canChooseTerritory = canChooseTerritory;
}

void Triviador::SetNumberOfPlayers(uint16_t numberOfPlayers)
{
	m_numberOfPlayers = numberOfPlayers;	
}

void Triviador::SaveSingleChoiceQuestionsToFile(const QString fileName)
{
	QFile file(QString("%1").arg(fileName));

	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream ostream(&file);

		for (const auto& question : m_questions.GetSingleChoiceQuestionsVector())
			ostream << QString::fromStdString(question.GetQuestionText()) << "\n" << question.GetAnswer() << "\n\n";

		file.close();
	}
}

void Triviador::SaveMultipleChoiceQuestionsToFile(const QString fileName)
{
	QFile file(QString("%1").arg(fileName));

	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream ostream(&file);

		for (const auto& question : m_questions.GetMultipleChoiceQuestionsVector())
		{
			ostream << QString::fromStdString(question.GetQuestionText()) << "\n";

			for (const auto& answer : question.GetAnswers())
				ostream << QString::fromStdString(answer) << "\n";

			ostream << "\n";
		}
		file.close();
	}
}

void Triviador::CheckMultipleChoiceAnswer(const QString chosenAnswer, bool& isCorrectAnswer)
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

void Triviador::on_getRandomQuestionButton_released()
{
	if(m_numberOfPlayers>=2 && m_numberOfPlayers<=4)
	if (ui.getRandomQuestionButton->isEnabled())
	{
		srand(time(0));

		uint8_t randomValue = rand() % 2;

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

			ui.displayAnswerVerdictMultipleChoiceQuestionLabel->hide();
			ui.displayAnswerVerdictSingleChoiceQuestionLabel->hide();

			ui.checkAnswerSelection->hide();
			ui.chooseTerritoryLabel->hide();
			QuestionSingleChoice SCQuestion = m_questions.GetRandomSingleChoiceQuestion();
			m_currentAnswer = std::to_string(SCQuestion.GetAnswer());

			QString scq = QString::fromStdString(SCQuestion.GetQuestionText());
			ui.titleLabel->setText(scq);

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
			ui.checkAnswerSelection->setText("");
			ui.checkAnswerSelection->show();
			m_answerHasBeenSelected = false;
			QuestionMultipleChoice MCQuestion = m_questions.GetRandomMultipleChoiceQuestion();
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

			update();
		}
	}
}

void Triviador::on_submitAnswerButton_released()
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
			}
		}
	}
}

void Triviador::on_multipleChoiceAnswer1Button_released()
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

void Triviador::on_multipleChoiceAnswer2Button_released()
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

void Triviador::on_multipleChoiceAnswer3Button_released()
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

void Triviador::on_multipleChoiceAnswer4Button_released()
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

void Triviador::on_saveQuestionsInFileButton_released()
{
	if (ui.saveQuestionsInFileButton->isChecked())
	{
		SaveSingleChoiceQuestionsToFile("outputSCQuestions.txt");
		SaveMultipleChoiceQuestionsToFile("outputMCQuestions.txt");
	}
}