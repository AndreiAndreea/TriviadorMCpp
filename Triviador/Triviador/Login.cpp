#include "Login.h"
#include "Game.h"
#include "Triviador.h"

Login::Login()
{
	ui.setupUi(this);

	ui.passwordLineEdit->setEchoMode(QLineEdit::Password);

	ui.displayPasswordPushButton->setFlat(true);
	ui.displayPasswordPushButton->setStyleSheet("QPushButton { background-color: transparent; }");

	ui.displayPasswordPushButton->setIcon(QIcon("resources/img/icons/eye-off.png"));

	ui.transferToTriviadorProgressBar->hide();
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

void Login::OnTimerTick()
{
	ui.transferToTriviadorProgressBar->setValue(ui.transferToTriviadorProgressBar->value() + 1);

	if (ui.transferToTriviadorProgressBar->value() >= 100)
	{
		Triviador* t = new Triviador;
		t->show();
		timer->disconnect();

		this->close();
	}
}

void Login::StartTimer()
{
	ui.transferToTriviadorProgressBar->setValue(0);

	timer = new QTimer(this);

	timer->setInterval(100);
	timer->setTimerType(Qt::PreciseTimer);

	connect(timer, SIGNAL(timeout()), this, SLOT(OnTimerTick()));

	timer->start();
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

	std::string usernameFromUser = ui.usernameLineEdit->text().toLocal8Bit().constData();
	std::string passwordFromUser = ui.passwordLineEdit->text().toLocal8Bit().constData();

	std::string link = "http://localhost:18080/checkuser/?username=" +usernameFromUser + "&password=" + passwordFromUser;

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	auto db_user = crow::json::load(responseFromServer.text);

	/*for (const auto& jsonIt : db_user)
	{
		if (jsonIt["Username"].s() == usernameFromUser && jsonIt["Password"].s() == passwordFromUser)
		{
			int idDecodedFromServer = jsonIt["ID"].i();
			std::string usernameDecodedFromServer = jsonIt["Username"].s();
			std::string passwordDecodedFromServer = jsonIt["Password"].s();
			std::string emailDecodedFromServer = jsonIt["Email"].s();
			std::string accountCreationDateDecodedFromServer = jsonIt["AccountCreationDate"].s();
			std::string totalScoreDecodedFromServer = jsonIt["TotalScore"].s();
			std::string playedGamesDecodedFromServer = jsonIt["PlayedGames"].s();
			std::string wonGamesDecodedFromServer = jsonIt["WonGames"].s();

			//idDecodedFromServer = curl_unescape(idDecodedFromServer.c_str(), idDecodedFromServer.length());
			usernameDecodedFromServer = curl_unescape(usernameDecodedFromServer.c_str(), usernameDecodedFromServer.length());
			passwordDecodedFromServer = curl_unescape(passwordDecodedFromServer.c_str(), passwordDecodedFromServer.length());
			emailDecodedFromServer = curl_unescape(emailDecodedFromServer.c_str(), emailDecodedFromServer.length());
			accountCreationDateDecodedFromServer = curl_unescape(accountCreationDateDecodedFromServer.c_str(), accountCreationDateDecodedFromServer.length());
			totalScoreDecodedFromServer = curl_unescape(totalScoreDecodedFromServer.c_str(), totalScoreDecodedFromServer.length());
			playedGamesDecodedFromServer = curl_unescape(playedGamesDecodedFromServer.c_str(), playedGamesDecodedFromServer.length());
			wonGamesDecodedFromServer = curl_unescape(wonGamesDecodedFromServer.c_str(), wonGamesDecodedFromServer.length());

			//currentPlayer.SetID(std::stoi(idDecodedFromServer));
			currentPlayer.SetID(idDecodedFromServer);
			currentPlayer.SetUsername(usernameDecodedFromServer);
			currentPlayer.SetPassword(passwordDecodedFromServer);
			currentPlayer.SetEmail(emailDecodedFromServer);
			currentPlayer.SetAccountCreationDate(accountCreationDateDecodedFromServer);
			currentPlayer.SetTotalScore(totalScoreDecodedFromServer);
			currentPlayer.SetPlayedGames(playedGamesDecodedFromServer);
			currentPlayer.SetWonGames(wonGamesDecodedFromServer);

			ui.transferToTriviadorProgressLabel->setText("Login successful! You will be redirected to the game menu page in a few moments!");
			ui.transferToTriviadorProgressLabel->show();

			ui.transferToTriviadorProgressBar->show();
			StartTimer();

			close();
		}
		else
		{
			ui.loginErrorLabel->setText(responseFromServer.text.c_str());
			ui.loginErrorLabel->show();
		}
	}*/

	/*if (db_user["Username"] == usernameFromUser && db_user["Password"] == passwordFromUser)
	{
		std::string idDecodedFromServer = db_user["ID"].s();
		std::string usernameDecodedFromServer = db_user["Username"].s();
		std::string passwordDecodedFromServer = db_user["Password"].s();
		std::string emailDecodedFromServer = db_user["Email"].s();
		std::string accountCreationDateDecodedFromServer = db_user["AccountCreationDate"].s();
		std::string totalScoreDecodedFromServer = db_user["TotalScore"].s();
		std::string playedGamesDecodedFromServer = db_user["PlayedGames"].s();
		std::string wonGamesDecodedFromServer = db_user["WonGames"].s();

		idDecodedFromServer = curl_unescape(idDecodedFromServer.c_str(), idDecodedFromServer.length());
		usernameDecodedFromServer = curl_unescape(usernameDecodedFromServer.c_str(), usernameDecodedFromServer.length());
		passwordDecodedFromServer = curl_unescape(passwordDecodedFromServer.c_str(), passwordDecodedFromServer.length());
		emailDecodedFromServer = curl_unescape(emailDecodedFromServer.c_str(), emailDecodedFromServer.length());
		accountCreationDateDecodedFromServer = curl_unescape(accountCreationDateDecodedFromServer.c_str(), accountCreationDateDecodedFromServer.length());
		totalScoreDecodedFromServer = curl_unescape(totalScoreDecodedFromServer.c_str(), totalScoreDecodedFromServer.length());
		playedGamesDecodedFromServer = curl_unescape(playedGamesDecodedFromServer.c_str(), playedGamesDecodedFromServer.length());
		wonGamesDecodedFromServer = curl_unescape(wonGamesDecodedFromServer.c_str(), wonGamesDecodedFromServer.length());

		currentPlayer.SetID(std::stoi(idDecodedFromServer));
		currentPlayer.SetUsername(usernameDecodedFromServer);
		currentPlayer.SetPassword(passwordDecodedFromServer);
		currentPlayer.SetEmail(emailDecodedFromServer);
		currentPlayer.SetAccountCreationDate(accountCreationDateDecodedFromServer);
		currentPlayer.SetTotalScore(totalScoreDecodedFromServer);
		currentPlayer.SetPlayedGames(playedGamesDecodedFromServer);
		currentPlayer.SetWonGames(wonGamesDecodedFromServer);

		//Game* g = new Game;
		//g->show();

		close();
	}
	else
	{
		ui.loginErrorLabel->setText(responseFromServer.text.c_str());
		ui.loginErrorLabel->show();
	}*/

	//to be moved in if after login server bug is resolved
	ui.transferToTriviadorProgressLabel->setText("Login successful! You will be redirected to the game menu page in a few moments!");
	ui.transferToTriviadorProgressLabel->show();

	ui.transferToTriviadorProgressBar->show();
	StartTimer();
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