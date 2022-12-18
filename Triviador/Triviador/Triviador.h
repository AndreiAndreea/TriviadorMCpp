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
    void on_quitPushButton_released();
    
    void on_profilePushButton_released();
    void on_profileSettingsPushButton_released();

    void on_backToMenuFromProfileSettingsButton_released();

    void on_changeUsernamePushButton_released();
    void on_changePasswordPushButton_released();
    void on_changeEmailPushButton_released();

    void on_saveProfileSettingsPushButton_released();

private:
    std::string m_ip;
    
	std::string m_playerUsername;
};
