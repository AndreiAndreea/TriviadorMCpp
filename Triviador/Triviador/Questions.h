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

	std::vector<QuestionSingleChoice> GetSingleChoiceQuestionsVector() const;
	std::vector<QuestionMultipleChoice> GetMultipleChoiceQuestionsVector() const;

public:
	void RemoveSingleChoiceQuestionByIndex(const uint8_t& index);
	void RemoveMultipleChoiceQuestionByIndex(const uint8_t& index);

	void RemoveSingleChoiceQuestionByObject(QuestionSingleChoice& object);
	void RemoveMultipleChoiceQuestionByObject(QuestionMultipleChoice& object);

	uint8_t GenerateRandomNumber(const uint8_t& size);

	const QuestionSingleChoice& GetRandomSingleChoiceQuestion();
	const QuestionMultipleChoice& GetRandomMultipleChoiceQuestion();

private: 
	//std::string m_filePathForSingleChoiceQuestions = "SingleChoiceQuestions.txt";
	//std::string m_filePathForMultipleChoiceQuestions = "MultipleChoiceQuestions.txt";

public:
	void GetSingleChoiceQuestionsFromFile(const std::string& filePathSingleChoiceQuestions);
	void GetMultipleChoiceQuestionsFromFile(const std::string& filePathMultipleChoiceQuestions);

private:
	std::vector<QuestionSingleChoice> m_singleChoiceQuestions;
	std::vector<QuestionMultipleChoice> m_multipleChoiceQuestions;
	

};

