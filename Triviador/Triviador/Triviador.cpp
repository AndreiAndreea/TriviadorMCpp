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

}

Triviador::~Triviador()
{}

void Triviador::DisplaySCQuestionsInFile(QString fileName)
{
	QFile file(QString("%1").arg(fileName));

	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream ostream(&file);

		for (auto& question : questions.GetQuestionsSC())
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

		for (auto& question : questions.GetQuestionsMC())
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
			ui.pushButton_2->show();
			ui.lineEdit->show();
			ui.label_2->hide();

			QuestionSingleChoice SCQuestion = questions.GetRandomSCQuestion();

			QString scq = QString::fromStdString(SCQuestion.GetQuestion());
			ui.label->setText(scq);

			update();
		}
		else
		{
			ui.pushButton_2->hide();
			ui.lineEdit->hide();
			ui.label_2->show();

			QuestionMultipleChoice MCQuestion = questions.GetRandomMCQuestion();

			QString mcq = QString::fromStdString(MCQuestion.GetQuestion());
			ui.label->setText(mcq);

			std::stringstream ss;
			for (int i = 1; i < 5; i++)
				ss << MCQuestion.GetAnswers()[i]<<"\n";

			QString answers = QString::fromStdString(ss.str());
			ui.label_2->setText(answers);

			update();
		}
	}
}

void Triviador::on_pushButton_2_clicked()
{
	if (ui.pushButton_2->isEnabled())
	{
		QString text = ui.lineEdit->text();

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
