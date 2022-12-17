#include "Questions.h"

Questions::Questions()
{
	//GetSingleChoiceQuestionsFromFile(m_filePathForSingleChoiceQuestions); //need to be deleted when the server is ready

	//GetMultipleChoiceQuestionsFromFile(m_filePathForMultipleChoiceQuestions); //need to be deleted when the server is ready
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
	catch (int errorValue) {
		std::cout << "Error #" << errorValue << ". Invalid index at RemoveSCQuestion method from Questions class." << std::endl;
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
	if (std::find(m_singleChoiceQuestions.begin(), m_singleChoiceQuestions.end(), object) == m_singleChoiceQuestions.end())
		m_singleChoiceQuestions.erase(std::find(m_singleChoiceQuestions.begin(), m_singleChoiceQuestions.end(), object));
}

void Questions::RemoveMultipleChoiceQuestionByObject(QuestionMultipleChoice& object)
{
	if (std::find(m_multipleChoiceQuestions.begin(), m_multipleChoiceQuestions.end(), object) == m_multipleChoiceQuestions.end())
		m_multipleChoiceQuestions.erase(std::find(m_multipleChoiceQuestions.begin(), m_multipleChoiceQuestions.end(), object));
}

uint8_t Questions::GenerateRandomNumber(const uint8_t& size)
{
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

std::vector<QuestionSingleChoice> Questions::GetAFewRandomSingleChoiceQuestions(uint16_t amount)
{
	randomSingleChoiceQuestions.clear();

	if (amount <= m_singleChoiceQuestions.size())
	{
		for (int index = 0; index < amount; index++)
		{
			int position = GenerateRandomNumber(m_singleChoiceQuestions.size());
			randomSingleChoiceQuestions.push_back(m_singleChoiceQuestions[position]);

			m_singleChoiceQuestions.erase(m_singleChoiceQuestions.begin() + position);
		}

		return randomSingleChoiceQuestions;
	}
	else
	{
		throw new std::exception("Invalid amount of questions.");
	}
}

std::vector<QuestionMultipleChoice> Questions::GetAFewRandomMultipleChoiceQuestions(uint16_t amount)
{
	randomMultipleChoiceQuestions.clear();

	if (amount <= m_multipleChoiceQuestions.size())
	{
		for (int index = 0; index < amount; index++)
		{
			int position = GenerateRandomNumber(m_multipleChoiceQuestions.size());
			randomMultipleChoiceQuestions.push_back(m_multipleChoiceQuestions[GenerateRandomNumber(m_multipleChoiceQuestions.size())]);

			m_multipleChoiceQuestions.erase(m_multipleChoiceQuestions.begin() + position);
		}

		return randomMultipleChoiceQuestions;
	}
	else
	{
		throw new std::exception("Invalid amount of questions.");
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

QuestionDatabaseControl::QuestionDatabaseControl(QuestionsStorage& storage)
	: database(storage)
{

}

crow::response QuestionDatabaseControl::operator()(const crow::request& request) const
{
	//NEED TO BE IMPLEMENTED
	
	return crow::response(200);
}