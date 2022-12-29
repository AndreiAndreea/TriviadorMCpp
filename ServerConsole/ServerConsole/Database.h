#pragma once
#include <string>
#include <vector>
#include <fstream>

#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>

#include "Utilities.h"

#include "User.h"
#include "Questions.h"
#include "Lobby.h"

using namespace sqlite_orm;

inline auto createStorage(const std::string& filename)
{
	return make_storage(
		filename,
		make_table(
			"Users",
			make_column("id", &User::GetID, &User::SetID, autoincrement(), primary_key()),
			make_column("username", &User::GetUsername, &User::SetUsername),
			make_column("password", &User::GetPassword, &User::SetPassword),
			make_column("email", &User::GetEmail, &User::SetEmail),
			make_column("accountCreationDate", &User::GetAccountCreationDate, &User::SetAccountCreationDate),
			make_column("score", &User::GetTotalScore, &User::SetTotalScore),
			make_column("playedGames", &User::GetPlayedGames, &User::SetPlayedGames),
			make_column("wonGames", &User::GetWonGames, &User::SetWonGames),
			make_column("connectStatus", &User::GetConnectStatus, &User::SetConnectStatus)
		),
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
		),
		make_table(
			"LobbyDetails",
			make_column("id_lobby", &Lobby::GetLobbyID, &Lobby::SetLobbyID, autoincrement(), primary_key()),
			make_column("game_type", &Lobby::GetGameType, &Lobby::SetGameType),
			make_column("player1", &Lobby::GetPlayer1, &Lobby::SetPlayer1),
			make_column("player2", &Lobby::GetPlayer2, &Lobby::SetPlayer2),
			make_column("player3", &Lobby::GetPlayer3, &Lobby::SetPlayer3),
			make_column("player4", &Lobby::GetPlayer4, &Lobby::SetPlayer4),
			make_column("player5", &Lobby::GetPlayer5, &Lobby::SetPlayer5),
			make_column("player6", &Lobby::GetPlayer6, &Lobby::SetPlayer6)
		)
	);
}

using Storage = decltype(createStorage(""));

class DatabaseStorage
{
public:
	DatabaseStorage(const std::string& filePathForSingleChoiceQuestion, const std::string& filePathForMultipeChoiceQuestion);

public:
	void Initialize();

private:
	void PopulateDatabaseWithData();

private:
	std::string m_filePathForDatabase = "resources/data_files/database/triviador.sqlite";
	
	Storage m_database = createStorage(m_filePathForDatabase);
	
	std::string m_filePathForSingleChoiceQuestion;
	std::string m_filePathForMultipeChoiceQuestion;

	Questions m_questions;
};
