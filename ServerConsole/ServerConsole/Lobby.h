#pragma once

#include <string>

#include "Room.h"

class Lobby :
	virtual public Room
{
public:
	Lobby();
	
	Lobby(uint32_t id,
		const std::string& roomNumber,
		numberOfPlayers currentNumberOfPlayers,
		const std::string& player1,
		const std::string& player2,
		const std::string& player3,
		const std::string& player4,
		const std::string& player5,
		const std::string& player6,
		const std::string& gameType,
		const std::string& gameStatus,
		numberOfPlayers maximNumberOfPlayers,
		numberOfPlayers numberOfReadyPlayers);
	
	~Lobby();

public:
	void SetGameType(const std::string& game_type);
	const std::string& GetGameType() const;

	void SetGameStatus(const std::string& game_status);
	const std::string& GetGameStatus() const;

	void SetMaximNumberOfPlayers(numberOfPlayers maximNumberOfPlayers);
	numberOfPlayers GetMaximNumberOfPlayers() const;

	void SetNumberOfReadyPlayers(numberOfPlayers numberOfReadyPlayers);
	numberOfPlayers GetNumberOfReadyPlayers() const;

private:
	std::string m_gameType, m_gameStatus;

	numberOfPlayers m_maximNumberOfPlayers, m_numberOfReadyPlayers;
};