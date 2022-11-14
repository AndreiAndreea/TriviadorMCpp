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

void Player::AddRegion(Coords coords)
{
	if (m_territory.size() == 0)
		m_score += 300;
	else
		m_score += 100;

	m_territory.push_back(coords);
}

void Player::RemoveLostRegion(Coords coords)
{
	uint8_t counter = 0;

	for (auto& region : m_territory)
	{
		counter++;

		if (region == coords)
		{
			m_score -= 100;
			
			if(m_territory.size() > 1)
				m_territory.erase(m_territory.begin() + counter - 1);
		}
	}
}

std::ostream& operator<<(std::ostream& os, const Player& player)
{
	return os << player.m_username << std::endl
		<< player.m_score << std::endl;
}
