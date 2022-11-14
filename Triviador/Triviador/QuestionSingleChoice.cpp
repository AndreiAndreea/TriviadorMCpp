#include "QuestionSingleChoice.h"

QuestionSingleChoice::QuestionSingleChoice()
{
	m_question = "None";
	m_answer = 0;
}

QuestionSingleChoice::QuestionSingleChoice(std::string question, uint16_t answer)
	: m_question(question),
	m_answer(answer)
{

}

QuestionSingleChoice::QuestionSingleChoice(const QuestionSingleChoice& object)
{
	*this = object;
}

QuestionSingleChoice::QuestionSingleChoice(QuestionSingleChoice&& object)
{
	*this = std::move(object);
}

std::string& QuestionSingleChoice::GetQuestion()
{
	return m_question;
}

const uint16_t& QuestionSingleChoice::GetAnswer()
{
	return m_answer;
}

bool QuestionSingleChoice::operator==(const QuestionSingleChoice& object)
{
	if (this->m_answer == object.m_answer && this->m_question == object.m_question)
		return true;

	return false;
}

QuestionSingleChoice& QuestionSingleChoice::operator=(const QuestionSingleChoice& object)
{
	m_question = object.m_question;
	m_answer = object.m_answer;

	return *this;
}

QuestionSingleChoice& QuestionSingleChoice::operator=(QuestionSingleChoice&& object)
{
	m_question = object.m_question;
	m_answer = object.m_answer;

	new(&object)QuestionSingleChoice;

	return *this;
}

std::ostream& operator<<(std::ostream& out, const QuestionSingleChoice& qsc)
{
	out << qsc.m_question << std::endl;

	return out;
}
