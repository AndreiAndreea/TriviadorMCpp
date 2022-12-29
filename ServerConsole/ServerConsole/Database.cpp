#include "DataBase.h"

using namespace sqlite_orm;

//DatabaseStorage::DatabaseStorage(const std::string& filePathForSingleChoiceQuestion, const std::string& filePathForMultipeChoiceQuestion)
//	: m_filePathForSingleChoiceQuestion(filePathForSingleChoiceQuestion),
//	m_filePathForMultipeChoiceQuestion(filePathForMultipeChoiceQuestion)
//{
//}

DatabaseStorage::DatabaseStorage(Storage& storage)
	: m_database(storage)
{
}

void DatabaseStorage::Initialize()
{
	m_database.sync_schema();

	auto initQuestionsCount = m_database.count<User>();
	
	if (initQuestionsCount == 0)
	{
		m_questions.GetSingleChoiceQuestionsFromFile(m_filePathForSingleChoiceQuestion);
		m_questions.GetMultipleChoiceQuestionsFromFile(m_filePathForMultipeChoiceQuestion);
		
		PopulateDatabaseWithData();

		std::cout << "Database was successfully created!\n";
	}
	else
	{
		std::cout << "Database already exists!\n";
	}
}

crow::response DatabaseStorage::operator()(const crow::request& request) const
{
	auto bodyArgs = ParseUrlArgs(request.body); //id=2&quantity=3&...
	auto end = bodyArgs.end();

	auto lobbyID = bodyArgs.find("lobby_id");
	auto gameType = bodyArgs.find("game_type");
	auto player1 = bodyArgs.find("player1");
	auto player2 = bodyArgs.find("player2");
	auto player3 = bodyArgs.find("player3");
	auto player4 = bodyArgs.find("player4");
	auto player5 = bodyArgs.find("player5");
	auto player6 = bodyArgs.find("player6");

	if (lobbyID != end && gameType != end && player1 != end && player2 != end && player3 != end && player4 != end && player5 != end && player6 != end)
	{
		Lobby lobby(std::stoi(lobbyID->second), gameType->second, player1->second, player2->second, player3->second, player4->second, player5->second, player6->second);

		m_database.update(lobby);

		return crow::response(200);
	}
	else
	{
		return crow::response(400);
	}	
}

void DatabaseStorage::PopulateDatabaseWithData()
{
	std::vector<QuestionSingleChoice> singleChoiceQuestionsVector; //save the questions to export them to database
	std::vector<QuestionMultipleChoice> multipleChoiceQuestionsVector; //save the questions to export them to database

	std::array<std::string, m_numberOfAnswers> multipleChoiceAnswersArray;

	std::string currentQuestionText;
	std::string currentAnswer;

	for (const auto& question : m_questions.GetSingleChoiceQuestionsVector())
		singleChoiceQuestionsVector.emplace_back(QuestionSingleChoice{ 0, question.GetQuestionText(), question.GetAnswer() });

	m_database.insert_range(singleChoiceQuestionsVector.begin(), singleChoiceQuestionsVector.end());

	for (const auto& question : m_questions.GetMultipleChoiceQuestionsVector())
		multipleChoiceQuestionsVector.emplace_back(QuestionMultipleChoice{ 0, question.GetQuestionText(), question.GetAnswers()[0], question.GetAnswers()[1], question.GetAnswers()[2], question.GetAnswers()[3], question.GetAnswers()[4] });

	m_database.insert_range(multipleChoiceQuestionsVector.begin(), multipleChoiceQuestionsVector.end());
		
	m_database.insert(User(0, "Admin", "Admin28!", "admin@blue-zone.ro", "08/10/2022 at 12:12:00", "999999999", "5", "2", "Offline"));

	m_database.insert(Lobby(0, "2players", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL"));
	m_database.insert(Lobby(0, "3players", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL"));
	m_database.insert(Lobby(0, "4players", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL"));
	m_database.insert(Lobby(0, "customMode", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL"));
}