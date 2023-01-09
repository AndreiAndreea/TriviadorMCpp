#pragma once

#include <string>

using numberOfPlayers = uint8_t;

class Match
{
public:
	Match();
	
	Match(uint32_t id,
		const std::string& matchNumber,
		const std::string& winner,
		numberOfPlayers numberOfPlayers,
		const std::string& player1,
		const std::string& player2,
		const std::string& player3,
		const std::string& player4,
		const std::string& player5,
		const std::string& player6);

	~Match();

public:
	void SetMatchID(uint32_t id);
	uint32_t GetMatchID() const;

	void SetMatchNumber(const std::string& matchNumber);
	const std::string& GetMatchNumber() const;
	
	void SetWinner(const std::string& winner);
	const std::string& GetWinner() const;

	void SetNumberOfPlayers(numberOfPlayers numberOfPlayers);
	numberOfPlayers GetNumberOfPlayers() const;

	void SetPlayer1(const std::string& player1);
	const std::string& GetPlayer1() const;

	void SetPlayer2(const std::string& player2);
	const std::string& GetPlayer2() const;

	void SetPlayer3(const std::string& player3);
	const std::string& GetPlayer3() const;

	void SetPlayer4(const std::string& player4);
	const std::string& GetPlayer4() const;

	void SetPlayer5(const std::string& player5);
	const std::string& GetPlayer5() const;

	void SetPlayer6(const std::string& player6);
	const std::string& GetPlayer6() const;

private:
	uint32_t m_IDMatch;

	std::string m_matchNumber;
	
	std::string m_winner;

	numberOfPlayers m_numberOfPlayers;

	std::string m_player1, m_player2, m_player3, m_player4, m_player5, m_player6;
};