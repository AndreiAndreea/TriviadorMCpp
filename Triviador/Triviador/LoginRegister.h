#pragma once

#include <QDialog>

#include "ui_LoginRegister.h"

#include "Register.h"
#include "Login.h"

class LoginRegister : public QDialog
{
	Q_OBJECT

private:
	Ui::LoginRegister ui;
public:
	explicit LoginRegister();
	~LoginRegister();

private:
	std::vector<std::string> SplitIP();
	bool IsNumber(const std::string& str);

	QString CheckServerIP();
	QString CheckServerPort();

private slots:
	void on_connectButton_clicked();

	void on_signInButton_released();
	void on_signUpButton_released();

	void backToServerFromLoginButton();
	void backToMenuFromLoginOrRegisterButton();
	void backToLoginFromRegisterButton();

	void ServerCrashedDetails();

	void closeEvent(QCloseEvent* e);

	void LogoutUserForced();

private:
	std::string m_serverIP, m_serverPort;
	std::string m_playerUsername;

private:
	Register* RegisterWindow;
	Login* LoginWindow;
};

