#pragma once

#include <string>
#include <iostream>
#include <cstdint>
#include <vector>

#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>

#include "Utilities.h"

using namespace sqlite_orm;

class User
{
public:
	User();
	User(uint32_t id, const std::string& username, const std::string& password, const std::string& email, const std::string& accountCreationDate, const std::string& totalScore, const std::string& playedGames, const std::string& wonGames);
	
public:
	void SetID(uint32_t id);
	uint32_t GetID() const;
	
	void SetUsername(const std::string& username);
	const std::string& GetUsername() const;

	void SetPassword(const std::string& password);
	const std::string& GetPassword() const;

	void SetEmail(const std::string& email);
	const std::string& GetEmail() const;

	void SetAccountCreationDate(const std::string& accountCreationDate);
	const std::string& GetAccountCreationDate() const;

	void SetTotalScore(const std::string& totalScore);
	const std::string& GetTotalScore() const;

	void SetPlayedGames(const std::string& playedGames);
	const std::string& GetPlayedGames() const;

	void SetWonGames(const std::string& wonGames);
	const std::string& GetWonGames() const;

private:
	uint32_t m_id;

	std::string m_username;
	std::string m_password;
	std::string m_email;
	std::string m_accountCreationDate;
	
	std::string m_totalScore;

	std::string m_playedGames;
	std::string m_wonGames;
};

inline auto CreateStorage(const std::string& fileName)
{
	return make_storage(
		fileName,
		make_table(
			"Users",
			make_column("id", &User::GetID, &User::SetID, autoincrement(), primary_key()),
			make_column("username", &User::GetUsername, &User::SetUsername),
			make_column("password", &User::GetPassword, &User::SetPassword),
			make_column("email", &User::GetEmail, &User::SetEmail),
			make_column("accountCreationDate", &User::GetAccountCreationDate, &User::SetAccountCreationDate),
			make_column("score", &User::GetTotalScore, &User::SetTotalScore),
			make_column("playedGames", &User::GetPlayedGames, &User::SetPlayedGames),
			make_column("wonGames", &User::GetWonGames, &User::SetWonGames)
		)
	);
}

using UsersStorage = decltype(CreateStorage(""));

class UserDatabaseControl
{
private:
	UsersStorage& database;

public:
	UserDatabaseControl(UsersStorage& storage);

	crow::response operator() (const crow::request& request) const;
};

