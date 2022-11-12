#include "Questions.h"

Questions::Questions()
{
	GenerateSCQuestions();

	GenerateMCQuestions();
}

void Questions::GenerateSCQuestions()
{
	std::string question;
	uint16_t answer;

	std::ifstream finSC("SingleChoiceQuestions.txt");

	while (!finSC.eof())
	{
		std::getline(finSC, question);
		
		finSC >> answer;

		finSC.get();

		m_questionsSC.push_back(QuestionSingleChoice(question, answer));
	}

	finSC.close();
}

void Questions::GenerateMCQuestions()
{
	std::string question;
	std::array<std::string, 5> answers;
	std::string answer;

	std::ifstream finMC("MultipleChoiceQuestions.txt");

	while (!finMC.eof())
	{
		std::getline(finMC, question);

		for (int index = 0; index < 5; index++)
		{
			std::getline(finMC, answer);
			answers[index] = answer;
		}

		m_questionsMC.push_back(QuestionMultipleChoice(question, answers));
	}

	finMC.close();
}

std::vector<QuestionSingleChoice> Questions::GetQuestionsSC()
{
	return m_questionsSC;
}

std::vector<QuestionMultipleChoice> Questions::GetQuestionsMC()
{
	return m_questionsMC;
}
