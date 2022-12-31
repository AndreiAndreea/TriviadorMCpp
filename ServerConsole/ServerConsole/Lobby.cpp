#include "Lobby.h"

Lobby::Lobby()
{
	m_IDLobby = 0;
	m_gameType = std::string();
	m_gameStatus = std::string();
	m_roomNumber = std::string();
	m_currentNumberOfPlayers = 0;
	m_maximNumberOfPlayers = 0;
	m_player1 = std::string();
	m_player2 = std::string();
	m_player3 = std::string();
	m_player4 = std::string();
	m_player5 = std::string();
	m_player6 = std::string();
}

Lobby::Lobby(uint32_t id, const std::string& game_type, const std::string& game_status, const std::string& room_number, numberOfPlayers current_number_of_players, numberOfPlayers maxim_number_of_players, const std::string& player1, const std::string& player2, const std::string& player3, const std::string& player4, const std::string& player5, const std::string& player6)
	: m_IDLobby(id),
	m_gameType(game_type),
	m_gameStatus(game_status),
	m_roomNumber(room_number),
	m_currentNumberOfPlayers(current_number_of_players),
	m_maximNumberOfPlayers(maxim_number_of_players),
	m_player1(player1),
	m_player2(player2),
	m_player3(player3),
	m_player4(player4),
	m_player5(player5),
	m_player6(player6)
{
	
}

Lobby::~Lobby()
{
	/* EMPTY */
}

void Lobby::SetLobbyID(uint32_t id)
{
	m_IDLobby = id;
}

uint32_t Lobby::GetLobbyID() const
{
	return m_IDLobby;
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

void Lobby::SetRoomNumber(const std::string& room_number)
{
	m_roomNumber = room_number;
}

const std::string& Lobby::GetRoomNumber() const
{
	return m_roomNumber;
}

void Lobby::SetCurrentNumberOfPlayers(numberOfPlayers current_number_of_players)
{
	m_currentNumberOfPlayers = current_number_of_players;
}

numberOfPlayers Lobby::GetCurrentNumberOfPlayers() const
{
	return m_currentNumberOfPlayers;
}

void Lobby::SetMaximNumberOfPlayers(numberOfPlayers maxim_number_of_players)
{
	m_maximNumberOfPlayers = maxim_number_of_players;
}

numberOfPlayers Lobby::GetMaximNumberOfPlayers() const
{
	return m_maximNumberOfPlayers;
}

void Lobby::SetPlayer1(const std::string& player1)
{
	m_player1 = player1;
}

const std::string& Lobby::GetPlayer1() const
{
	return m_player1;
}

void Lobby::SetPlayer2(const std::string& player2)
{
	m_player2 = player2;
}

const std::string& Lobby::GetPlayer2() const
{
	return m_player2;
}

void Lobby::SetPlayer3(const std::string& player3)
{
	m_player3 = player3;
}

const std::string& Lobby::GetPlayer3() const
{
	return m_player3;
}

void Lobby::SetPlayer4(const std::string& player4)
{
	m_player4 = player4;
}

const std::string& Lobby::GetPlayer4() const
{
	return m_player4;
}

void Lobby::SetPlayer5(const std::string& player5)
{
	m_player5 = player5;
}

const std::string& Lobby::GetPlayer5() const
{
	return m_player5;
}

void Lobby::SetPlayer6(const std::string& player6)
{
	m_player6 = player6;
}

const std::string& Lobby::GetPlayer6() const
{
	return m_player6;
}
