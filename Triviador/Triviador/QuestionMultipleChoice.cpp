#include "QuestionMultipleChoice.h"

QuestionMultipleChoice::QuestionMultipleChoice()
{
	m_question = "None";
	for (int i = 0; i < 5; i++)
	{
		m_answers[i] = "None";
	}
}

QuestionMultipleChoice::QuestionMultipleChoice(std::string question, std::array<std::string, 5> answers)
	: m_question(question),
	m_answers(answers)
{

}

QuestionMultipleChoice::QuestionMultipleChoice(const QuestionMultipleChoice& object)
{
	*this = object;
}

QuestionMultipleChoice::QuestionMultipleChoice(QuestionMultipleChoice&& object)
{
	*this = std::move(object);
}

const std::string& QuestionMultipleChoice::GetQuestion()
{
	return m_question;
}

const std::array<std::string, 5>& QuestionMultipleChoice::GetAnswers()
{
	return m_answers;
}

bool QuestionMultipleChoice::operator==(const QuestionMultipleChoice& object)
{
	if (this->m_question != object.m_question)
		return false;

	for (int index = 0; index < 5; index++)
		if (this->m_answers[index] != object.m_answers[index])
			return false;

	return true;
}

QuestionMultipleChoice& QuestionMultipleChoice::operator=(const QuestionMultipleChoice& object)
{
	m_question = object.m_question;
	for (int i = 0; i < 5; i++)
	{
		m_answers[i] = object.m_answers[i];
	}

	return *this;
}

QuestionMultipleChoice& QuestionMultipleChoice::operator=(QuestionMultipleChoice&& object)
{
	m_question = object.m_question;
	for (int i = 0; i < 5; i++)
	{
		m_answers[i] = object.m_answers[i];
	}

	new(&object)QuestionMultipleChoice;

	return *this;
}

std::ostream& operator<<(std::ostream& out, const QuestionMultipleChoice& qmc)
{
	out << qmc.m_question << std::endl;

	for (int index = 1; index < 5; index++)
		out << qmc.m_answers[index] << std::endl;

	return out;
}
