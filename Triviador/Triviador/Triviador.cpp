#include "Triviador.h"
#include "Questions.h"

#include<QDebug>

Triviador::Triviador(QWidget *parent)
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

void Triviador::on_checkToDisplayQuestionsInFile_released()
{
    if (ui.checkToDisplayQuestionsInFile->isChecked())
    {
        DisplaySCQuestionsInFile("outputSCQuestions.txt");
        DisplayMCQuestionsInFile("outputMCQuestions.txt");
    }
}
