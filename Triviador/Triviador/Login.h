#pragma once

#include <QDialog>

#include "ui_Login.h"

#include <QFile>
#include <QTextStream>

#include <string>

#include <curl/curl.h>
#include <cpr/cpr.h>

class Login : public QDialog
{
	Q_OBJECT

private:
	Ui::Login ui;

public:
	explicit Login();
	~Login();

private slots:
	void on_loginPushButton_released();

	void on_displayPasswordPushButton_pressed();
	void on_displayPasswordPushButton_released();

public:
	const std::string GetUsername() const;
	const std::string GetPassword() const;
};