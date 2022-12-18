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
    ui.serverErrorLabel->hide();

    ui.loginRegisterWidget->hide();
}

LoginRegister::~LoginRegister()
{

}

std::vector<std::string> LoginRegister::SplitIP()
{
    int i = 0;

    std::vector<std::string> ip;

    auto pos = m_serverIP.find(".");

    while (pos != std::string::npos)
    {
        ip.push_back(m_serverIP.substr(i, pos - i));

        i = ++pos;

        pos = m_serverIP.find(".", pos);
    }

    ip.push_back(m_serverIP.substr(i, m_serverIP.length()));

    return ip;
}

bool LoginRegister::IsNumber(const std::string& str)
{
    for (const auto& c : str)
        if (!isdigit(c))
            return false;

    return true;
}

QString LoginRegister::CheckServerIP()
{
    if (m_serverIP == "localhost")
        return "Server IP is valid!";

    std::vector<std::string> ipTokens = SplitIP();

    if (m_serverIP.size() == 0)
        return "No server IP inserted!";
    else if (ipTokens.size() != 4)
        return "Not valid IP! Not enough tokens!";

    // checks that tokens are numbers, positive and in range
    for (const auto& str : ipTokens) {
        if (!IsNumber(str))
            return "Not valid IP! Tokens contain non-digit values!";
        else if (stoi(str) > 255)
            return "Not valid IP! Token values bigger than 255 not allowed!";
    }

    return "Server IP is valid!";
}

QString LoginRegister::CheckServerPort()
{
    if (m_serverPort.size() == 0)
        return "No server Port inserted!";
    else if (!IsNumber(m_serverPort))
        return "Not valid Port! Contains non-digit values!";

    return "Server port is valid!";
}

void LoginRegister::on_connectButton_released()
{
    ui.serverErrorLabel->hide();

    m_serverIP = ui.serverIPLineEdit->text().toStdString();
    m_serverPort = ui.serverPortLineEdit->text().toStdString();

    QString ipErrorText = CheckServerIP();
    QString portErrorText = CheckServerPort();

    ui.ipErrorLabel->setText(ipErrorText);
    ui.ipErrorLabel->show();

    ui.portErrorLabel->setText(portErrorText);
    ui.portErrorLabel->show();

    if (ipErrorText == "Server IP is valid!" && portErrorText == "Server port is valid!")
    {
        std::string link = "http://" + m_serverIP + ":" + m_serverPort + "/";

        cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

        if (responseFromServer.status_code == 200)
        {
            ui.serverDataWidget->hide();
            ui.loginRegisterWidget->show();

            ui.ipErrorLabel->hide();
            ui.portErrorLabel->hide();
            ui.serverErrorLabel->hide();
        }
        else
        {
            ui.serverErrorLabel->setText("Server is offline. Please try again later!");
            ui.serverErrorLabel->show();
        }
    }
}

void LoginRegister::on_signInButton_released()
{
    if (ui.signInButton->isEnabled())
    {
        Login* loginWindow = new Login("http://" + m_serverIP + ":" + m_serverPort);
        loginWindow->show();

        this->hide();
    }
}

void LoginRegister::on_signUpButton_released()
{
    if (ui.signUpButton->isEnabled())
    {
        Register* registerWindow = new Register(m_serverIP, m_serverPort);
        registerWindow->show();

        this->hide();
    }
}