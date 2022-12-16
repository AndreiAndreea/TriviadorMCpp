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

QuestionMultipleChoice::QuestionMultipleChoice(uint16_t id, const std::string& questionText, const std::string& correctAnswer, const std::string& answer1, const std::string& answer2, const std::string& answer3, const std::string& answer4)
{
	m_id = id;
	m_questionText = questionText;
	
	m_answers[0] = correctAnswer;

	m_answers[1] = answer1;
	m_answers[2] = answer2;
	m_answers[3] = answer3;
	m_answers[4] = answer4;
}

QuestionMultipleChoice::QuestionMultipleChoice(const QuestionMultipleChoice& object)
{
	*this = object;
}

QuestionMultipleChoice::QuestionMultipleChoice(QuestionMultipleChoice&& object)
{
	*this = std::move(object);
}

void QuestionMultipleChoice::SetID(uint16_t id)
{
	m_id = id;
}

uint16_t QuestionMultipleChoice::GetID() const
{
	return m_id;
}

void QuestionMultipleChoice::SetQuestionText(const std::string& questionText)
{
	m_questionText = questionText;
}

const std::string& QuestionMultipleChoice::GetQuestionText() const
{
	return m_questionText;
}

void QuestionMultipleChoice::SetCorrectAnswer(const std::string& answer)
{
	m_answers[0] = answer;
}

const std::string& QuestionMultipleChoice::GetCorrectAnswer() const
{
	return m_answers[0];
}

void QuestionMultipleChoice::SetAnswer1(const std::string& answer)
{
	m_answers[1] = answer;
}

const std::string& QuestionMultipleChoice::GetAnswer1() const
{
	return m_answers[1];
}

void QuestionMultipleChoice::SetAnswer2(const std::string& answer)
{
	m_answers[2] = answer;
}

const std::string& QuestionMultipleChoice::GetAnswer2() const
{
	return m_answers[2];
}

void QuestionMultipleChoice::SetAnswer3(const std::string& answer)
{
	m_answers[3] = answer;
}

const std::string& QuestionMultipleChoice::GetAnswer3() const
{
	return m_answers[3];
}

void QuestionMultipleChoice::SetAnswer4(const std::string& answer)
{
	m_answers[4] = answer;
}

const std::string& QuestionMultipleChoice::GetAnswer4() const
{
	return m_answers[4];
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
