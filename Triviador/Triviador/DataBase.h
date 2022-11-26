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

struct User
{
	uint32_t id;

	std::string username;
	std::string password;
	std::string email;

	std::string accountCreationDate;

	std::string score;
	std::string playedGames;
	std::string wonGames;
};

inline auto createStorage(const std::string& filename)
{
	return sql::make_storage(
		filename,
		sql::make_table(
			"Users",
			sql::make_column("id", &User::id, sql::autoincrement(), sql::primary_key()),
			sql::make_column("username", &User::username),
			sql::make_column("password", &User::password),
			sql::make_column("email", &User::email),
			sql::make_column("accountCreationDate", &User::accountCreationDate),
			sql::make_column("score", &User::score),
			sql::make_column("playedGames", &User::playedGames),
			sql::make_column("wonGames", &User::wonGames)
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
};
