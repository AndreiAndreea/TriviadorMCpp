#pragma once
#include <string>
#include <vector>

#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>

#include "Utils.h"
#include "Player.h"

namespace sql = sqlite_orm;

using Coords = std::pair<uint8_t, uint8_t>;

struct User
{
	int id;
	std::string username;
	std::string password;
	std::string accountCreationDate;
	std::string score;
	std::string playedGames, wonGames;
};

inline auto createStorage(const std::string& filename)
{
	return sql::make_storage(
		//returneaza un obiect specific acestei biblioteci
		//- obiectul este un template -> se va crea de fiecare data un obiect special
		filename,
		sql::make_table(
			"Users",
			sql::make_column("id", &User::id, sql::autoincrement(), sql::primary_key()),
			sql::make_column("name", &User::username),
			sql::make_column("password", &User::password),
			sql::make_column("score", &User::score),
			sql::make_column("playedGames", &User::playedGames),
			sql::make_column("wonGames", &User::wonGames),
			sql::make_column("accountCreationDate", &User::accountCreationDate)
		)
	);
}

using Storage = decltype(createStorage(""));

void populateStorage(Storage& storage);

class DataBase
{
public:
	DataBase(Storage& storage);

private:
	std::string m_IP;

	Storage& m_db;
};