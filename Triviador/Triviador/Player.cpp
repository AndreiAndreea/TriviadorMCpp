#include "Player.h"

Player::Player()
{
	m_username = "null";
	m_currentGameScore = 0;
}

Player::Player(const std::string& username, uint16_t score)
	:m_username(username),
	m_currentGameScore(score)
{

}

Player::Player(const Player& player)
{
	*this = player;
}

Player::Player(Player&& player)
{
	*this = std::move(player);
}

void Player::SetCurrentGameScore(uint16_t score)
{
	m_currentGameScore = score;
}

uint16_t Player::GetCurrentGameScore() const
{
	return m_currentGameScore;
}

void Player::SetUsername(const std::string& username)
{
	m_username = username;
}

const std::string& Player::GetUsername() const
{
	return m_username;
}

void Player::SetPassword(const std::string& password)
{
	m_password = password;
}

const std::string& Player::GetPassword() const
{
	return m_password;
}

void Player::SetEmail(const std::string& email)
{
	m_email = email;
}

const std::string& Player::GetEmail() const
{
	return m_email;
}

void Player::SetAccountCreationDate(const std::string& accountCreationDate)
{
	m_accountCreationDate = accountCreationDate;
}

const std::string& Player::GetAccountCreationDate() const
{
	return m_accountCreationDate;
}

void Player::SetTotalScore(const std::string& totalScore)
{
	m_totalScore = totalScore;
}

const std::string& Player::GetTotalScore() const
{
	return m_totalScore;
}

void Player::SetPlayedGames(const std::string& playedGames)
{
	m_playedGames = playedGames;
}

const std::string& Player::GetPlayedGames() const
{
	return m_playedGames;
}

void Player::SetWonGames(const std::string& wonGames)
{
	m_wonGames = wonGames;
}

const std::string& Player::GetWonGames() const
{
	return m_wonGames;
}

std::vector<std::pair<uint8_t, uint8_t>> Player::GetTerritory() const
{
	return m_territory;
}

void Player::AddRegion(Coords coords)
{
	if (m_territory.size() == 0)
		m_currentGameScore += 300;
	else
		m_currentGameScore += 100;

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
			m_currentGameScore -= 100;
			
			if(m_territory.size() > 1)
				m_territory.erase(m_territory.begin() + counter - 1);
		}
	}
}

void Player::AddTerritory(std::vector<Coords> newTerritory)
{
	for (auto& newRegion : newTerritory)
		m_territory.push_back(newRegion);
}

void Player::RemoveTerritory()
{
	m_territory.clear();
}

bool Player::IsBase(Coords coords)
{
	if (m_territory[0] == coords)
		return true;

	return false;
}

void Player::LoseLife()
{
	m_lives--;
}

bool Player::IsDead()
{
	if (m_lives == 0)
		return true;

	return false;
}

Player& Player::operator=(const Player& player)
{
	m_username = player.m_username;
	m_currentGameScore = player.m_currentGameScore;
	m_territory = player.m_territory;
	m_lives = player.m_lives;

	return *this;
}

Player& Player::operator=(Player&& player)
{
	m_username = player.m_username;
	m_currentGameScore = player.m_currentGameScore;
	m_territory = player.m_territory;
	m_lives = player.m_lives;

	new(&player)Player;

	return *this;
}

std::ostream& operator<<(std::ostream& os, const Player& player)
{
	return os << player.m_username << std::endl
		<< player.m_currentGameScore << std::endl;
}
