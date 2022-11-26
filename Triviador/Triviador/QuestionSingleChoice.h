#pragma once
#include <string>
#include <array>
#include <cstdint>
#include <iostream>

#include <QString>
#include <QDebug>
#include <QTextStream>
#include <QFile>

class QuestionSingleChoice
{
public:
	QuestionSingleChoice();
	QuestionSingleChoice(const std::string& question, uint16_t answer);

	QuestionSingleChoice(const QuestionSingleChoice& object); //copy constructor
	QuestionSingleChoice(QuestionSingleChoice&& object); //move constructor

public:
	const std::string& GetQuestionText() const;
	const uint16_t& GetAnswer() const;

public:
	friend std::ostream& operator << (std::ostream& out, const QuestionSingleChoice& qsc);

	bool operator == (const QuestionSingleChoice& object);

	QuestionSingleChoice& operator=(const QuestionSingleChoice& object);
	QuestionSingleChoice& operator=(QuestionSingleChoice&& object);

private:
	std::string m_questionText;
	uint16_t m_answer;
	
};