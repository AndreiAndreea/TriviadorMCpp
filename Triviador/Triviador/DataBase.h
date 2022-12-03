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

struct SingleChoiceQuestionWrapper
{
	uint32_t id;

	std::string questionText;
	uint16_t correctAnswer;
};

struct MultipleChoiceQuestionWrapper
{
	uint32_t id;

	std::string questionText;
	std::string correctAnswer;

	std::string answer1;
	std::string answer2;
	std::string answer3;
	std::string answer4;
};

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
			sql::make_column("id", &SingleChoiceQuestionWrapper::id, sql::autoincrement(), sql::primary_key()),
			sql::make_column("title", &SingleChoiceQuestionWrapper::questionText),
			sql::make_column("correct_answer", &SingleChoiceQuestionWrapper::correctAnswer)
		),
		sql::make_table(
			"MultipleChoiceQuestions",
			sql::make_column("id", &MultipleChoiceQuestionWrapper::id, sql::autoincrement(), sql::primary_key()),
			sql::make_column("title", &MultipleChoiceQuestionWrapper::questionText),
			sql::make_column("correct_answer", &MultipleChoiceQuestionWrapper::correctAnswer),
			sql::make_column("answer_1", &MultipleChoiceQuestionWrapper::answer1),
			sql::make_column("answer_2", &MultipleChoiceQuestionWrapper::answer2),
			sql::make_column("answer_3", &MultipleChoiceQuestionWrapper::answer3),
			sql::make_column("answer_4", &MultipleChoiceQuestionWrapper::answer4)
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
