#pragma once
#include <string>
#include <array>
#include <cstdint>
#include <iostream>

class QuestionSingleChoice
{
public:
	QuestionSingleChoice(std::string question, uint16_t answer);

	const std::string& GetQuestion();
	const uint16_t& GetAnswer();

	friend std::ostream& operator << (std::ostream& out, const QuestionSingleChoice& qsc);

	bool operator == (const QuestionSingleChoice& object);

private:
	std::string m_question;
	uint16_t m_answer;
	
};