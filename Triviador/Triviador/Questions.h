#pragma once
#include "QuestionMultipleChoice.h"
#include "QuestionSingleChoice.h"

#include <vector>
#include <fstream>

class Questions
{
public:
	Questions();

	std::vector<QuestionSingleChoice> GetQuestionsSC() const;
	std::vector<QuestionMultipleChoice> GetQuestionsMC() const;

public:
	void RemoveSCQuestion(const uint8_t& index);
	void RemoveMCQuestion(const uint8_t& index);

private: 
	void GenerateSCQuestions();
	void GenerateMCQuestions();

private:
	std::vector<QuestionSingleChoice> m_questionsSC;
	std::vector<QuestionMultipleChoice> m_questionsMC;
	

};

