#include "DataBase.h"

DatabaseStorage::DatabaseStorage(const std::string& filePathForSingleChoiceQuestion, const std::string& filePathForMultipeChoiceQuestion)
	: m_filePathForSingleChoiceQuestion { filePathForSingleChoiceQuestion },
	m_filePathForMultipeChoiceQuestion { filePathForMultipeChoiceQuestion }
{

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
	std::vector<User> users = {
		User{ 0, "Sm0k3", "csm1245@", "cosmyn_dobre@yahoo.com", "2022-08-10","39893","5", "2"},
		User{ 0, "MilkAna", "1234AnaLapte", "andreea_andrei@yahoo.com", "2022-11-15", "213131","3","1"}
	};

	m_db.insert_range(users.begin(), users.end());
}