#include "Questions.h"

Questions::Questions()
{

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
