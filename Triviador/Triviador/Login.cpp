#include "Login.h"

QTime timeLeft(0, 2, 0);

Login::Login()
{
}

Login::Login(const std::string& serverIP, const std::string& serverPort)
{
	m_serverIP = serverIP;
	m_serverPort = serverPort;

	m_ip = "http://" + m_serverIP + ":" + m_serverPort;

	m_wrongDataInsertionCounter = 0;
	m_userAlreadyOnlineCounter = 0;

	ui.setupUi(this);

	ui.passwordLineEdit->setEchoMode(QLineEdit::Password);

	ui.displayPasswordPushButton->setFlat(true);
	ui.displayPasswordPushButton->setStyleSheet("QPushButton { background-color: transparent; }");

	ui.displayPasswordPushButton->setIcon(QIcon("resources/img/icons/eye-off.png"));

	ui.progressBar->hide();
	ui.progressBarLeftTimeLabel->hide();
	ui.transferToTriviadorProgressLabel->hide();

	ui.loginErrorLabel->hide();

	ui.usernameLineEdit->setFocus();

	ui.stackedWidget->setCurrentIndex(0);

	triviadorMenu = new Triviador("", "");//temp fix for reading invalid memory location in Triviador UI
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

	timeLeft = timeLeft.addSecs(-1);
	ui.progressBarLeftTimeLabel->setText("0" + timeLeft.toString("m:ss") + " minutes left");

	if (ui.progressBar->value() >= 100)
	{
		ui.usernameLineEdit->setDisabled(false);
		ui.passwordLineEdit->setDisabled(false);
		
		ui.displayPasswordPushButton->setDisabled(false);
		
		ui.loginPushButton->setDisabled(false);
		
		ui.serverDetailsPushButton->setDisabled(false);
		ui.exitPushButton->setDisabled(false);
		ui.mainMenuPushButton->setDisabled(false);

		ui.loginErrorLabel->hide();
		ui.progressBar->hide();
		ui.progressBarLeftTimeLabel->hide();

		pauseTimer->disconnect();
	}
}

void Login::OnTransferTimerTick()
{
	ui.progressBar->setValue(ui.progressBar->value() + 1);

	if (ui.progressBar->value() >= 100)
	{
		triviadorMenu = new Triviador(m_ip, GetUsername());
		ui.stackedWidget->insertWidget(1, triviadorMenu);

		connect(triviadorMenu, SIGNAL(BackToLoginSignal()), this, SLOT(CloseApplicationSlotFromGame()));
		connect(triviadorMenu, SIGNAL(ServerCrashedSignalTriviador()), this, SLOT(ServerCrashedSlot()));

		ui.stackedWidget->setCurrentIndex(1);

		transferTimer->disconnect();
	}
}

void Login::CloseApplicationSlotFromGame()
{
	ui.usernameLineEdit->setText("");

	ui.passwordLineEdit->setText("");

	ui.progressBar->hide();
	ui.progressBarLeftTimeLabel->hide();
	ui.transferToTriviadorProgressLabel->hide();

	ui.usernameLineEdit->setDisabled(false);
	ui.passwordLineEdit->setDisabled(false);

	ui.displayPasswordPushButton->setDisabled(false);

	ui.serverDetailsPushButton->setDisabled(false);
	ui.exitPushButton->setDisabled(false);
	ui.mainMenuPushButton->setDisabled(false);

	ui.loginPushButton->setDisabled(false);
	
	ui.stackedWidget->setCurrentIndex(0);
}

void Login::ServerCrashedSlot()
{
	ui.stackedWidget->setCurrentIndex(0);

	emit ServerCrashedSignalLogin();
}

void Login::StartPauseTimer()
{
	ui.usernameLineEdit->setDisabled(true);
	ui.passwordLineEdit->setDisabled(true);
	
	ui.displayPasswordPushButton->setDisabled(true);
	
	ui.loginPushButton->setDisabled(true);
	
	ui.serverDetailsPushButton->setDisabled(true);
	ui.exitPushButton->setDisabled(true);
	ui.mainMenuPushButton->setDisabled(true);
	
	ui.progressBar->setValue(0);

	pauseTimer = new QTimer(this);

	if (m_wrongDataInsertionCounter == 3)
	{
		pauseTimer->setInterval(1200);
		ui.progressBarLeftTimeLabel->setText("0" + timeLeft.toString("m:ss") + " minutes left");
		ui.progressBarLeftTimeLabel->show();
	}
	else if (m_userAlreadyOnlineCounter == 3)
	{
		pauseTimer->setInterval(3000);
		timeLeft = timeLeft.addSecs(180);
		ui.progressBarLeftTimeLabel->setText("0" + timeLeft.toString("m:ss") + " minutes left");
		ui.progressBarLeftTimeLabel->show();
	}

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

	std::string link = m_ip + "/loginuser/?username=" + usernameFromUser + "&password=" + passwordFromUser;

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

		ui.serverDetailsPushButton->setDisabled(true);
		ui.exitPushButton->setDisabled(true);
		ui.mainMenuPushButton->setDisabled(true);
		
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

		if (responseFromServer.status_code == 409)
			m_userAlreadyOnlineCounter++;
		else
			m_wrongDataInsertionCounter++;

		if (m_wrongDataInsertionCounter == 3)
		{
			errorText = "Login data was inserted wrong 3 times! Wait 2 minutes to try again!";
			ui.progressBar->show();
			StartPauseTimer();

			m_wrongDataInsertionCounter = 0;
		}
		else if (m_userAlreadyOnlineCounter == 3)
		{
			errorText = "The user whose data was inserted 3 times is already online! Wait 5 minutes to try connecting again!";
			ui.progressBar->show();
			StartPauseTimer();

			m_userAlreadyOnlineCounter = 0;
		}

		ui.loginErrorLabel->setText(errorText.c_str());
		ui.loginErrorLabel->show();
	}
}

void Login::on_mainMenuPushButton_clicked()
{
	emit BackToMenu();
}

void Login::on_serverDetailsPushButton_clicked()
{
	emit BackToServer();
}

void Login::on_exitPushButton_clicked()
{
	emit CloseApplicationSignal();
}
