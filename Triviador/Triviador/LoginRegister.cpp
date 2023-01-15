#include "LoginRegister.h"

LoginRegister::LoginRegister()
{
	ui.setupUi(this);

	ui.ipErrorLabel->hide();
	ui.portErrorLabel->hide();
	ui.serverErrorLabel->hide();

	ui.serverIPLineEdit->setText("");
	ui.serverPortLineEdit->setText("");	

	this->setWindowTitle("Triviador");

	ui.stackedWidget->setCurrentIndex(0);

	ui.serverIPLineEdit->setFocus();

	LoginWindow = new Login("","");//temp fix for reading invalid memory location in Login UI

	ui.greetingLabel->setFont(QFont("OldEnglish", 20, QFont::Bold));
	
	QPixmap background;
	background.load("resources/img/icons/papyrus_background.jpg");
	background = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	QPalette palette;
	palette.setBrush(QPalette::Window, background);
	this->setPalette(palette);

	QPixmap pixmap("resources/img/icons/button.jpg");
	int w = ui.connectButton->width();
	int h = ui.connectButton->height();
	ui.connectButton->setIcon(pixmap);	
	ui.connectButton->setFixedSize(w, h);
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

void LoginRegister::on_connectButton_clicked()
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
			ui.signInButton->setFocus();

			ui.ipErrorLabel->hide();
			ui.portErrorLabel->hide();
			ui.serverErrorLabel->hide();

			ui.serverIPLineEdit->setText("");
			ui.serverPortLineEdit->setText("");
			
			ui.stackedWidget->setCurrentIndex(1);

			LoginWindow = new Login(m_serverIP, m_serverPort);
			ui.stackedWidget->insertWidget(2, LoginWindow);

			RegisterWindow = new Register(m_serverIP, m_serverPort);
			ui.stackedWidget->insertWidget(3, RegisterWindow);
			
			connect(LoginWindow, SIGNAL(BackToMenu()), this, SLOT(backToMenuFromLoginOrRegisterButton()));
			connect(LoginWindow, SIGNAL(BackToServer()), this, SLOT(backToServerFromLoginButton()));

			connect(RegisterWindow, SIGNAL(BackToMenu()), this, SLOT(backToMenuFromLoginOrRegisterButton()));
			connect(RegisterWindow, SIGNAL(BackToLogin()), this, SLOT(backToLoginFromRegisterButton()));

			connect(LoginWindow, SIGNAL(CloseApplicationSignal()), this, SLOT(close()));
			
			connect(LoginWindow, SIGNAL(ServerCrashedSignalLogin()), this, SLOT(ServerCrashedDetails()));
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
		ui.stackedWidget->setCurrentIndex(2);
	}
}

void LoginRegister::on_signUpButton_released()
{
	if (ui.signUpButton->isEnabled())
	{
		ui.stackedWidget->setCurrentIndex(3); 
	}
}

void LoginRegister::backToServerFromLoginButton()
{
	ui.stackedWidget->setCurrentIndex(0);
}

void LoginRegister::backToMenuFromLoginOrRegisterButton()
{
	ui.stackedWidget->setCurrentIndex(1);
}

void LoginRegister::backToLoginFromRegisterButton()
{
	ui.stackedWidget->setCurrentIndex(2);
}

void LoginRegister::ServerCrashedDetails()
{
	LogoutUserForced();
	
	ui.stackedWidget->setCurrentIndex(0);
	
	ui.serverErrorLabel->show();
	ui.serverErrorLabel->setText("The server is not responding and has closed automatically. Please try again later!");
}

void LoginRegister::closeEvent(QCloseEvent* e)
{
	LogoutUserForced();

	e->accept();
}

void LoginRegister::LogoutUserForced()
{
	//update the database with the new credentials of the user before closing the application by pressing the 'X' button or by pressing 'ALT+F4' or by pressing the 'EXIT' button on the window
	if (LoginWindow->GetUsername().empty() == false && LoginWindow->GetPassword().empty() == false)
	{
		m_playerUsername = LoginWindow->GetUsername();

		std::string link = "http://" + m_serverIP + ":" + m_serverPort + "/getuserdata/?username=" + m_playerUsername;

		cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

		if (responseFromServer.status_code == 200)
		{
			auto userDetails = crow::json::load(responseFromServer.text);

			std::string db_status = userDetails["ConnectStatus"].s();

			if (db_status == "Online")
			{
				link = "http://" + m_serverIP + ":" + m_serverPort + "/leaveRoomForced/?playerUsername=" + m_playerUsername;

				responseFromServer = cpr::Get(cpr::Url(link));

				link = "http://" + m_serverIP + ":" + m_serverPort + "/logoutuser/?username=" + m_playerUsername;

				responseFromServer = cpr::Get(cpr::Url(link));
			}
		}
	}
}
