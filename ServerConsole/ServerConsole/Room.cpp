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
	m_player1 = std::string();
	m_player2 = std::string();
	m_player3 = std::string();
	m_player4 = std::string();
	m_player5 = std::string();
	m_player6 = std::string();
	m_color1 = std::string();
	m_color2 = std::string();
	m_color3 = std::string();
	m_color4 = std::string();
	m_color5 = std::string();
	m_color6 = std::string();
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
	m_number_of_ready_players(number_of_ready_players),
	m_player1(player1),
	m_player2(player2),
	m_player3(player3),
	m_player4(player4),
	m_player5(player5),
	m_player6(player6)
{
	m_color1 = std::string();
	m_color2 = std::string();
	m_color3 = std::string();
	m_color4 = std::string();
	m_color5 = std::string();
	m_color6 = std::string();

	m_winner = std::string();
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
	m_player1 = player1;
	SetColor1();
}

const std::string& Room::GetPlayer1() const
{
	return m_player1;
}

void Room::SetPlayer2(const std::string& player2)
{
	m_player2 = player2;
	SetColor2();
}

const std::string& Room::GetPlayer2() const
{
	return m_player2;
}

void Room::SetPlayer3(const std::string& player3)
{
	m_player3 = player3;
	SetColor3();
}

const std::string& Room::GetPlayer3() const
{
	return m_player3;
}

void Room::SetPlayer4(const std::string& player4)
{
	m_player4 = player4;
	SetColor4();
}

const std::string& Room::GetPlayer4() const
{
	return m_player4;
}

void Room::SetPlayer5(const std::string& player5)
{
	m_player5 = player5;
	SetColor5();
}

const std::string& Room::GetPlayer5() const
{
	return m_player5;
}

void Room::SetPlayer6(const std::string& player6)
{
	m_player6 = player6;
	SetColor6();
}

const std::string& Room::GetPlayer6() const
{
	return m_player6;
}

void Room::SetColor1()
{
	if (m_player1 == "")
		m_color1 = "";
	else
		m_color1 = GenerateRandomColorForPlayer();
}

const std::string& Room::GetColor1() const
{
	return m_color1;
}

void Room::SetColor2()
{
	if (m_player2 == "")
		m_color2 = "";
	else
		m_color2 = GenerateRandomColorForPlayer();
}

const std::string& Room::GetColor2() const
{
	return m_color2;
}

void Room::SetColor3()
{
	if (m_player3 == "")
		m_color3 = "";
	else
		m_color3 = GenerateRandomColorForPlayer();
}

const std::string& Room::GetColor3() const
{
	return m_color3;
}

void Room::SetColor4()
{
	if (m_player4 == "")
		m_color4 = "";
	else
		m_color4 = GenerateRandomColorForPlayer();
}

const std::string& Room::GetColor4() const
{
	return m_color4;
}

void Room::SetColor5()
{
	if (m_player5 == "")
		m_color5 = "";
	else
		m_color5 = GenerateRandomColorForPlayer();
}

const std::string& Room::GetColor5() const
{
	return m_color5;
}

void Room::SetColor6()
{
	if (m_player6 == "")
		m_color6 = "";
	else
		m_color6 = GenerateRandomColorForPlayer();
}

const std::string& Room::GetColor6() const
{
	return m_color6;
}

void Room::SetWinner(const std::string& winner)
{
	m_winner = winner;
}

const std::string& Room::GetWinner() const
{
	return m_winner;
}

std::string Room::GenerateRandomColorForPlayer()
{
	srand(time(0));

	int randomColorIndex = rand() % m_colorList.size();

	std::string color = m_colorList[randomColorIndex];
	
	//scoatem culoarea aleasa din lista de culori disponibile, ca sa nu se repete
	m_colorList.erase(m_colorList.begin() + randomColorIndex);
	
	return color;
}

std::string Room::GetColorForPlayer(const std::string& player_name)
{
	if (player_name == m_player1)
		return m_color1;
	
	if (player_name == m_player2)
		return m_color2;
	
	if (player_name == m_player3)
		return m_color3;
	if (player_name == m_player4)
		return m_color4;
	
	if (player_name == m_player5)
		return m_color5;
	
	if (player_name == m_player6)
		return m_color6;

	return "Player not found!";
}
