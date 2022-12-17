#include "Login.h"
#include "Game.h"

#include "Triviador.h"

Login::Login()
{
}

Login::Login(const std::string& ip)
{
	m_ip = ip;

	m_wrongDataInsertionCounter = 0;

	ui.setupUi(this);

	ui.passwordLineEdit->setEchoMode(QLineEdit::Password);

	ui.displayPasswordPushButton->setFlat(true);
	ui.displayPasswordPushButton->setStyleSheet("QPushButton { background-color: transparent; }");

	ui.displayPasswordPushButton->setIcon(QIcon("resources/img/icons/eye-off.png"));

	ui.progressBar->hide();
	ui.transferToTriviadorProgressLabel->hide();

	ui.loginErrorLabel->hide();
}

Login::~Login()
{
}

const std::string Login::GetUsername() const
{
	return ui.usernameLineEdit->text().toStdString();
}

const std::string Login::GetPassword() const
{
	return ui.passwordLineEdit->text().toStdString();
}

void Login::OnPauseTimerTick()
{
	ui.progressBar->setValue(ui.progressBar->value() + 1);

	if (ui.progressBar->value() >= 100)
	{
		ui.usernameLineEdit->setDisabled(false);
		ui.passwordLineEdit->setDisabled(false);
		ui.displayPasswordPushButton->setDisabled(false);
		ui.loginPushButton->setDisabled(false);
		ui.loginBackPushButton->setDisabled(false);

		ui.progressBar->hide();

		pauseTimer->disconnect();
	}
}

void Login::OnTransferTimerTick()
{
	ui.progressBar->setValue(ui.progressBar->value() + 1);

	if (ui.progressBar->value() >= 100)
	{
		Triviador* trivia = new Triviador;
		trivia->show();

		transferTimer->disconnect();

		this->close();
	}
}

void Login::StartPauseTimer()
{
	ui.usernameLineEdit->setDisabled(true);
	ui.passwordLineEdit->setDisabled(true);
	ui.displayPasswordPushButton->setDisabled(true);
	ui.loginPushButton->setDisabled(true);
	ui.loginBackPushButton->setDisabled(true);
	
	ui.progressBar->setValue(0);

	pauseTimer = new QTimer(this);

	pauseTimer->setInterval(200);
	pauseTimer->setTimerType(Qt::PreciseTimer);

	connect(pauseTimer, SIGNAL(timeout()), this, SLOT(OnPauseTimerTick()));

	pauseTimer->start();
}

void Login::StartTransferTimer()
{
	ui.progressBar->setValue(0);

	transferTimer = new QTimer(this);

	transferTimer->setInterval(30);
	transferTimer->setTimerType(Qt::PreciseTimer);

	connect(transferTimer, SIGNAL(timeout()), this, SLOT(OnTransferTimerTick()));

	transferTimer->start();
}

void Login::on_displayPasswordPushButton_pressed()
{
	ui.displayPasswordPushButton->setIcon(QIcon("resources/img/icons/eye-on.png"));

	ui.passwordLineEdit->setEchoMode(QLineEdit::Normal);
}

void Login::on_displayPasswordPushButton_released()
{
	ui.displayPasswordPushButton->setIcon(QIcon("resources/img/icons/eye-off.png"));

	ui.passwordLineEdit->setEchoMode(QLineEdit::Password);
}

void Login::on_loginPushButton_released()
{
	Player currentPlayer;

	std::string usernameFromUser = ui.usernameLineEdit->text().toStdString();
	std::string passwordFromUser = ui.passwordLineEdit->text().toStdString();

	std::string link = m_ip + "/checkuser/?username=" + usernameFromUser + "&password=" + passwordFromUser;
	//std::string link = "localhost:18080/checkuser/?username=" +usernameFromUser + "&password=" + passwordFromUser;

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	if (responseFromServer.status_code >= 200 && responseFromServer.status_code < 300)
	{
		ui.loginErrorLabel->hide();

		auto db_user = crow::json::load(responseFromServer.text);

		currentPlayer.SetID(db_user["ID"].i());
		currentPlayer.SetUsername(db_user["Username"].s());
		currentPlayer.SetPassword(db_user["Password"].s());
		currentPlayer.SetEmail(db_user["Email"].s());
		currentPlayer.SetAccountCreationDate(db_user["AccountCreationDate"].s());
		currentPlayer.SetTotalScore(db_user["TotalScore"].s());
		currentPlayer.SetPlayedGames(db_user["PlayedGames"].s());
		currentPlayer.SetWonGames(db_user["WonGames"].s());
		
		ui.usernameLineEdit->setDisabled(true);
		ui.passwordLineEdit->setDisabled(true);
		
		ui.displayPasswordPushButton->setDisabled(true);

		ui.loginBackPushButton->setDisabled(true);
		
		ui.loginPushButton->setDisabled(true);

		ui.transferToTriviadorProgressLabel->setText("Login successfully! You will be redirected to the game menu in a few moments!");
		ui.transferToTriviadorProgressLabel->show();
		
		ui.progressBar->show();
		
		StartTransferTimer();
	}
	else
	{
		std::string errorText;
		
		if (responseFromServer.status_code >= 400 && responseFromServer.status_code < 500)
			errorText = "Client error: " + std::to_string(responseFromServer.status_code) + "\n" + responseFromServer.text;
		else if (responseFromServer.status_code >= 500)
			errorText = "Server error: " + std::to_string(responseFromServer.status_code) + "\n" + responseFromServer.text;

		m_wrongDataInsertionCounter++;
		if (m_wrongDataInsertionCounter == 3)
		{
			errorText = "Login data was inserted wrong 3 times! Wait to try again!";
			ui.progressBar->show();
			StartPauseTimer();

			m_wrongDataInsertionCounter = 0;
		}

		ui.loginErrorLabel->setText(errorText.c_str());
		ui.loginErrorLabel->show();
	}
}

void Login::on_loginBackPushButton_released()
{
	if (ui.loginBackPushButton->isEnabled())
	{
		LoginRegister* r = new LoginRegister;
		r->show();

		this->close();
	}
}