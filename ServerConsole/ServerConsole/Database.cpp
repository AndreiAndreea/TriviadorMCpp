#include "DataBase.h"

using namespace sqlite_orm;

DatabaseStorage::DatabaseStorage(const std::string& filePathForSingleChoiceQuestion, const std::string& filePathForMultipeChoiceQuestion, Storage& storage)
	: m_filePathForSingleChoiceQuestion(filePathForSingleChoiceQuestion),
	m_filePathForMultipeChoiceQuestion(filePathForMultipeChoiceQuestion),
	m_database(storage)
{
}

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
}

crow::response DatabaseStorage::operator()(const crow::request& request) const
{
	auto bodyArgs = ParseUrlArgs(request.body);
	auto end = bodyArgs.end();

	auto roomID = bodyArgs.find("room_id");
	auto gameType = bodyArgs.find("game_type");
	auto gameStatus = bodyArgs.find("game_status");
	auto roomNumber = bodyArgs.find("room_number");
	auto currentNumberOfPlayers = bodyArgs.find("current_numbers_of_players");
	auto maximNumberOfPlayers = bodyArgs.find("maxim_numbers_of_players");
	auto numberOfReadyPlayers = bodyArgs.find("number_of_ready_players");
	auto player1 = bodyArgs.find("player1");
	auto player2 = bodyArgs.find("player2");
	auto player3 = bodyArgs.find("player3");
	auto player4 = bodyArgs.find("player4");
	auto player5 = bodyArgs.find("player5");
	auto player6 = bodyArgs.find("player6");

	if (roomID != end && gameType != end && gameStatus != end && roomNumber != end && currentNumberOfPlayers != end && maximNumberOfPlayers != end && numberOfReadyPlayers != end && player1 != end && player2 != end && player3 != end && player4 != end && player5 != end && player6 != end)
	{
		Room room(std::stoi(roomID->second), gameType->second, gameStatus->second, roomNumber->second, std::stoi(currentNumberOfPlayers->second), std::stoi(maximNumberOfPlayers->second), std::stoi(numberOfReadyPlayers->second), player1->second, player2->second, player3->second, player4->second, player5->second, player6->second);

		m_database.update(room);

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
	m_database.insert(User(0, "test", "test", "test@blue-zone.ro", "01/01/2023 at 12:00:00", "0", "0", "0", "Offline"));
	m_database.insert(User(0, "andre", "andre", "contact@blue-zone.ro", "01/01/2024 at 12:00:00", "0", "0", "0", "Offline"));
	m_database.insert(User(0, "alex", "alex", "contact@blue-zone.ro", "01/01/2025 at 12:00:00", "0", "0", "0", "Offline"));
	m_database.insert(User(0, "asd", "asd", "contact@blue-zone.ro", "01/01/2026 at 12:00:00", "0", "0", "0", "Offline"));

	m_database.insert(Room(0, "2players", EnumGameStatusToString(ROOM_CREATED), "2P_12302022110500", 0, 2, 0, "", "", "", "", "", ""));
	m_database.insert(Room(0, "3players", EnumGameStatusToString(ROOM_CREATED), "3P_12302022120600", 0, 3, 0, "", "", "", "", "", ""));
	m_database.insert(Room(0, "4players", EnumGameStatusToString(ROOM_CREATED), "4P_12302022130700", 0, 4, 0, "", "", "", "", "", ""));
	m_database.insert(Room(0, "customMode", EnumGameStatusToString(ROOM_CREATED), "CM_12302022140800", 0, 6, 0, "", "", "", "", "", ""));
}