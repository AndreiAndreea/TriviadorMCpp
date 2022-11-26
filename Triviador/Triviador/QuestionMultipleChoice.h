#pragma once
#include <string>
#include <array>
#include <iostream>

#define m_numberOfAnswers 5

class QuestionMultipleChoice
{
public:
	QuestionMultipleChoice();
	QuestionMultipleChoice(const std::string& questionText, std::array<std::string, m_numberOfAnswers> answers);

	QuestionMultipleChoice(const QuestionMultipleChoice& object); //copy constructor
	QuestionMultipleChoice(QuestionMultipleChoice&& object); //move constructor

public:
	const std::string& GetQuestionText() const;
	const std::array<std::string, m_numberOfAnswers>& GetAnswers() const;

public:
	friend std::ostream& operator << (std::ostream& out, const QuestionMultipleChoice& qmc);

	bool operator == (const QuestionMultipleChoice& object);

	QuestionMultipleChoice& operator=(const QuestionMultipleChoice& object);
	QuestionMultipleChoice& operator=(QuestionMultipleChoice&& object);

private:
	std::string m_questionText;
	std::array<std::string, m_numberOfAnswers> m_answers;
};

