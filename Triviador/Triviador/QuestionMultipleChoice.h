#pragma once
#include <string>
#include <array>

class QuestionMultipleChoice
{
public:
	QuestionMultipleChoice(std::string question, std::array<std::string, 5> answers);

	const std::string& GetQuestion();
	const std::array<std::string, 5>&  GetAnswers();

private:
	std::string m_question;
	std::array<std::string, 5> m_answers;
};

