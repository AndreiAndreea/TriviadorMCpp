#pragma once

#include <string>

class Lobby
{
public:
	Lobby();
	Lobby(uint32_t id, const std::string& game_type, const std::string& player1, const std::string& player2, const std::string& player3, const std::string& player4, const std::string& player5, const std::string& player6);
	
	~Lobby();

public:
	void SetLobbyID(uint32_t id);
	uint32_t GetLobbyID() const;

	void SetGameType(const std::string& game_type);
	const std::string& GetGameType() const;
	
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
	uint32_t m_IDLobby;

	std::string m_gameType;

	std::string m_player1, m_player2, m_player3, m_player4, m_player5, m_player6;
};