#pragma once

#include <QDialog>

#include "ui_Login.h"

#include <QFile>
#include <QTextStream>

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