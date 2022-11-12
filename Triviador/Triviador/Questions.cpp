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

std::vector<QuestionSingleChoice> Questions::GetQuestionsSC() const
{
	return m_questionsSC;
}

std::vector<QuestionMultipleChoice> Questions::GetQuestionsMC() const
{
	return m_questionsMC;
}

void Questions::RemoveSCQuestionByIndex(const uint8_t& index)
{
	try {
		if (index < m_questionsSC.size())
		{
			std::vector<QuestionSingleChoice>::iterator it = m_questionsSC.begin() + index;
			m_questionsSC.erase(it);
		}
		else
		{
			throw 1;
		}
	}
	catch(int errorValue) {
			std::cout<< "Error #" << errorValue << ". Invalid index at RemoveSCQuestion method from Questions class." << std::endl;
	}
}

void Questions::RemoveMCQuestionByIndex(const uint8_t& index)
{
	try {
		if (index < m_questionsMC.size())
		{
			std::vector<QuestionMultipleChoice>::iterator it = m_questionsMC.begin() + index;
			m_questionsMC.erase(it);
		}
		else
		{
			throw 2;
		}
	}
	catch (int errorValue) {
		std::cout << "Error #" << errorValue << ". Invalid index at RemoveMCQuestion method from Questions class." << std::endl;
	}
}

uint8_t Questions::GenerateRandomNumber(const uint8_t& size)
{
	srand((int)time(0));

	try {
		if (size > 0)
			return (rand() % size) + 0;
		else
			throw 3;
	}
	catch (int errorValue) {
		std::cout << "Error #" << errorValue << ". Invalid size at GenerateRandomNumber method from Questions class." << std::endl;
	}
}

const QuestionSingleChoice& Questions::GetRandomSCQuestion()
{
	uint8_t randomIndex = GenerateRandomNumber(m_questionsSC.size());

	return m_questionsSC[randomIndex];
}

const QuestionMultipleChoice& Questions::GetRandomMCQuestion()
{
	uint8_t randomIndex = GenerateRandomNumber(m_questionsMC.size());

	return m_questionsMC[randomIndex];
}
