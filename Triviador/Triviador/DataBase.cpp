#include "DataBase.h"

DatabaseStorage::DatabaseStorage(const std::string& filePathForSingleChoiceQuestion, const std::string& filePathForMultipeChoiceQuestion)
	: m_filePathForSingleChoiceQuestion { filePathForSingleChoiceQuestion },
	m_filePathForMultipeChoiceQuestion { filePathForMultipeChoiceQuestion }
{
	m_questions.GetSingleChoiceQuestionsFromFile(m_filePathForSingleChoiceQuestion);

	m_questions.GetMultipleChoiceQuestionsFromFile(m_filePathForMultipeChoiceQuestion);
}

bool DatabaseStorage::Initialize()
{
	m_db.sync_schema();

	auto initQuestionsCount = m_db.count<User>();
	if (initQuestionsCount == 0)
		PopulateDatabaseWithQuestions();

	auto questionsCount = m_db.count<User>();

	return questionsCount != 0;
}

void DatabaseStorage::PopulateDatabaseWithQuestions()
{
	std::vector<SingleChoiceQuestionWrapper> singleChoiceQuestionsVector; //save the questions for exporting them to database
	std::vector<MultipleChoiceQuestionWrapper> multipleChoiceQuestionsVector; //save the questions for exporting them to database

	std::array<std::string, m_numberOfAnswers> multipleChoiceAnswersArray;

	std::string currentQuestionText;
	std::string currentAnswer;

	for (const auto& question : m_questions.GetSingleChoiceQuestionsVector())
		singleChoiceQuestionsVector.emplace_back(SingleChoiceQuestionWrapper{ 0, question.GetQuestionText(), question.GetAnswer() });

	m_db.insert_range(singleChoiceQuestionsVector.begin(), singleChoiceQuestionsVector.end());

	for (const auto& question : m_questions.GetMultipleChoiceQuestionsVector())
		multipleChoiceQuestionsVector.emplace_back(MultipleChoiceQuestionWrapper{ 0, question.GetQuestionText(), question.GetAnswers()[0], question.GetAnswers()[1], question.GetAnswers()[2], question.GetAnswers()[3], question.GetAnswers()[4] });

	m_db.insert_range(multipleChoiceQuestionsVector.begin(), multipleChoiceQuestionsVector.end());

	std::vector<User> users = {
		User{ 0, "Admin", "Admin28!", "admin@blue-zone.ro", "2022-08-10","999999999","5", "2"}
	};

	m_db.insert_range(users.begin(), users.end());
}