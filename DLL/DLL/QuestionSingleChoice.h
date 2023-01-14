#pragma once
#include <string>
#include <array>
#include <cstdint>
#include <iostream>

#include "LibDefine.h"

class DLL_API QuestionSingleChoice
{
public:
	QuestionSingleChoice();
	QuestionSingleChoice(const std::string& question, uint16_t answer);
	QuestionSingleChoice(uint16_t id, const std::string& questionText, uint16_t answer);

	QuestionSingleChoice(const QuestionSingleChoice& object); //copy constructor
	QuestionSingleChoice(QuestionSingleChoice&& object); //move constructor

public:
	void SetQuestionText(const std::string& username);
	const std::string& GetQuestionText() const;

	void SetAnswer(uint16_t answer);
	uint16_t GetAnswer() const;

	void SetID(uint16_t id);
	uint16_t GetID() const;

public:
	friend std::ostream& operator << (std::ostream& out, const QuestionSingleChoice& qsc);

	bool operator == (const QuestionSingleChoice& object);

	QuestionSingleChoice& operator=(const QuestionSingleChoice& object);
	QuestionSingleChoice& operator=(QuestionSingleChoice&& object);

private:
	uint16_t m_id;
	std::string m_questionText;
	uint16_t m_answer;

};