#pragma once
#include "QuestionMultipleChoice.h"
#include "QuestionSingleChoice.h"

#include <vector>
#include <fstream>

class Questions
{
public:
	Questions();

	std::vector<QuestionSingleChoice> GetQuestionsSC();
	std::vector<QuestionMultipleChoice> GetQuestionsMC();

private: 
	void GenerateSCQuestions();
	void GenerateMCQuestions();

private:
	std::vector<QuestionSingleChoice> m_questionsSC;
	std::vector<QuestionMultipleChoice> m_questionsMC;
	

};

