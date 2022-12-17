#include "LoginRegister.h"

#include "Register.h"
#include "Login.h"

#include <regex>

LoginRegister::LoginRegister()
{
	ui.setupUi(this);

	ui.serverDataWidget->show();
	ui.ipErrorLabel->hide();
	ui.portErrorLabel->hide();

	ui.loginRegisterWidget->hide();
}

LoginRegister::~LoginRegister()
{

}

std::string LoginRegister::GetServerIP()
{
	return m_serverIP;
}

std::string LoginRegister::GetServerPort()
{
	return m_serverPort;
}

std::vector<std::string> LoginRegister::splitIP()
{
	int i = 0;
	std::vector<std::string> ip;
	auto pos = m_serverIP.find(".");
	while ( pos != std::string::npos) 
	{
		ip.push_back(m_serverIP.substr(i, pos - i));
		i = ++pos;
		pos = m_serverIP.find(".", pos);
	}
	ip.push_back(m_serverIP.substr(i, m_serverIP.length()));
	return ip;
}

bool LoginRegister::isNumber(const std::string& str)
{
	for (const auto& c : str)
		if (!isdigit(c))
			return false;
	return true;
}

QString LoginRegister::checkServerIP()
{
	std::vector<std::string> ipTokens = splitIP();

	if (m_serverIP.size() == 0)
		return "No server IP inserted!";
	else if (ipTokens.size() != 4)
		return "Not valid IP! Not enough tokens!";
		
	// checks that tokens are numbers, positive and in range
	for (const auto& str : ipTokens) {
		if (!isNumber(str))
			return "Not valid IP! Tokens contain non-digit values!";
		else if (stoi(str) > 255)
			return "Not valid IP! Token values bigger than 255 not allowed!";
	}

	return "";
}

QString LoginRegister::checkServerPort()
{
	if(m_serverPort.size() == 0)
		return "No server Port inserted!";
	else if (!isNumber(m_serverPort))
		return "Not valid Port! Contains non-digit values!";

	return "";
}

void LoginRegister::on_connectButton_released()
{
	m_serverIP = ui.serverIPLineEdit->text().toStdString();
	m_serverPort = ui.serverPortLineEdit->text().toStdString();

	QString ipErrorText = checkServerIP();
	QString portErrorText = checkServerPort();

	ui.ipErrorLabel->setText(ipErrorText);
	ui.ipErrorLabel->show();

	ui.portErrorLabel->setText(portErrorText);
	ui.portErrorLabel->show();

	if (ipErrorText.isEmpty() && portErrorText.isEmpty())
	{
		ui.serverDataWidget->hide();
		ui.loginRegisterWidget->show();
	}
}

void LoginRegister::on_signInButton_released()
{
	if (ui.signInButton->isEnabled())
	{
		Login* loginWindow = new Login();
		loginWindow->show();
		this->hide();
	}
}

void LoginRegister::on_signUpButton_released()
{
	if (ui.signUpButton->isEnabled())
	{
		Register* registerWindow = new Register();
		registerWindow->show();
		this->hide();
	}
}