#pragma once

#include "ui_Triviador.h"

#include "Game.h"

#include <QDebug>
#include <QTableWidgetItem>
#include <QTableWidget>

#include <regex>
#include <curl/curl.h>
#include <cpr/cpr.h>

//declare an enum to store the current state of the game
enum class GameState
{
	NotStarted,
	InProgress,
	Started
};

class Triviador : public QMainWindow
{
	Q_OBJECT

public:
	Triviador(QWidget* parent = nullptr);
	Triviador(const std::string& ip, const std::string& playerUsername);
	~Triviador();

private:
	Ui::TriviadorClass ui;

private slots:
	/*MENU*/
	void on_playGamePushButton_released();
	void on_profilePushButton_released();
	void on_quitPushButton_clicked();

	/*PLAYER PROFILE*/
	void on_profileSettingsPushButton_released();
	void on_backToMenuPushButton_released();

	/*CHANGE CREDENTIALS*/
	void on_changeUsernamePushButton_released();
	void on_changePasswordPushButton_released();
	void on_changeEmailPushButton_released();
	void on_saveProfileSettingsPushButton_released();
	void on_backToProfileButton_released();

	/*AVAILABLE LOBBIES*/
	void on_backToMenuPushButton_2_released();

	void on_twoPlayersPushButton_released();
	void on_threePlayersPushButton_released();
	void on_fourPlayersPushButton_released();
	void on_customModePushButton_released();

	void on_playersSpinBox_valueChanged(int arg1);
	void on_joinLobbyPushButton_released();

	/*LOBBY DETAILS*/
	void on_backToLobbyPushButton_released();

	void on_readyGameLobbyPushButton_released();
	void on_startGameLobbyPushButton_released();

	void TimerMethodToUpdateLobbyDetails();
	
	void StartTransferToGameTimer();
	void OnTransferToGameTimerTick();

signals:
	void BackToMenuSignal();
	void BackToLoginSignal();

	void ServerCrashedSignalTriviador();

private:
	/*PLAYER PROFILE*/
	void UpdateUserDetails();
	void UpdateGamesHistory();
	bool isValidEmail(const std::string& email);

	/*AVAILABLE LOBBIES*/
	void TurnAutoExclusiveButtonsForCustomMode(bool state);
	void SetCheckedButtonsForLobby(bool state);

	void ShowCustomModeSettings();
	void HideCustomModeSettings();

	/*LOBBY DETAILS*/
	void UpdateLobbiesDetails();
	void UpdateCurrentLobbyPlayers();

	bool CheckIfLobbyIsReadyToBegin();
	void SetLobbyDetails(const std::string& lobbyType);
	void CreateNewLobby(const std::string& lobbyType);


private:
	std::string m_ip;

	std::string m_playerUsername;

	uint16_t m_numberOfPlayers;
	uint16_t m_numberOfRounds;
	uint16_t m_mapHeight;
	uint16_t m_mapWidth;

	uint8_t m_firstEmptyPlayerSeatID;

	int lobbyID;

	bool m_isLobbyReadyToBegin;
	GameState m_startingGame;

private:
	QPushButton* buttonSender;

	QTimer* timerToUpdateLobbyDetails = new QTimer(this);
	QTimer* transferToGameTimer = new QTimer(this);

private:
	Game* triviadorGame;
};
