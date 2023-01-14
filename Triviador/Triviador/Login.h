#pragma once

#include <QDialog>

#include "ui_Login.h"

#include "Triviador.h"

class Login : public QDialog
{
	Q_OBJECT

private:
	Ui::Login ui;

public:
	explicit Login();
	Login(const std::string& serverIP, const std::string& serverPort);
	
	~Login();

private slots:
	void on_displayPasswordPushButton_pressed();
	void on_displayPasswordPushButton_released();
	
	void on_loginPushButton_released();

	void on_mainMenuPushButton_clicked();
	void on_serverDetailsPushButton_clicked();

	void on_exitPushButton_clicked();

	void OnPauseTimerTick();
	void OnTransferTimerTick();

	void CloseApplicationSlotFromGame();

	void ServerCrashedSlot();
	
signals:
	void BackToMenu();
	void BackToServer();
	
	void CloseApplicationSignal();

	void ServerCrashedSignalLogin();

public:
	const std::string GetUsername() const;
	const std::string GetPassword() const;

	void StartPauseTimer();
	void StartTransferTimer();

private:
	QTimer* pauseTimer;
	QTimer* transferTimer;

	uint16_t m_wrongDataInsertionCounter, m_userAlreadyOnlineCounter;
	std::string m_ip;

	std::string m_serverIP;
	std::string m_serverPort;

private:
	Triviador* triviadorMenu;
};