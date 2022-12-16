#pragma once
#include <string>
#include <vector>

#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>

#include "Utils.h"

#include "Player.h"
#include "Questions.h"

#include <fstream>

namespace sql = sqlite_orm;

const std::string dbFile = "triviador.sqlite";

inline auto createStorage(const std::string& filename)
{
	return sql::make_storage(
		filename,
		sql::make_table(
			"Users",
			sql::make_column("id", &Player::GetID, &Player::SetID, sql::autoincrement(), sql::primary_key()),
			sql::make_column("username", &Player::GetUsername, &Player::SetUsername),
			sql::make_column("password", &Player::GetPassword, &Player::SetPassword),
			sql::make_column("email", &Player::GetEmail, &Player::SetEmail),
			sql::make_column("accountCreationDate", &Player::GetAccountCreationDate, &Player::SetAccountCreationDate),
			sql::make_column("score", &Player::GetTotalScore, &Player::SetTotalScore),
			sql::make_column("playedGames", &Player::GetPlayedGames, &Player::SetPlayedGames),
			sql::make_column("wonGames", &Player::GetWonGames, &Player::SetWonGames)
		),
		sql::make_table(
			"SingleChoiceQuestions",
			sql::make_column("id", &QuestionSingleChoice::GetID, &QuestionSingleChoice::SetID, sql::autoincrement(), sql::primary_key()),
			sql::make_column("title", &QuestionSingleChoice::GetQuestionText, &QuestionSingleChoice::SetQuestionText),
			sql::make_column("correct_answer", &QuestionSingleChoice::GetAnswer, &QuestionSingleChoice::SetAnswer)
		),
		sql::make_table(
			"MultipleChoiceQuestions",
			sql::make_column("id", &QuestionMultipleChoice::GetID, &QuestionMultipleChoice::SetID, sql::autoincrement(), sql::primary_key()),
			sql::make_column("title", &QuestionMultipleChoice::GetQuestionText, &QuestionMultipleChoice::SetQuestionText),
			sql::make_column("correct_answer", &QuestionMultipleChoice::SetCorrectAnswer, &QuestionMultipleChoice::GetCorrectAnswer),
			sql::make_column("answer_1", &QuestionMultipleChoice::GetAnswer1, &QuestionMultipleChoice::SetAnswer1),
			sql::make_column("answer_2", &QuestionMultipleChoice::GetAnswer2, &QuestionMultipleChoice::SetAnswer2),
			sql::make_column("answer_3", &QuestionMultipleChoice::GetAnswer3, &QuestionMultipleChoice::SetAnswer3),
			sql::make_column("answer_4", &QuestionMultipleChoice::GetAnswer4, &QuestionMultipleChoice::SetAnswer4)
		)
	);
}

using Storage = decltype(createStorage(""));

class DatabaseStorage
{

public:
	DatabaseStorage(const std::string& filePathForSingleChoiceQuestion, const std::string& filePathForMultipeChoiceQuestion);

public:
	bool Initialize();

private:
	void PopulateDatabaseWithQuestions();

private:
	Storage m_db = createStorage(dbFile);

	std::string m_filePathForSingleChoiceQuestion;
	std::string m_filePathForMultipeChoiceQuestion;

	Questions m_questions;
};
