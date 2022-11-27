#pragma once

#include <QDialog>
#include "ui_LoginRegister.h"

#include "Player.h"

class LoginRegister : public QDialog
{
	Q_OBJECT

private:
	Ui::LoginRegister ui;

public:
	explicit LoginRegister();
	~LoginRegister();

	//explicit Login(QWidget* parent);

private slots:
	void on_signInButton_released();
	void on_signUpButton_released();

	void on_usernamePushButton_released();
	void on_passwordPushButton_released();
	void on_emailPushButton_released();

	void on_displayPasswordPushButton_released();

private:
	std::vector<Player> m_players;
	Player m_currentPlayer;

	bool newUsernameIsValid;
	bool newPasswordIsValid;
	bool newEmailIsValid;
};

