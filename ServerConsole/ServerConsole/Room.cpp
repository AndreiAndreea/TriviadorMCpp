#include "Room.h"

Room::Room()
{
	m_IDRoom = 0;
	m_roomNumber = std::string();
	m_currentNumberOfPlayers = 0;
	m_player1 = std::string();
	m_player2 = std::string();
	m_player3 = std::string();
	m_player4 = std::string();
	m_player5 = std::string();
	m_player6 = std::string();
}

Room::Room(uint32_t id, const std::string& matchNumber, numberOfPlayers currentNumberOfPlayers,
	const std::string& player1, const std::string& player2, const std::string& player3, 
	const std::string& player4, const std::string& player5, const std::string& player6)
	:m_IDRoom(id),
	m_roomNumber(matchNumber),
	m_currentNumberOfPlayers(currentNumberOfPlayers),
	m_player1(player1),
	m_player2(player2),
	m_player3(player3),
	m_player4(player4),
	m_player5(player5),
	m_player6(player6)
{
}

Room::~Room()
{
	/* EMPTY */
}

void Room::SetRoomID(uint32_t id)
{
	m_IDRoom = id;
}

uint32_t Room::GetRoomID() const
{
	return m_IDRoom;
}

void Room::SetRoomNumber(const std::string& roomNumber)
{
	m_roomNumber = roomNumber;
}

const std::string& Room::GetRoomNumber() const
{
	return m_roomNumber;
}

void Room::SetCurrentNumberOfPlayers(numberOfPlayers numberOfPlayers)
{
	m_currentNumberOfPlayers = numberOfPlayers;
}

numberOfPlayers Room::GetCurrentNumberOfPlayers() const
{
	return m_currentNumberOfPlayers;
}

void Room::SetPlayer1(const std::string& player1)
{
	m_player1 = player1;
}

const std::string& Room::GetPlayer1() const
{
	return m_player1;
}

void Room::SetPlayer2(const std::string& player2)
{
	m_player2 = player2;
}

const std::string& Room::GetPlayer2() const
{
	return m_player2;
}

void Room::SetPlayer3(const std::string& player3)
{
	m_player3 = player3;
}

const std::string& Room::GetPlayer3() const
{
	return m_player3;
}

void Room::SetPlayer4(const std::string& player4)
{
	m_player4 = player4;
}

const std::string& Room::GetPlayer4() const
{
	return m_player4;
}

void Room::SetPlayer5(const std::string& player5)
{
	m_player5 = player5;
}

const std::string& Room::GetPlayer5() const
{
	return m_player5;
}

void Room::SetPlayer6(const std::string& player6)
{
	m_player6 = player6;
}

const std::string& Room::GetPlayer6() const
{
	return m_player6;
}
