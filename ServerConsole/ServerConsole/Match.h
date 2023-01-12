#pragma once

#include <string>

#include "Room.h"

class Match:
	virtual public Room
{
public:
	Match();
	
	Match(uint32_t id,
		const std::string& roomNumber,
		numberOfPlayers currentNumberOfPlayers,
		const std::string& player1,
		const std::string& player2,
		const std::string& player3,
		const std::string& player4,
		const std::string& player5,
		const std::string& player6,
		const std::string& winner);

	~Match();

public:
	void SetWinner(const std::string& winner);
	const std::string& GetWinner() const;

private:
	std::string m_winner;
};