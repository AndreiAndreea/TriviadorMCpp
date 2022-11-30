#pragma once

#include <QDialog>

#include "ui_Register.h"
#include "Player.h"

#include<ctime>

#include <QFile>
#include <QTextStream>

class Register : public QDialog
{
	Q_OBJECT

private:
	Ui::Register ui;

public:
	explicit Register();
	~Register();

	//explicit Login(QWidget* parent);

private:
	std::string GetCurrentDate();
	std::vector<Player> GetPlayers();

	void showPlayersInFile(); //for test purposes only

private slots:
	void on_usernamePushButton_released();
	void on_passwordPushButton_released();
	void on_emailPushButton_released();

	void on_displayPasswordPushButton_pressed();
	void on_displayPasswordPushButton_released();

	void on_submitDataPushButton_released();

private:
	std::vector<Player> m_players;
	Player m_currentPlayer;

	bool isUniqueUsername;
	bool newUsernameIsValid;
	bool newPasswordIsValid;
	bool isUniqueEmail;
	bool newEmailIsValid;
};

