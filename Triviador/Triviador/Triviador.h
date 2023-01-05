#pragma once

#include "ui_Triviador.h"

#include "Game.h"

#include <QDebug>

#include <curl/curl.h>
#include <cpr/cpr.h>

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
    void on_playGamePushButton_released();
    void on_backToMenuPushButton_released();
    void on_quitPushButton_clicked();
    
    void on_profilePushButton_released();
    void on_profileSettingsPushButton_released();

    void on_backToProfileButton_released();
    void on_backToMenuPushButton_2_released();

    void on_changeUsernamePushButton_released();
    void on_changePasswordPushButton_released();
    void on_changeEmailPushButton_released();

    void on_saveProfileSettingsPushButton_released();
    
    void on_twoPlayersPushButton_released();
	void on_threePlayersPushButton_released();
	void on_fourPlayersPushButton_released();
    void on_customModePushButton_released();

    void on_playersSpinBox_valueChanged(int arg1);

    void on_joinLobbyPushButton_released();
    void on_backToLobbyPushButton_released();

    void on_readyGameLobbyPushButton_released();
    void on_startGameLobbyPushButton_released();
    
    void TimerMethodToUpdateLobbyDetails();

signals:
    void BackToMenuSignal();
    void BackToLoginSignal();
    
    void ServerCrashedSignalTriviador();

private:
    void UpdateUserDetails();

    void UpdateLobbiesDetails();
    void UpdateCurrentLobbyPlayers();
    bool CheckIfLobbyIsReadyToBegin();

    void SetLobbyDetails(const std::string& lobbyType);
	void CreateNewLobby(const std::string& lobbyType);

	void ShowCustomModeSettings();
	void HideCustomModeSettings();

	void TurnAutoExclusiveButtonsForCustomMode(bool state);
	void SetCheckedButtonsForLobby(bool state);

private:
    std::string m_ip;
    
	std::string m_playerUsername;

    uint8_t m_firstEmptyPlayerSeatID;

    int lobbyID;

private:
    QPushButton* buttonSender;

    QTimer* timerToUpdateLobbyDetails = new QTimer(this);

private:
    Game* triviadorGame;
};
