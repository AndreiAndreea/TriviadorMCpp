#include "Match.h"

Match::Match()
	:Room()
{
	m_winner = std::string();
}

Match::Match(uint32_t id, const std::string& roomNumber, numberOfPlayers currentNumberOfPlayers,
	const std::string& player1, const std::string& player2, const std::string& player3,
	const std::string& player4, const std::string& player5, const std::string& player6,
	const std::string& winner)
	
	:Room(id, roomNumber, currentNumberOfPlayers,
		player1, player2, player3, 
		player4, player5, player6),
	m_winner(winner)
{
}

Match::~Match()
{
	/* EMPTY */
}

void Match::SetWinner(const std::string& winner)
{
	m_winner = winner;
}

const std::string& Match::GetWinner() const
{
	return m_winner;
}