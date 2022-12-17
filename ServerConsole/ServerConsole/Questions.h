#pragma once
#include "QuestionMultipleChoice.h"
#include "QuestionSingleChoice.h"

#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>

#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>

#include "Utilities.h"

using namespace sqlite_orm;

class Questions
{
public:
	Questions();

	const std::vector<QuestionSingleChoice>& GetSingleChoiceQuestionsVector() const;
	const std::vector<QuestionMultipleChoice>& GetMultipleChoiceQuestionsVector() const;

public:
	void RemoveSingleChoiceQuestionByIndex(const uint8_t& index);
	void RemoveMultipleChoiceQuestionByIndex(const uint8_t& index);

	void RemoveSingleChoiceQuestionByObject(QuestionSingleChoice& object);
	void RemoveMultipleChoiceQuestionByObject(QuestionMultipleChoice& object);

	uint8_t GenerateRandomNumber(const uint8_t& size);

public:
	std::vector<QuestionSingleChoice> GetAFewRandomSingleChoiceQuestions(uint16_t amount);
	std::vector<QuestionMultipleChoice> GetAFewRandomMultipleChoiceQuestions(uint16_t amount);

	const QuestionSingleChoice& GetRandomSingleChoiceQuestion();
	const QuestionMultipleChoice& GetRandomMultipleChoiceQuestion();

private:
	std::string m_filePathForSingleChoiceQuestions = "SingleChoiceQuestions.txt"; //need to be deleted when the server is ready
	std::string m_filePathForMultipleChoiceQuestions = "MultipleChoiceQuestions.txt"; //need to be deleted when the server is ready

public:
	void GetSingleChoiceQuestionsFromFile(const std::string& filePathSingleChoiceQuestions);
	void GetMultipleChoiceQuestionsFromFile(const std::string& filePathMultipleChoiceQuestions);

private:
	std::vector<QuestionSingleChoice> m_singleChoiceQuestions;
	std::vector<QuestionMultipleChoice> m_multipleChoiceQuestions;

	std::vector<QuestionSingleChoice> randomSingleChoiceQuestions;
	std::vector<QuestionMultipleChoice> randomMultipleChoiceQuestions;

};

inline auto CreateQuestionsStorage(const std::string& fileName)
{
	return make_storage(
		fileName,
		make_table(
			"SingleChoiceQuestions",
			make_column("id", &QuestionSingleChoice::GetID, &QuestionSingleChoice::SetID, autoincrement(), primary_key()),
			make_column("title", &QuestionSingleChoice::GetQuestionText, &QuestionSingleChoice::SetQuestionText),
			make_column("correct_answer", &QuestionSingleChoice::GetAnswer, &QuestionSingleChoice::SetAnswer)
		),
		make_table(
			"MultipleChoiceQuestions",
			make_column("id", &QuestionMultipleChoice::GetID, &QuestionMultipleChoice::SetID, autoincrement(), primary_key()),
			make_column("title", &QuestionMultipleChoice::GetQuestionText, &QuestionMultipleChoice::SetQuestionText),
			make_column("correct_answer", &QuestionMultipleChoice::SetCorrectAnswer, &QuestionMultipleChoice::GetCorrectAnswer),
			make_column("answer_1", &QuestionMultipleChoice::GetAnswer1, &QuestionMultipleChoice::SetAnswer1),
			make_column("answer_2", &QuestionMultipleChoice::GetAnswer2, &QuestionMultipleChoice::SetAnswer2),
			make_column("answer_3", &QuestionMultipleChoice::GetAnswer3, &QuestionMultipleChoice::SetAnswer3),
			make_column("answer_4", &QuestionMultipleChoice::GetAnswer4, &QuestionMultipleChoice::SetAnswer4)
		)
	);
}

using QuestionsStorage = decltype(CreateQuestionsStorage(""));

class QuestionDatabaseControl
{
private:
	QuestionsStorage& database;

public:
	QuestionDatabaseControl(QuestionsStorage& storage);

	crow::response operator() (const crow::request& request) const;
};
