#include "Room.h"

Room::Room()
{
	m_roomID = 0;
	m_gameType = std::string();
	m_gameStatus = std::string();
	m_roomNumber = std::string();
	m_currentNumberOfPlayers = 0;
	m_maximNumberOfPlayers = 0;
	m_number_of_ready_players = 0;
	m_player1 = { std::string(), std::string(), std::string(), float() };
	m_player2 = { std::string(), std::string(), std::string(), float() };
	m_player3 = { std::string(), std::string(), std::string(), float() };
	m_player4 = { std::string(), std::string(), std::string(), float() };
	m_player5 = { std::string(), std::string(), std::string(), float() };
	m_player6 = { std::string(), std::string(), std::string(), float() };
	m_winner = std::string();
}

Room::Room(
	uint32_t id,
	const std::string& game_type,
	const std::string& game_status,
	const std::string& room_number,
	numberOfPlayers current_number_of_players,
	numberOfPlayers maxim_number_of_players,
	numberOfPlayers number_of_ready_players,
	const std::string& player1,
	const std::string& player2,
	const std::string& player3,
	const std::string& player4,
	const std::string& player5,
	const std::string& player6
)
	: m_roomID(id),
	m_gameType(game_type),
	m_gameStatus(game_status),
	m_roomNumber(room_number),
	m_currentNumberOfPlayers(current_number_of_players),
	m_maximNumberOfPlayers(maxim_number_of_players),
	m_number_of_ready_players(number_of_ready_players)
{
	std::get<0>(m_player1) = player1;
	std::get<1>(m_player1) = std::string();
	std::get<2>(m_player1) = std::string();
	std::get<3>(m_player1) = float();
	
	std::get<0>(m_player2) = player2;
	std::get<1>(m_player2) = std::string();
	std::get<2>(m_player2) = std::string();
	std::get<3>(m_player2) = float();
	
	std::get<0>(m_player3) = player3;
	std::get<1>(m_player3) = std::string();
	std::get<2>(m_player3) = std::string();
	std::get<3>(m_player3) = float();
	
	std::get<0>(m_player4) = player4;
	std::get<1>(m_player4) = std::string();
	std::get<2>(m_player4) = std::string();
	std::get<3>(m_player4) = float();
	
	std::get<0>(m_player5) = player5;
	std::get<1>(m_player5) = std::string();
	std::get<2>(m_player5) = std::string();
	std::get<3>(m_player5) = float();
	
	std::get<0>(m_player6) = player6;
	std::get<1>(m_player6) = std::string();
	std::get<2>(m_player6) = std::string();
	std::get<3>(m_player6) = float();
	
	m_winner = std::string();
}

Room::Room(const Room& object)
{
	*this = object;
}

Room::Room(Room&& object)
{
	*this = std::move(object);
}

Room& Room::operator=(const Room& object)
{
	if (this != &object)
	{
		m_roomID = object.m_roomID;
		m_gameType = object.m_gameType;
		m_gameStatus = object.m_gameStatus;
		m_roomNumber = object.m_roomNumber;
		m_currentNumberOfPlayers = object.m_currentNumberOfPlayers;
		m_maximNumberOfPlayers = object.m_maximNumberOfPlayers;
		m_number_of_ready_players = object.m_number_of_ready_players;
		m_player1 = object.m_player1;
		m_player2 = object.m_player2;
		m_player3 = object.m_player3;
		m_player4 = object.m_player4;
		m_player5 = object.m_player5;
		m_player6 = object.m_player6;
		m_winner = object.m_winner;
	}
	
	return *this;
}

Room& Room::operator=(Room&& object)
{
	if (this != &object)
	{
		m_roomID = object.m_roomID;
		m_gameType = object.m_gameType;
		m_gameStatus = object.m_gameStatus;
		m_roomNumber = object.m_roomNumber;
		m_currentNumberOfPlayers = object.m_currentNumberOfPlayers;
		m_maximNumberOfPlayers = object.m_maximNumberOfPlayers;
		m_number_of_ready_players = object.m_number_of_ready_players;
		m_player1 = object.m_player1;
		m_player2 = object.m_player2;
		m_player3 = object.m_player3;
		m_player4 = object.m_player4;
		m_player5 = object.m_player5;
		m_player6 = object.m_player6;
		m_winner = object.m_winner;
	}

	new(&object) Room();

	return *this;
}

