#pragma once
#include <string>
#include <array>
#include <iostream>

#include "LibDefine.h"

#define m_numberOfAnswers 5

class DLL_API QuestionMultipleChoice
{
public:
	QuestionMultipleChoice();
	QuestionMultipleChoice(const std::string& questionText, std::array<std::string, m_numberOfAnswers> answers);
	QuestionMultipleChoice(uint16_t id, const std::string& questionText, const std::string& correctAnswer, const std::string& answer1, const std::string& answer2, const std::string& answer3, const std::string& answer4);

	QuestionMultipleChoice(const QuestionMultipleChoice& object); //copy constructor
	QuestionMultipleChoice(QuestionMultipleChoice&& object); //move constructor

public:
	void SetID(uint16_t id);
	uint16_t GetID() const;

	void SetQuestionText(const std::string& questionText);
	const std::string& GetQuestionText() const;

	void SetCorrectAnswer(const std::string& answer);
	const std::string& GetCorrectAnswer() const;

	void SetAnswer1(const std::string& answer);
	const std::string& GetAnswer1() const;

	void SetAnswer2(const std::string& answer);
	const std::string& GetAnswer2() const;

	void SetAnswer3(const std::string& answer);
	const std::string& GetAnswer3() const;

	void SetAnswer4(const std::string& answer);
	const std::string& GetAnswer4() const;

	const std::array<std::string, m_numberOfAnswers>& GetAnswers() const;

public:
	friend std::ostream& operator << (std::ostream& out, const QuestionMultipleChoice& qmc);

	bool operator == (const QuestionMultipleChoice& object);

	QuestionMultipleChoice& operator=(const QuestionMultipleChoice& object);
	QuestionMultipleChoice& operator=(QuestionMultipleChoice&& object);

private:
	uint16_t m_id;
	std::string m_questionText;
	std::array<std::string, m_numberOfAnswers> m_answers;
};

