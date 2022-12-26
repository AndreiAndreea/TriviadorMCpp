#include "User.h"

User::User()
{
	m_id = 0;
	m_username = std::string();
	m_password = std::string();
	m_email = std::string();
	m_accountCreationDate = std::string();
	m_totalScore = std::string();
	m_playedGames = std::string();
	m_wonGames = std::string();
	m_connectStatus = std::string();
}

User::User(uint32_t id, const std::string& username, const std::string& password, const std::string& email, const std::string& accountCreationDate, const std::string& totalScore, const std::string& playedGames, const std::string& wonGames, const std::string& connectStatus)
	: m_id(id),
	m_username(username),
	m_password(password),
	m_email(email),
	m_accountCreationDate(accountCreationDate),
	m_totalScore(totalScore),
	m_playedGames(playedGames),
	m_wonGames(wonGames),
	m_connectStatus(connectStatus)
{
}

void User::SetID(uint32_t id)
{
	m_id = id;
}

uint32_t User::GetID() const
{
	return m_id;
}

void User::SetUsername(const std::string& username)
{
	m_username = username;
}

const std::string& User::GetUsername() const
{
	return m_username;
}

void User::SetPassword(const std::string& password)
{
	m_password = password;
}

const std::string& User::GetPassword() const
{
	return m_password;
}

void User::SetEmail(const std::string& email)
{
	m_email = email;
}

const std::string& User::GetEmail() const
{
	return m_email;
}

void User::SetAccountCreationDate(const std::string& accountCreationDate)
{
	m_accountCreationDate = accountCreationDate;
}

const std::string& User::GetAccountCreationDate() const
{
	return m_accountCreationDate;
}

void User::SetTotalScore(const std::string& totalScore)
{
	m_totalScore = totalScore;
}

const std::string& User::GetTotalScore() const
{
	return m_totalScore;
}

void User::SetPlayedGames(const std::string& playedGames)
{
	m_playedGames = playedGames;
}

const std::string& User::GetPlayedGames() const
{
	return m_playedGames;
}

void User::SetWonGames(const std::string& wonGames)
{
	m_wonGames = wonGames;
}

const std::string& User::GetWonGames() const
{
	return m_wonGames;
}

void User::SetConnectStatus(const std::string& connectStatus)
{
	m_connectStatus = connectStatus;
}

const std::string& User::GetConnectStatus() const
{
	return m_connectStatus;
}

UserDatabaseControl::UserDatabaseControl(UsersStorage& storage)
	: database(storage)
{

}

crow::response UserDatabaseControl::operator()(const crow::request& request) const
{
	auto bodyArgs = ParseUrlArgs(request.body); //id=2&quantity=3&...
	auto end = bodyArgs.end();

	auto id = bodyArgs.find("id");
	auto username = bodyArgs.find("username");
	auto password = bodyArgs.find("password");
	auto email = bodyArgs.find("email");
	auto accountCreationDate = bodyArgs.find("accountCreationDate");
	auto totalScore = bodyArgs.find("totalScore");
	auto playedGames = bodyArgs.find("playedGames");
	auto wonGames = bodyArgs.find("wonGames");
	auto connectStatus = bodyArgs.find("connectStatus");

	if (id != end && username != end && password != end && email != end && accountCreationDate != end && totalScore != end && playedGames != end && wonGames != end && connectStatus != end)
	{
		User user(std::stoi(id->second), username->second, password->second, email->second, accountCreationDate->second, totalScore->second, playedGames->second, wonGames->second, connectStatus->second);

		database.update(user);

		return crow::response(200);
	}
	else
	{
		return crow::response(400);
	}
}