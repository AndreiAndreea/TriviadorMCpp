#pragma once

#include <string>

using numberOfPlayers = uint8_t;

class Room
{
public:
	Room();

	Room(uint32_t id,
		const std::string& roomNumber,
		numberOfPlayers currentNumberOfPlayers,
		const std::string& player1,
		const std::string& player2,
		const std::string& player3,
		const std::string& player4,
		const std::string& player5,
		const std::string& player6);

	virtual ~Room();


public:
	void SetRoomID(uint32_t id);
	uint32_t GetRoomID() const;

	void SetRoomNumber(const std::string& matchNumber);
	const std::string& GetRoomNumber() const;

	void SetCurrentNumberOfPlayers(numberOfPlayers currentNumberOfPlayers);
	numberOfPlayers GetCurrentNumberOfPlayers() const;

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
	uint32_t m_IDRoom;

	std::string m_roomNumber;

	numberOfPlayers m_currentNumberOfPlayers;

	std::string m_player1, m_player2, m_player3, m_player4, m_player5, m_player6;
};