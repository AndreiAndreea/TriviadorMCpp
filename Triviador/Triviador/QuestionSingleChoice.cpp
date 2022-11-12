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
