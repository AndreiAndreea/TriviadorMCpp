#pragma once

#include "QuestionSingleChoice.h"
#include "QuestionMultipleChoice.h"

#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>

class Questions
{
public:
	Questions();

	const std::vector<QuestionSingleChoice>& GetSingleChoiceQuestionsVector() const;
	const std::vector<QuestionMultipleChoice>& GetMultipleChoiceQuestionsVector() const;

	~Questions();

public:
	void GetSingleChoiceQuestionsFromFile(const std::string& filePathSingleChoiceQuestions);
	void GetMultipleChoiceQuestionsFromFile(const std::string& filePathMultipleChoiceQuestions);

private:
	std::vector<QuestionSingleChoice> m_singleChoiceQuestions;
	std::vector<QuestionMultipleChoice> m_multipleChoiceQuestions;
};
