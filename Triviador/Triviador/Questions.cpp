#include "Questions.h"

Questions::Questions()
{
	GetSingleChoiceQuestionsFromFile(m_filePathForSingleChoiceQuestions);

	GetMultipleChoiceQuestionsFromFile(m_filePathForMultipleChoiceQuestions);
}

void Questions::GetSingleChoiceQuestionsFromFile(const std::string& filePathSingleChoiceQuestions)
{
	std::string title;
	uint16_t answer;

	std::ifstream inputStream(filePathSingleChoiceQuestions);

	while (!inputStream.eof())
	{
		std::getline(inputStream, title);
		
		inputStream >> answer;

		inputStream.get();

		m_singleChoiceQuestions.push_back(QuestionSingleChoice(title, answer));
	}

	inputStream.close();
}

void Questions::GetMultipleChoiceQuestionsFromFile(const std::string& filePathMultipleChoiceQuestions)
{
	std::string title;
	std::array<std::string, m_numberOfAnswers> answers;
	std::string currentAnswer;

	std::ifstream inputStream(filePathMultipleChoiceQuestions);

	while (!inputStream.eof())
	{
		std::getline(inputStream, title);

		for (int index = 0; index < m_numberOfAnswers; index++)
		{
			std::getline(inputStream, currentAnswer);
			answers[index] = currentAnswer;
		}

		m_multipleChoiceQuestions.push_back(QuestionMultipleChoice(title, answers));
	}

	inputStream.close();
}

std::vector<QuestionSingleChoice> Questions::GetSingleChoiceQuestionsVector() const
{
	return m_singleChoiceQuestions;
}

std::vector<QuestionMultipleChoice> Questions::GetMultipleChoiceQuestionsVector() const
{
	return m_multipleChoiceQuestions;
}

void Questions::RemoveSingleChoiceQuestionByIndex(const uint8_t& index)
{
	try {
		if (index < m_singleChoiceQuestions.size())
		{
			std::vector<QuestionSingleChoice>::iterator it = m_singleChoiceQuestions.begin() + index;
			m_singleChoiceQuestions.erase(it);
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

void Questions::RemoveMultipleChoiceQuestionByIndex(const uint8_t& index)
{
	try {
		if (index < m_multipleChoiceQuestions.size())
		{
			std::vector<QuestionMultipleChoice>::iterator it = m_multipleChoiceQuestions.begin() + index;
			m_multipleChoiceQuestions.erase(it);
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

void Questions::RemoveSingleChoiceQuestionByObject(QuestionSingleChoice& object)
{
	if(std::find(m_singleChoiceQuestions.begin(), m_singleChoiceQuestions.end(), object) == m_singleChoiceQuestions.end())
		m_singleChoiceQuestions.erase(std::find(m_singleChoiceQuestions.begin(), m_singleChoiceQuestions.end(), object));
}

void Questions::RemoveMultipleChoiceQuestionByObject(QuestionMultipleChoice& object)
{
	if (std::find(m_multipleChoiceQuestions.begin(), m_multipleChoiceQuestions.end(), object) == m_multipleChoiceQuestions.end())
		m_multipleChoiceQuestions.erase(std::find(m_multipleChoiceQuestions.begin(), m_multipleChoiceQuestions.end(), object));
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

const QuestionSingleChoice& Questions::GetRandomSingleChoiceQuestion()
{
	uint8_t randomIndex = GenerateRandomNumber(m_singleChoiceQuestions.size());

	return m_singleChoiceQuestions[randomIndex];
}

const QuestionMultipleChoice& Questions::GetRandomMultipleChoiceQuestion()
{
	uint8_t randomIndex = GenerateRandomNumber(m_multipleChoiceQuestions.size());

	return m_multipleChoiceQuestions[randomIndex];
}
