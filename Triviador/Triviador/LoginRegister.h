#pragma once

#include <QDialog>

#include "ui_LoginRegister.h"

#include <QFile>
#include <QTextStream>

class LoginRegister : public QDialog
{
	Q_OBJECT

private:
	Ui::LoginRegister ui;
public:
	explicit LoginRegister();
	~LoginRegister();

	//explicit Login(QWidget* parent); - Cosmin

public:
	std::string GetServerIP();
	std::string GetServerPort();
	
private:
	std::vector<std::string> splitIP();
	bool isNumber(const std::string& str);

	QString checkServerIP();
	QString checkServerPort();

private slots:
	void on_connectButton_released();

	void on_signInButton_released();
	void on_signUpButton_released();

private:
	std::string m_serverIP, m_serverPort;
};

