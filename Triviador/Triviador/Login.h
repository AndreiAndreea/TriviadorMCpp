#pragma once

#include <QDialog>

#include "ui_Login.h"

#include <QFile>
#include <QTextStream>

struct UserPlayer
{
	uint32_t id;

	std::string username;
	std::string password;
	std::string email;

	std::string accountCreationDate;

	std::string score;
	std::string playedGames;
	std::string wonGames;
};

class Login : public QDialog
{
	Q_OBJECT

private:
	Ui::Login ui;

private slots:
	void on_loginPushButton_released();

public:
	explicit Login();
	~Login();

public:
	const std::string& GetUsername() const;
	const std::string& GetPassword() const;	

private:
	bool isCorrectUsernameAndPassword;
};