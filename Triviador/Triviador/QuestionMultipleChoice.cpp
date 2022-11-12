#include "QuestionMultipleChoice.h"

QuestionMultipleChoice::QuestionMultipleChoice(std::string question, std::array<std::string, 5> answers) 
	: m_question(question),
	m_answers(answers)
{

}

const std::string& QuestionMultipleChoice::GetQuestion()
{
	return m_question;
}

const std::array<std::string, 5>& QuestionMultipleChoice::GetAnswers()
{
	return m_answers;
}
