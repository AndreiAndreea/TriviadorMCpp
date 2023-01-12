#pragma once

#include <string>
#include <iostream>
#include <cstdint>
#include <vector>

class User
{
public:
	User();
	User(uint32_t id, const std::string& username, const std::string& password, const std::string& email, const std::string& accountCreationDate, const std::string& totalScore, const std::string& playedGames, const std::string& wonGames, const std::string& connectStatus);

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

	void SetConnectStatus(const std::string& connectStatus);
	const std::string& GetConnectStatus() const;

private:
	uint32_t m_id;

	std::string m_username;
	std::string m_password;
	std::string m_email;
	std::string m_accountCreationDate;

	std::string m_totalScore;

	std::string m_playedGames;
	std::string m_wonGames;

	std::string m_connectStatus;
};