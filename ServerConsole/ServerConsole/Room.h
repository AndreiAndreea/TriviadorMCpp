#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

using numberOfPlayers = uint8_t;

class Room
{
public:
	Room();
	Room(uint32_t id,
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
	);

	~Room();

public:
	void SetRoomID(uint32_t id);
	uint32_t GetRoomID() const;

	void SetGameType(const std::string& game_type);
	const std::string& GetGameType() const;

	void SetGameStatus(const std::string& game_status);
	const std::string& GetGameStatus() const;

	void SetRoomNumber(const std::string& room_number);
	const std::string& GetRoomNumber() const;

	void SetCurrentNumberOfPlayers(numberOfPlayers current_number_of_players);
	numberOfPlayers GetCurrentNumberOfPlayers() const;

	void SetMaximNumberOfPlayers(numberOfPlayers maxim_number_of_players);
	numberOfPlayers GetMaximNumberOfPlayers() const;

	void SetNumberOfReadyPlayers(numberOfPlayers number_of_ready_players);
	numberOfPlayers GetNumberOfReadyPlayers() const;

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

	void SetColor1();
	const std::string& GetColor1() const;
	
	void SetColor2();
	const std::string& GetColor2() const;
	
	void SetColor3();
	const std::string& GetColor3() const;
	
	void SetColor4();
	const std::string& GetColor4() const;
	
	void SetColor5();
	const std::string& GetColor5() const;
	
	void SetColor6();
	const std::string& GetColor6() const;
	
public:
	std::string GenerateRandomColorForPlayer();
	std::string GetColorForPlayer(const std::string& player_name);
	
private:
	uint32_t m_roomID;

	std::string m_gameType, m_gameStatus, m_roomNumber;

	numberOfPlayers m_currentNumberOfPlayers, m_maximNumberOfPlayers, m_number_of_ready_players;

	std::string m_player1, m_player2, m_player3, m_player4, m_player5, m_player6;

	std::vector<std::string> m_colorList = { "yellow", "blue", "green", "cyan", "magenta", "red", "darkGreen", "darkCyan", "darkMagenta", "darkRed"};
	
	std::string m_color1, m_color2, m_color3, m_color4, m_color5, m_color6;
};