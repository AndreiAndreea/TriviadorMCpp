#include "DataBase.h"

using namespace sqlite_orm;

DatabaseStorage::DatabaseStorage(const std::string& filePathForSingleChoiceQuestion, const std::string& filePathForMultipeChoiceQuestion)
	: m_filePathForSingleChoiceQuestion(filePathForSingleChoiceQuestion),
	m_filePathForMultipeChoiceQuestion(filePathForMultipeChoiceQuestion)
{
	
}

void DatabaseStorage::Initialize()
{
	m_db.sync_schema();

	auto initQuestionsCount = m_db.count<User>();
	
	if (initQuestionsCount == 0)
	{
		m_questions.GetSingleChoiceQuestionsFromFile(m_filePathForSingleChoiceQuestion);
		m_questions.GetMultipleChoiceQuestionsFromFile(m_filePathForMultipeChoiceQuestion);
		
		PopulateDatabaseWithQuestions();

		std::cout << "Database was successfully created!\n";
	}
	else
	{
		std::cout << "Database already exists!\n";
	}
}

void DatabaseStorage::PopulateDatabaseWithQuestions()
{
	std::vector<QuestionSingleChoice> singleChoiceQuestionsVector; //save the questions to export them to database
	std::vector<QuestionMultipleChoice> multipleChoiceQuestionsVector; //save the questions to export them to database

	std::array<std::string, m_numberOfAnswers> multipleChoiceAnswersArray;

	std::string currentQuestionText;
	std::string currentAnswer;

	for (const auto& question : m_questions.GetSingleChoiceQuestionsVector())
		singleChoiceQuestionsVector.emplace_back(QuestionSingleChoice{ 0, question.GetQuestionText(), question.GetAnswer() });

	m_db.insert_range(singleChoiceQuestionsVector.begin(), singleChoiceQuestionsVector.end());

	for (const auto& question : m_questions.GetMultipleChoiceQuestionsVector())
		multipleChoiceQuestionsVector.emplace_back(QuestionMultipleChoice{ 0, question.GetQuestionText(), question.GetAnswers()[0], question.GetAnswers()[1], question.GetAnswers()[2], question.GetAnswers()[3], question.GetAnswers()[4] });

	m_db.insert_range(multipleChoiceQuestionsVector.begin(), multipleChoiceQuestionsVector.end());

	m_db.insert(User(0, "Admin", "Admin28!", "admin@blue-zone.ro", "08/10/2022 at 12:12:00", "999999999", "5", "2"));
}