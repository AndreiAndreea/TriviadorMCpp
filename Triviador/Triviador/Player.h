#pragma once
#include <string>
#include <iostream>
#include <cstdint>

class Player
{
public:
	Player();
	Player(const std::string& username, uint16_t score);

	void SetScore(uint16_t score);
	const uint16_t GetScore() const;

	void SetUsername(const std::string& username);
	const std::string GetUsername() const;

	friend std::ostream& operator<<(std::ostream& os, const Player& player);

private:
	std::string m_username;
	uint16_t m_score;
};
