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

			QuestionMultipleChoice MCQuestion = m_questions.GetRandomMCQuestion();
			m_currentAnswer = MCQuestion.GetAnswers()[0];

			QString mcq = QString::fromStdString(MCQuestion.GetQuestion());
			ui.label->setText(mcq);

			std::stringstream ss;
			for (int i = 1; i < 5; i++)
				ss << MCQuestion.GetAnswers()[i] << "\n";

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
				<< "close by: "<< abs(currentAnswer-inputAnswer);

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
void Triviador::on_checkToDisplayQuestionsInFile_released()
{
	if (ui.checkToDisplayQuestionsInFile->isChecked())
	{
		DisplaySCQuestionsInFile("outputSCQuestions.txt");
		DisplayMCQuestionsInFile("outputMCQuestions.txt");
	}
}
