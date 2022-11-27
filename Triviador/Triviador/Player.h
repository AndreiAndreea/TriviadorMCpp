#pragma once
#include <string>
#include <iostream>
#include <cstdint>
#include <vector>

class Player
{
public:
	Player();
	Player(const std::string& username, uint16_t score);

	Player(const Player& player); //copy constructor
	Player(Player&& player); //move constructor

public:
	using Coords = std::pair<uint8_t, uint8_t>;

public:
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

	void SetCurrentGameScore(uint16_t currentGameScore);
	uint16_t GetCurrentGameScore() const;

	std::vector<Coords> GetTerritory() const;

public:
	void AddRegion(Coords coords);
	void RemoveLostRegion(Coords coords);

	void AddTerritory(std::vector<Coords> newTerritory);
	void RemoveTerritory();

	bool IsBase(Coords coords);
	void LoseLife();
	bool IsDead();

public:
	friend std::ostream& operator<<(std::ostream& os, const Player& player);

	Player& operator=(const Player& player);
	Player& operator=(Player&& player);

private:
	std::string m_username;
	std::string m_password;
	std::string m_email;
	std::string m_accountCreationDate;

	std::string m_totalScore;
	uint16_t m_currentGameScore;

	std::string m_playedGames;
	std::string m_wonGames;

	std::vector<Coords> m_territory;

	uint8_t m_lives = 3;
};
