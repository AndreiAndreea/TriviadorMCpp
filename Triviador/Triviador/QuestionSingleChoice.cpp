#include "QuestionSingleChoice.h"

QuestionSingleChoice::QuestionSingleChoice(std::string question, uint16_t answer)
	: m_question(question),
	m_answer(answer)
{

}

const std::string& QuestionSingleChoice::GetQuestion()
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

std::ostream& operator<<(std::ostream& out, const QuestionSingleChoice& qsc)
{
	out << qsc.m_question << std::endl;

	return out;
}
