#pragma once
#include "QuestionMultipleChoice.h"
#include "QuestionSingleChoice.h"

#include <vector>
#include <fstream>

class Questions
{
public:
	Questions();

private: 
	void GenerateSCQuestions();

private:
	std::vector<QuestionSingleChoice> m_questionsSC;
	std::vector<QuestionMultipleChoice> m_questionsMC;
	

};

