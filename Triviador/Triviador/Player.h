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
	void SetScore(uint16_t score);
	const uint16_t GetScore() const;

	void SetUsername(const std::string& username);
	const std::string GetUsername() const;

	std::vector<Coords> GetTerritory() const;
	
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
	uint16_t m_score;
	std::vector<Coords> m_territory;
	uint8_t m_lives = 3;
};
