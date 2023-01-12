#include "Lobby.h"

Lobby::Lobby()
	:Room()
{
	m_gameType = std::string();
	m_gameStatus = std::string();
	m_maximNumberOfPlayers = 0;
	m_numberOfReadyPlayers = 0;
}

Lobby::Lobby(uint32_t id, const std::string& roomNumber, numberOfPlayers currentNumberOfPlayers,
	const std::string& player1, const std::string& player2, const std::string& player3, 
	const std::string& player4, const std::string& player5, const std::string& player6, 
	const std::string& gameType, const std::string& gameStatus, 
	numberOfPlayers maximNumberOfPlayers, numberOfPlayers numberOfReadyPlayers)
{
}

Lobby::~Lobby()
{
	/* EMPTY */
}

void Lobby::SetGameType(const std::string& game_type)
{
	m_gameType = game_type;
}

const std::string& Lobby::GetGameType() const
{
	return m_gameType;
}

void Lobby::SetGameStatus(const std::string& game_status)
{
	m_gameStatus = game_status;
}

const std::string& Lobby::GetGameStatus() const
{
	return m_gameStatus;
}

void Lobby::SetMaximNumberOfPlayers(numberOfPlayers maximNumberOfPlayers)
{
	m_maximNumberOfPlayers = maximNumberOfPlayers;
}

numberOfPlayers Lobby::GetMaximNumberOfPlayers() const
{
	return m_maximNumberOfPlayers;
}

void Lobby::SetNumberOfReadyPlayers(numberOfPlayers numberOfReadyPlayers)
{
	m_numberOfReadyPlayers = numberOfReadyPlayers;
}

numberOfPlayers Lobby::GetNumberOfReadyPlayers() const
{
	return m_numberOfReadyPlayers;
}