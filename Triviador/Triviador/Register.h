#pragma once

#include <QDialog>

#include "ui_Register.h"
#include "Player.h"

#include <ctime>
#include <iomanip>

#include <QFile>
#include <QTextStream>

#include <curl/curl.h>
#include <cpr/cpr.h>
#include <crow.h>

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

	const std::string GetInputUsernameByPlayer() const;
	const std::string GetInputPasswordByPlayer() const;
	const std::string GetInputEmailByPlayer() const;

private slots:
	void on_displayPasswordPushButton_pressed();
	void on_displayPasswordPushButton_released();

	void on_submitDataPushButton_released();
	void on_registerBackPushButton_released();

private:
	Player m_currentPlayer;
};