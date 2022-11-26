#include "QuestionMultipleChoice.h"

QuestionMultipleChoice::QuestionMultipleChoice()
{
	m_questionText = "None";
	for (int i = 0; i < m_numberOfAnswers; i++)
	{
		m_answers[i] = "None";
	}
}

QuestionMultipleChoice::QuestionMultipleChoice(const std::string& questionText, std::array<std::string, m_numberOfAnswers> answers)
	: m_questionText(questionText),
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

const std::string& QuestionMultipleChoice::GetQuestionText() const
{
	return m_questionText;
}

const std::array<std::string, m_numberOfAnswers>& QuestionMultipleChoice::GetAnswers() const
{
	return m_answers;
}

bool QuestionMultipleChoice::operator==(const QuestionMultipleChoice& object)
{
	if (this->m_questionText != object.m_questionText)
		return false;

	for (int index = 0; index < m_numberOfAnswers; index++)
		if (this->m_answers[index] != object.m_answers[index])
			return false;

	return true;
}

QuestionMultipleChoice& QuestionMultipleChoice::operator=(const QuestionMultipleChoice& object)
{
	m_questionText = object.m_questionText;
	for (int i = 0; i < m_numberOfAnswers; i++)
	{
		m_answers[i] = object.m_answers[i];
	}

	return *this;
}

QuestionMultipleChoice& QuestionMultipleChoice::operator=(QuestionMultipleChoice&& object)
{
	m_questionText = object.m_questionText;
	for (int i = 0; i < m_numberOfAnswers; i++)
	{
		m_answers[i] = object.m_answers[i];
	}

	new(&object)QuestionMultipleChoice;

	return *this;
}

std::ostream& operator<<(std::ostream& out, const QuestionMultipleChoice& qmc)
{
	out << qmc.m_questionText << std::endl;

	for (int index = 1; index < m_numberOfAnswers; index++)
		out << qmc.m_answers[index] << std::endl;

	return out;
}
