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

bool QuestionMultipleChoice::operator==(const QuestionMultipleChoice& object)
{
	if (this->m_question != object.m_question)
		return false;

	for (int index = 0; index < 5; index++)
		if (this->m_answers[index] != object.m_answers[index])
			return false;

	return true;
}

std::ostream& operator<<(std::ostream& out, const QuestionMultipleChoice& qmc)
{
	out << qmc.m_question << std::endl;

	for (int index = 1; index < 5; index++)
		out << qmc.m_answers[index] << std::endl;

	return out;
}
