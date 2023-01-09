#include "Match.h"

Match::Match()
{
	m_IDMatch = 0;
	m_matchNumber = std::string();
	m_winner = std::string();
	m_numberOfPlayers = 0;
	m_player1 = std::string();
	m_player2 = std::string();
	m_player3 = std::string();
	m_player4 = std::string();
	m_player5 = std::string();
	m_player6 = std::string();
}

Match::Match(uint32_t id, const std::string& matchNumber, const std::string& winner, numberOfPlayers numberOfPlayers, const std::string& player1, const std::string& player2, const std::string& player3, const std::string& player4, const std::string& player5, const std::string& player6)
	:m_IDMatch(id),
	m_matchNumber(matchNumber),
	m_winner(winner),
	m_numberOfPlayers(numberOfPlayers),
	m_player1(player1),
	m_player2(player2),
	m_player3(player3),
	m_player4(player4),
	m_player5(player5),
	m_player6(player6)
{
}

Match::~Match()
{
	/* EMPTY */
}

void Match::SetMatchID(uint32_t id)
{
	m_IDMatch = id;
}

uint32_t Match::GetMatchID() const
{
	return m_IDMatch;
}

void Match::SetMatchNumber(const std::string& matchNumber)
{
	m_matchNumber = matchNumber;
}

const std::string& Match::GetMatchNumber() const
{
	return m_matchNumber;
}

void Match::SetWinner(const std::string& winner)
{
	m_winner = winner;
}

const std::string& Match::GetWinner() const
{
	return m_winner;
}

void Match::SetNumberOfPlayers(numberOfPlayers numberOfPlayers)
{
	m_numberOfPlayers = numberOfPlayers;
}

numberOfPlayers Match::GetNumberOfPlayers() const
{
	return m_numberOfPlayers;
}

void Match::SetPlayer1(const std::string& player1)
{
	m_player1 = player1;
}

const std::string& Match::GetPlayer1() const
{
	return m_player1;
}

void Match::SetPlayer2(const std::string& player2)
{
	m_player2 = player2;
}

const std::string& Match::GetPlayer2() const
{
	return m_player2;
}

void Match::SetPlayer3(const std::string& player3)
{
	m_player3 = player3;
}

const std::string& Match::GetPlayer3() const
{
	return m_player3;
}

void Match::SetPlayer4(const std::string& player4)
{
	m_player4 = player4;
}

const std::string& Match::GetPlayer4() const
{
	return m_player4;
}

void Match::SetPlayer5(const std::string& player5)
{
	m_player5 = player5;
}

const std::string& Match::GetPlayer5() const
{
	return m_player5;
}

void Match::SetPlayer6(const std::string& player6)
{
	m_player6 = player6;
}

const std::string& Match::GetPlayer6() const
{
	return m_player6;
}