Room::~Room()
{
	/* EMPTY */
}

void Room::SetRoomID(uint32_t id)
{
	m_roomID = id;
}

uint32_t Room::GetRoomID() const
{
	return m_roomID;
}

void Room::SetGameType(const std::string& game_type)
{
	m_gameType = game_type;
}

const std::string& Room::GetGameType() const
{
	return m_gameType;
}

void Room::SetGameStatus(const std::string& game_status)
{
	m_gameStatus = game_status;
}

const std::string& Room::GetGameStatus() const
{
	return m_gameStatus;
}

void Room::SetRoomNumber(const std::string& room_number)
{
	m_roomNumber = room_number;
}

const std::string& Room::GetRoomNumber() const
{
	return m_roomNumber;
}

void Room::SetCurrentNumberOfPlayers(numberOfPlayers current_number_of_players)
{
	m_currentNumberOfPlayers = current_number_of_players;
}

numberOfPlayers Room::GetCurrentNumberOfPlayers() const
{
	return m_currentNumberOfPlayers;
}

void Room::SetMaximNumberOfPlayers(numberOfPlayers maxim_number_of_players)
{
	m_maximNumberOfPlayers = maxim_number_of_players;
}

numberOfPlayers Room::GetMaximNumberOfPlayers() const
{
	return m_maximNumberOfPlayers;
}

void Room::SetNumberOfReadyPlayers(numberOfPlayers number_of_ready_players)
{
	m_number_of_ready_players = number_of_ready_players;
}

numberOfPlayers Room::GetNumberOfReadyPlayers() const
{
	return m_number_of_ready_players;
}

void Room::SetPlayer1(const std::string& player1)
{
	std::get<0>(m_player1) = player1;
	
	if (player1 != "")
	{
		std::get<1>(m_player1) = m_colorList[1];
	}
}

const std::string& Room::GetPlayer1() const
{
	return std::get<0>(m_player1);
}

void Room::SetPlayer2(const std::string& player2)
{
	std::get<0>(m_player2) = player2;

	if (player2 != "")
	{
		std::get<1>(m_player2) = m_colorList[2];
	}
}

const std::string& Room::GetPlayer2() const
{
	return std::get<0>(m_player2);
}

void Room::SetPlayer3(const std::string& player3)
{
	std::get<0>(m_player3) = player3;

	if (player3 != "")
	{
		std::get<1>(m_player3) = m_colorList[3];
	}
}

const std::string& Room::GetPlayer3() const
{
	return std::get<0>(m_player3);
}

void Room::SetPlayer4(const std::string& player4)
{
	std::get<0>(m_player4) = player4;

	if (player4 != "")
	{
		std::get<1>(m_player4) = m_colorList[4];
	}
}

const std::string& Room::GetPlayer4() const
{
	return std::get<0>(m_player4);
}

void Room::SetPlayer5(const std::string& player5)
{
	std::get<0>(m_player5) = player5;
	
	if (player5 != "")
	{
		std::get<1>(m_player5) = m_colorList[5];
	}
}

const std::string& Room::GetPlayer5() const
{
	return std::get<0>(m_player5);
}

void Room::SetPlayer6(const std::string& player6)
{
	std::get<0>(m_player6) = player6;

	if (player6 != "")
	{
		std::get<1>(m_player6) = m_colorList[6];
	}
}

const std::string& Room::GetPlayer6() const
{
	return std::get<0>(m_player6);
}

const std::string& Room::GetColor1() const
{
	return std::get<1>(m_player1);
}

const std::string& Room::GetColor2() const
{
	return std::get<1>(m_player2);
}

const std::string& Room::GetColor3() const
{
	return std::get<1>(m_player3);
}

const std::string& Room::GetColor4() const
{
	return std::get<1>(m_player4);
}

const std::string& Room::GetColor5() const
{
	return std::get<1>(m_player5);
}

const std::string& Room::GetColor6() const
{
	return std::get<1>(m_player6);
}

