#pragma once
#include <string>
#include <array>
#include <cstdint>

class QuestionSingleChoice
{
public:
	QuestionSingleChoice(std::string question, uint16_t answer);

	const std::string& GetQuestion();
	const uint16_t& GetAnswer();

private:
	std::string m_question;
	uint16_t m_answer;
	
};