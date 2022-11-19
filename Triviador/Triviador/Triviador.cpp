#include "Triviador.h"
#include "Questions.h"

#include<QDebug>

#include <sstream>
#include <ctime>
#include <cstdlib>

Triviador::Triviador(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.label_3->hide();
	ui.label_4->hide();
	ui.pushButton_2->hide();
	ui.lineEdit->hide();
	ui.mc_ans1->hide();
	ui.mc_ans2->hide();
	ui.mc_ans3->hide();
	ui.mc_ans4->hide();
	ui.mc_answerVerdict->hide();
	ui.mc_ans1->setChecked(false);
}

Triviador::~Triviador()
{}
void Triviador::DisplaySCQuestionsInFile(QString fileName)
{
	QFile file(QString("%1").arg(fileName));

	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream ostream(&file);

		for (auto& question : m_questions.GetQuestionsSC())
			ostream << QString::fromStdString(question.GetQuestion()) << "\n" << question.GetAnswer() << "\n\n";

		file.close();
	}
}

void Triviador::DisplayMCQuestionsInFile(QString fileName)
{
	QFile file(QString("%1").arg(fileName));

	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream ostream(&file);

		for (auto& question : m_questions.GetQuestionsMC())
		{
			ostream << QString::fromStdString(question.GetQuestion()) << "\n";

			for (auto& answer : question.GetAnswers())
				ostream << QString::fromStdString(answer) << "\n";

			ostream << "\n";
		}
		file.close();
	}
}

void Triviador::CheckMultipleChoiceAnswer(QString chosenAnswer, bool& answer)
{
	std::string correctAnswer = chosenAnswer.toStdString();
	QString text = QString::fromStdString(m_currentAnswer);
	if (correctAnswer == m_currentAnswer)
	{
		ui.mc_answerVerdict->setText("The answer is correct!");
		answer = 1;
	}
	else
	{
		ui.mc_answerVerdict->setText("The correct answer is: " + text);
		answer = 0;
	}
	ui.mc_answerVerdict->show();
}

void Triviador::on_pushButton_clicked()
{
	if (ui.pushButton->isEnabled())
	{
		srand(time(0));
		int randomval = rand() % 2;
		if (randomval == 1)
		{
			ui.lineEdit->clear();
			ui.label_4->clear();

			ui.pushButton_2->show();
			ui.lineEdit->show();
			ui.label_2->hide();

			//radio buttons from mc choice are hidden
			ui.mc_ans1->hide();
			ui.mc_ans2->hide();
			ui.mc_ans3->hide();
			ui.mc_ans4->hide();
			ui.mc_answerVerdict->hide();
			QuestionSingleChoice SCQuestion = m_questions.GetRandomSCQuestion();
			m_currentAnswer = std::to_string(SCQuestion.GetAnswer());

			QString scq = QString::fromStdString(SCQuestion.GetQuestion());
			ui.label->setText(scq);

			update();
		}
		else
		{
			ui.pushButton_2->hide();
			ui.lineEdit->hide();
			ui.label_2->show();
			ui.label_3->hide();
			ui.label_4->hide();
			ui.mc_answerVerdict->hide();

			ui.mc_ans1->show();
			ui.mc_ans2->show();
			ui.mc_ans3->show();
			ui.mc_ans4->show();
			QuestionMultipleChoice MCQuestion = m_questions.GetRandomMCQuestion();
			m_currentAnswer = MCQuestion.GetAnswers()[0];

			QString mcq = QString::fromStdString(MCQuestion.GetQuestion());
			ui.label->setText(mcq);

			std::stringstream ss;
			ui.mc_ans1->setStyleSheet("background-color:light;");
			ui.mc_ans2->setStyleSheet("background-color:light;");
			ui.mc_ans3->setStyleSheet("background-color:light;");
			ui.mc_ans4->setStyleSheet("background-color:light;");
			ui.mc_ans1->setText(QString::fromStdString(MCQuestion.GetAnswers()[1]));
			ui.mc_ans2->setText(QString::fromStdString(MCQuestion.GetAnswers()[2]));
			ui.mc_ans3->setText(QString::fromStdString(MCQuestion.GetAnswers()[3]));
			ui.mc_ans4->setText(QString::fromStdString(MCQuestion.GetAnswers()[4]));
			QString answers = QString::fromStdString(ss.str());
			ui.label_2->setText(answers);

			update();
		}
	}
}

void Triviador::on_pushButton_2_clicked()
{
	ui.label_3->hide();
	if (ui.pushButton_2->isEnabled())
	{
		QString text = ui.lineEdit->text();

		int ok = 1;
		for (int c = 0; c < text.length() && ok == 1; c++)
		{
			if (!text[c].isDigit())
			{
				ok = 0;
			}
		}
		if (ok == 1)
		{
			uint16_t inputAnswer = text.split(" ")[0].toInt();
			uint16_t currentAnswer = std::stoi(m_currentAnswer);

			std::stringstream ss;
			ss << "Correct answer is: " << m_currentAnswer << "\n"
				<< "close by: " << abs(currentAnswer - inputAnswer) << "\n";
			if (abs(currentAnswer - inputAnswer) == 0)
				ss << "Raspunsul este corect!";

			QString displayAnswer = QString::fromStdString(ss.str());
			ui.label_4->setText(displayAnswer);
			ui.label_4->show();
		}
		else
		{
			ui.label_3->show();
			ui.label_4->hide();
		}
	}
}

void Triviador::on_mc_ans1_released()
{
	if (ui.mc_ans1->isEnabled())
	{
		bool answer;
		CheckMultipleChoiceAnswer(ui.mc_ans1->text(), answer);
		if (answer)
		{
			ui.mc_ans1->setStyleSheet("background-color:green;");
		}
		else
		{
			ui.mc_ans1->setStyleSheet("background-color:red;");
		}
	}

}

void Triviador::on_mc_ans2_released()
{
	bool answer;
	if (ui.mc_ans2->isEnabled())
	{
		CheckMultipleChoiceAnswer(ui.mc_ans2->text(), answer);
		if (answer)
		{
			ui.mc_ans2->setStyleSheet("background-color:green;");
		}
		else
		{
			ui.mc_ans2->setStyleSheet("background-color:red;");
		}
	}
}

void Triviador::on_mc_ans3_released()
{
	bool answer = 0;
	if (ui.mc_ans3->isEnabled())
	{
		CheckMultipleChoiceAnswer(ui.mc_ans3->text(), answer);
		if (answer)
		{
			ui.mc_ans3->setStyleSheet("background-color:green;");
		}
		else
		{
			ui.mc_ans3->setStyleSheet("background-color:red;");
		}
	}
}

void Triviador::on_mc_ans4_released()
{
	bool answer;
	if (ui.mc_ans4->isEnabled())
	{
		CheckMultipleChoiceAnswer(ui.mc_ans4->text(), answer);
		if (answer)
		{
			ui.mc_ans4->setStyleSheet("background-color:green;");
		}
		else
		{
			ui.mc_ans4->setStyleSheet("background-color:red;");
		}
	}
}

void Triviador::on_checkToDisplayQuestionsInFile_released()
{
	if (ui.checkToDisplayQuestionsInFile->isChecked())
	{
		DisplaySCQuestionsInFile("outputSCQuestions.txt");
		DisplayMCQuestionsInFile("outputMCQuestions.txt");
	}
}