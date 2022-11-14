#pragma once
#include <string>
#include <array>
#include <iostream>

class QuestionMultipleChoice
{
public:
	QuestionMultipleChoice();
	QuestionMultipleChoice(std::string question, std::array<std::string, 5> answers);

	QuestionMultipleChoice(const QuestionMultipleChoice& object); //copy constructor
	QuestionMultipleChoice(QuestionMultipleChoice&& object); //move constructor

	const std::string& GetQuestion();
	const std::array<std::string, 5>&  GetAnswers();

	friend std::ostream& operator << (std::ostream& out, const QuestionMultipleChoice& qmc);

	bool operator == (const QuestionMultipleChoice& object);

	QuestionMultipleChoice& operator=(const QuestionMultipleChoice& object);
	QuestionMultipleChoice& operator=(QuestionMultipleChoice&& object);

private:
	std::string m_question;
	std::array<std::string, 5> m_answers;
};

