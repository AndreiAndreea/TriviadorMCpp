#pragma once
#include <string>
#include <array>
#include <iostream>

class QuestionMultipleChoice
{
public:
	QuestionMultipleChoice(std::string question, std::array<std::string, 5> answers);

	const std::string& GetQuestion();
	const std::array<std::string, 5>&  GetAnswers();

	friend std::ostream& operator << (std::ostream& out, const QuestionMultipleChoice& qmc);

	bool operator == (const QuestionMultipleChoice& object);

private:
	std::string m_question;
	std::array<std::string, 5> m_answers;
};

