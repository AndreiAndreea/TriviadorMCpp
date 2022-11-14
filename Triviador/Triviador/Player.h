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

public:
	using Coords = std::pair<uint8_t, uint8_t>;

public:
	void SetScore(uint16_t score);
	const uint16_t GetScore() const;

	void SetUsername(const std::string& username);
	const std::string GetUsername() const;

public:
	void AddRegion(Coords coords);
	void RemoveLostRegion(Coords coords);

public:
	friend std::ostream& operator<<(std::ostream& os, const Player& player);

private:
	std::string m_username;
	uint16_t m_score;
	std::vector<Coords> m_territory;
};
