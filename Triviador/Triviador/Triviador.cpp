#include "Triviador.h"
#include "Questions.h"

#include<QDebug>
#include <ctime>
#include <cstdlib>

Triviador::Triviador(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

}

Triviador::~Triviador()
{}

void Triviador::setLabelText(QString& text)
{
    ui.label->setText(text);
}

void Triviador::DisplaySCQuestionsInFile(QString fileName)
{
    QFile file(QString("%1").arg(fileName));

    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream ostream(&file);

        for (auto& question : questions.GetQuestionsSC())
            ostream << QString::fromStdString(question.GetQuestion()) << "\n" << question.GetAnswer() <<"\n\n";

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
            QuestionSingleChoice SCQuestion = questions.GetRandomSCQuestion();
            QString scq = QString::fromStdString(SCQuestion.GetQuestion());
            setLabelText(scq);
        }
        else
        {
            QuestionMultipleChoice MCQuestion = questions.GetRandomMCQuestion();
            QString mcq = QString::fromStdString(MCQuestion.GetQuestion());
            setLabelText(mcq);
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