void Room::SetResponseTime(std::string username, float responseTime)
{
	if (std::get<0>(m_player1) == username)
		std::get<3>(m_player1) = responseTime;
	else if (std::get<0>(m_player2) == username)
		std::get<3>(m_player2) = responseTime;
	else if (std::get<0>(m_player3) == username)
		std::get<3>(m_player3) = responseTime;
	else if (std::get<0>(m_player4) == username)
		std::get<3>(m_player4) = responseTime;
	else if (std::get<0>(m_player5) == username)
		std::get<3>(m_player5) = responseTime;
	else if (std::get<0>(m_player6) == username)
		std::get<3>(m_player6) = responseTime;
}

float Room::GetResponseTime(std::string username)
{
	if (std::get<0>(m_player1) == username)
		return std::get<3>(m_player1);

	if (std::get<0>(m_player2) == username)
		return std::get<3>(m_player2);
	
	if (std::get<0>(m_player3) == username)
		return std::get<3>(m_player3);
	
	if (std::get<0>(m_player4) == username)
		return std::get<3>(m_player4);
	
	if (std::get<0>(m_player5) == username)
		return std::get<3>(m_player5);
	
	if (std::get<0>(m_player6) == username)
		return std::get<3>(m_player6);
}

void Room::SetPlayerAnswer(std::string username, std::string answer)
{
	if (std::get<0>(m_player1) == username)
		std::get<2>(m_player1) = answer;
	else if (std::get<0>(m_player2) == username)
		std::get<2>(m_player2) = answer;
	else if (std::get<0>(m_player3) == username)
		std::get<2>(m_player3) = answer;
	else if (std::get<0>(m_player4) == username)
		std::get<2>(m_player4) = answer;
	else if (std::get<0>(m_player5) == username)
		std::get<2>(m_player5) = answer;
	else if (std::get<0>(m_player6) == username)
		std::get<2>(m_player6) = answer;
}

std::string Room::GetPlayerAnswer(std::string username)
{
	if (std::get<0>(m_player1) == username)
		return std::get<2>(m_player1);

	if (std::get<0>(m_player2) == username)
		return std::get<2>(m_player2);

	if (std::get<0>(m_player3) == username)
		return std::get<2>(m_player3);

	if (std::get<0>(m_player4) == username)
		return std::get<2>(m_player4);

	if (std::get<0>(m_player5) == username)
		return std::get<2>(m_player5);

	if (std::get<0>(m_player6) == username)
		return std::get<2>(m_player6);
}

void Room::SetWinner(const std::string& winner)
{
	m_winner = winner;
}

const std::string& Room::GetWinner() const
{
	return m_winner;
}

std::vector<playerData> Room::GetPlayersInfo()
{
	std::vector<playerData> playersInfo;

	if (std::get<0>(m_player1) != "")
		playersInfo.push_back(m_player1);

	if (std::get<0>(m_player2) != "")
		playersInfo.push_back(m_player2);
	
	if (std::get<0>(m_player3) != "")
		playersInfo.push_back(m_player3);
	
	if (std::get<0>(m_player4) != "")
		playersInfo.push_back(m_player4);

	if (std::get<0>(m_player5) != "")
		playersInfo.push_back(m_player5);

	return playersInfo;
}

bool Room::operator==(const Room& room)
{
	return this->GetRoomID() == room.GetRoomID();
}

std::string Room::GetColorForPlayer(const std::string& player_name)
{
	if (player_name == std::get<0>(m_player1))
		return std::get<1>(m_player1);
	
	if (player_name == std::get<0>(m_player2))
		return std::get<1>(m_player2);
	
	if (player_name == std::get<0>(m_player3))
		return std::get<1>(m_player3);
	
	if (player_name == std::get<0>(m_player4))
		return std::get<1>(m_player4);
	
	if (player_name == std::get<0>(m_player5))
		return std::get<1>(m_player5);
	
	if (player_name == std::get<0>(m_player6))
		return std::get<1>(m_player6);

	return "Player not found!";
}

playerData& Room::GetPlayerByUsername(std::string username)
{
	if (std::get<0>(m_player1) == username)
		return m_player1;
	
	if (std::get<0>(m_player2) == username)
		return m_player2;
	
	if (std::get<0>(m_player3) == username)
		return m_player3;
	
	if (std::get<0>(m_player4) == username)
		return m_player4;
	
	if (std::get<0>(m_player5) == username)
		return m_player5;
	
	if (std::get<0>(m_player6) == username)
		return m_player6;
}
