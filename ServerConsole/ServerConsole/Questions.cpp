#include "Questions.h"

Questions::Questions()
{

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

const std::vector<QuestionSingleChoice>& Questions::GetSingleChoiceQuestionsVector() const
{
	return m_singleChoiceQuestions;
}

const std::vector<QuestionMultipleChoice>& Questions::GetMultipleChoiceQuestionsVector() const
{
	return m_multipleChoiceQuestions;
}

Questions::~Questions()
{

}