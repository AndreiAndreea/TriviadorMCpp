#pragma once

#include <QDialog>

#include "ui_Register.h"

#include <ctime>
#include <iomanip>

#include "Login.h"

class Register : public QDialog
{
	Q_OBJECT

private:
	Ui::Register ui;

public:
	explicit Register();
	Register(const std::string& serverIP, const std::string& serverPort);

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

	void OnTimerTick();

signals:
	void BackToMenu();
	void BackToLogin();

public:
	void StartTimer();

private:
	QTimer* timer;

	Player m_currentPlayer;

	std::string m_ip, m_serverIP, m_serverPort;
};