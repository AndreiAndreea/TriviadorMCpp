#pragma once
#include "QuestionMultipleChoice.h"
#include "QuestionSingleChoice.h"

#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>

class Questions
{
public:
	Questions();

	std::vector<QuestionSingleChoice> GetQuestionsSC() const;
	std::vector<QuestionMultipleChoice> GetQuestionsMC() const;

public:
	void RemoveSCQuestionByIndex(const uint8_t& index);
	void RemoveMCQuestionByIndex(const uint8_t& index);

	uint8_t GenerateRandomNumber(const uint8_t& size);

	const QuestionSingleChoice& GetRandomSCQuestion();
	const QuestionMultipleChoice& GetRandomMCQuestion();

private: 
	void GenerateSCQuestions();
	void GenerateMCQuestions();

private:
	std::vector<QuestionSingleChoice> m_questionsSC;
	std::vector<QuestionMultipleChoice> m_questionsMC;
	

};

