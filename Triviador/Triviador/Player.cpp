#include "Player.h"

Player::Player()
{
	m_username = "null";
	m_score = 0;
}

Player::Player(const std::string& username, uint16_t score)
	:m_username(username),
	m_score(score)
{
}

void Player::SetScore(uint16_t score)
{
	m_score = score;
}

const uint16_t Player::GetScore() const
{
	return m_score;
}

void Player::SetUsername(const std::string& username)
{
	m_username = username;
}

const std::string Player::GetUsername() const
{
	return m_username;
}

std::ostream& operator<<(std::ostream& os, const Player& player)
{
	return os << player.m_username << std::endl
		<< player.m_score << std::endl;
}
