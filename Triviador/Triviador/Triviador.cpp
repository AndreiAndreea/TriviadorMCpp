#include "Triviador.h"

Triviador::Triviador(QWidget* parent)
	: QMainWindow(parent)
{

}

Triviador::Triviador(const std::string& ip, const std::string& playerUsername)
{
	ui.setupUi(this);

	this->setWindowTitle("Triviador");

	m_ip = ip;
	m_playerUsername = playerUsername;

	ui.startGameLobbyPushButton->hide();

	ui.changeUsernameLineEdit->hide();
	ui.changePasswordLineEdit->hide();
	ui.changeEmailLineEdit->hide();

	ui.userErrorLabel->hide();

	ui.changeUsernameMessageLabel->hide();
	ui.changePasswordMessageLabel->hide();
	ui.changeEmailMessageLabel->hide();

	ui.playersSpinBox->setRange(2, 6);
	ui.roundsSpinBox->setRange(2, 25);
	ui.mapHeightSpinBox->setRange(3, 25);
	ui.mapWidthSpinBox->setRange(3, 25);

	ui.playersSpinBox->hide();
	ui.roundsSpinBox->hide();
	ui.mapHeightSpinBox->hide();
	ui.mapWidthSpinBox->hide();

	ui.playersLabel->hide();
	ui.roundsLabel->hide();
	ui.mapHeightLabel->hide();
	ui.mapWidthLabel->hide();
	
	ui.updateUserDetailsMessageLabel->hide();

	ui.joinLobbyPushButton->hide();

	ui.saveProfileSettingsPushButton->setDisabled(true);

	ui.stackedWidget->setCurrentIndex(0);
}

Triviador::~Triviador()
{
	
}

/*MENU*/
void Triviador::on_playGamePushButton_released()
{
	ui.stackedWidget->setCurrentIndex(3);

	if (ui.customModePushButton->isChecked())
		HideCustomModeSettings();

	UpdateLobbiesDetails();

	TurnAutoExclusiveButtonsForCustomMode(false);

	SetCheckedButtonsForLobby(false);

	ui.joinLobbyPushButton->hide();
	
	TimerMethodToUpdateLobbyDetails();

	connect(timerToUpdateLobbyDetails, SIGNAL(timeout()), this, SLOT(TimerMethodToUpdateLobbyDetails()));
}

void Triviador::on_profilePushButton_released()
{
	UpdateUserDetails();
	UpdateGamesHistory();
}

void Triviador::on_quitPushButton_clicked()
{
	std::string link = m_ip + "/logoutuser/?username=" + m_playerUsername;

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	emit BackToLoginSignal();
}


/*PLAYER PROFILE*/

void Triviador::on_profileSettingsPushButton_released()
{
	ui.stackedWidget->setCurrentIndex(2);
}

void Triviador::on_backToMenuPushButton_released()
{
	ui.stackedWidget->setCurrentIndex(0);
}

void Triviador::UpdateUserDetails()
{
	ui.userErrorLabel->hide();

	ui.stackedWidget->setCurrentIndex(1);

	std::string link = m_ip + "/getuserdata/?username=" + m_playerUsername;

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	if (responseFromServer.status_code >= 200 && responseFromServer.status_code < 300)
	{
		auto db_user = crow::json::load(responseFromServer.text);

		ui.userProfileUsernameLabel->setText(QString::fromStdString(db_user["Username"].s()));

		ui.userProfileEmailLabel->setText(QString::fromStdString(db_user["Email"].s()));

		ui.userProfilePlayedGamesLabel->setText(QString::fromStdString(db_user["PlayedGames"].s()));

		ui.userProfileWonGamesLabel->setText(QString::fromStdString(db_user["WonGames"].s()));

		ui.userProfileCreationDateLabel->setText(QString::fromStdString(db_user["AccountCreationDate"].s()));
	}
	else if (responseFromServer.status_code >= 400 && responseFromServer.status_code < 500)
	{
		ui.userErrorLabel->setText("Error: " + QString::fromStdString(std::to_string(responseFromServer.status_code)) + "\n" + QString::fromStdString(responseFromServer.text));
		ui.userErrorLabel->show();
	}
	else
	{
		emit ServerCrashedSignalTriviador();
	}
}

void Triviador::UpdateGamesHistory()
{
	ui.gamesHistoryTableWidget->setRowCount(6);
	ui.gamesHistoryTableWidget->setColumnCount(10);
	
	QTableWidgetItem* item0 = new QTableWidgetItem("game_type");
	ui.gamesHistoryTableWidget->setItem(0, 0, item0);
	QTableWidgetItem* item1 = new QTableWidgetItem("room_number");
	ui.gamesHistoryTableWidget->setItem(0, 1, item1);
	QTableWidgetItem* item2 = new QTableWidgetItem("number_of_players");
	ui.gamesHistoryTableWidget->setItem(0, 2, item2);
	QTableWidgetItem* item3 = new QTableWidgetItem("player1");
	ui.gamesHistoryTableWidget->setItem(0, 3, item3);
	QTableWidgetItem* item4 = new QTableWidgetItem("player2");
	ui.gamesHistoryTableWidget->setItem(0, 4, item4);
	QTableWidgetItem* item5 = new QTableWidgetItem("player3");
	ui.gamesHistoryTableWidget->setItem(0, 5, item5);
	QTableWidgetItem* item6 = new QTableWidgetItem("player4");
	ui.gamesHistoryTableWidget->setItem(0, 6, item6);
	QTableWidgetItem* item7 = new QTableWidgetItem("player5");
	ui.gamesHistoryTableWidget->setItem(0, 7, item7);
	QTableWidgetItem* item8 = new QTableWidgetItem("player6");
	ui.gamesHistoryTableWidget->setItem(0, 8, item8);
	QTableWidgetItem* item9 = new QTableWidgetItem("winner");
	ui.gamesHistoryTableWidget->setItem(0, 9, item9);
	
	//the last five games will be displayed
	for (int i = 1; i < 6; i++) {
		std::string text = std::to_string(i + 1) + "players";
		QTableWidgetItem* item0 = new QTableWidgetItem(text.c_str());
		ui.gamesHistoryTableWidget->setItem(i, 0, item0);
		
		text = std::to_string(i + 1) + "P_" + std::to_string(i);
		QTableWidgetItem* item1 = new QTableWidgetItem(text.c_str());
		ui.gamesHistoryTableWidget->setItem(i, 1, item1);
		
		QTableWidgetItem* item2 = new QTableWidgetItem(std::to_string(i + 1).c_str());
		ui.gamesHistoryTableWidget->setItem(i, 2, item2);
		
		QTableWidgetItem* item3 = new QTableWidgetItem("Ana");
		ui.gamesHistoryTableWidget->setItem(i, 3, item3);
		
		QTableWidgetItem* item4 = new QTableWidgetItem("Maria");
		ui.gamesHistoryTableWidget->setItem(i, 4, item4);
		
		QTableWidgetItem* item5 = new QTableWidgetItem("Andreea");
		ui.gamesHistoryTableWidget->setItem(i, 5, item5);
		
		QTableWidgetItem* item6 = new QTableWidgetItem("Theo");
		ui.gamesHistoryTableWidget->setItem(i, 6, item6);
		
		QTableWidgetItem* item7 = new QTableWidgetItem("Cosmin");
		ui.gamesHistoryTableWidget->setItem(i, 7, item7);
		
		QTableWidgetItem* item8 = new QTableWidgetItem("-");
		ui.gamesHistoryTableWidget->setItem(i, 8, item8);
		
		QTableWidgetItem* item9 = new QTableWidgetItem("Maria");
		ui.gamesHistoryTableWidget->setItem(i, 9, item9);
	}
}


/*CHANGE CREDENTIALS*/

void Triviador::on_changeUsernamePushButton_released()
{
	ui.changeUsernameLineEdit->show();

	ui.updateUserDetailsMessageLabel->hide();

	std::string newUsername = ui.changeUsernameLineEdit->text().toLocal8Bit().constData();

	std::string link = m_ip + "/findusernameduplicate/?new_username=" + newUsername;

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	if (responseFromServer.status_code == 404)
	{
		if (newUsername != "")
		{
			ui.changeUsernameMessageLabel->setText("Valid choice!");
			ui.saveProfileSettingsPushButton->setDisabled(false);
		}
		else
		{
			ui.changeUsernameMessageLabel->setText("Enter a new valid username and press the change username button!");
			ui.saveProfileSettingsPushButton->setDisabled(true);
		}
	}
	else
	{
		ui.changeUsernameMessageLabel->setText("This username is already used!");
		ui.saveProfileSettingsPushButton->setDisabled(true);
	}

	ui.changeUsernameMessageLabel->show();
}

void Triviador::on_changePasswordPushButton_released()
{
	ui.changePasswordLineEdit->show();

	ui.updateUserDetailsMessageLabel->hide();

	std::string newPassword = ui.changePasswordLineEdit->text().toLocal8Bit().constData();

	std::string link = m_ip + "/findpasswordduplicate/?new_password=" + newPassword;

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	if (responseFromServer.status_code == 404)
	{
		if (newPassword != "")
		{
			ui.changePasswordMessageLabel->setText("Valid choice!");
			ui.saveProfileSettingsPushButton->setDisabled(false);
		}
		else
		{
			ui.changePasswordMessageLabel->setText("Enter a new valid password and press the change password button!");
			ui.saveProfileSettingsPushButton->setDisabled(true);
		}
	}
	else
	{
		ui.changePasswordMessageLabel->setText("This password is already used!");
		ui.saveProfileSettingsPushButton->setDisabled(true);
	}

	ui.changePasswordMessageLabel->show();
}

void Triviador::on_changeEmailPushButton_released()
{
	ui.changeEmailLineEdit->show();

	ui.updateUserDetailsMessageLabel->hide();

	std::string newEmail = ui.changeEmailLineEdit->text().toLocal8Bit().constData();

	if (isValidEmail(newEmail) == true) 
	{
		std::string link = m_ip + "/findemailduplicate/?new_email=" + newEmail;

		cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

		if (responseFromServer.status_code == 404)
		{
			if (newEmail != "")
			{
				ui.changeEmailMessageLabel->setText("Valid choice!");
				ui.saveProfileSettingsPushButton->setDisabled(false);
			}
			else
			{
				ui.changeEmailMessageLabel->setText("Enter a new valid password and press the change password button!");
				ui.saveProfileSettingsPushButton->setDisabled(true);
			}
		}
		else
		{
			ui.changeEmailMessageLabel->setText("This password is already used!");
			ui.saveProfileSettingsPushButton->setDisabled(true);
		}
	}
	else
		ui.changeEmailMessageLabel->setText("Invalid email!");

	ui.changeEmailMessageLabel->show();
}

bool Triviador::isValidEmail(const std::string& email)
{
	std::regex validEmailPattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");

	return regex_match(email, validEmailPattern);
}

void Triviador::on_saveProfileSettingsPushButton_released()
{
	std::string currentUsername = m_playerUsername;
	std::string newUsername = ui.changeUsernameLineEdit->text().toStdString();
	std::string newPassword = ui.changePasswordLineEdit->text().toStdString();
	std::string newEmail = ui.changeEmailLineEdit->text().toStdString();
	
	std::string link = m_ip + "/updateuser/?current_username=" + currentUsername + "&new_username=" + newUsername + "&new_password=" + newPassword + "&new_email=" + newEmail;

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	if (responseFromServer.status_code >= 200 && responseFromServer.status_code < 300)
	{
		if (newUsername.empty() == false)
		{
			m_playerUsername = newUsername;
		}

		ui.changeUsernameLineEdit->setText("");
		ui.changePasswordLineEdit->setText("");
		ui.changeEmailLineEdit->setText("");

		ui.changeUsernameLineEdit->hide();
		ui.changePasswordLineEdit->hide();
		ui.changeEmailLineEdit->hide();

		ui.updateUserDetailsMessageLabel->setText(responseFromServer.text.c_str());
		ui.updateUserDetailsMessageLabel->show();

		ui.changeUsernameMessageLabel->hide();
		ui.changePasswordMessageLabel->hide();
		ui.changeEmailMessageLabel->hide();

		ui.saveProfileSettingsPushButton->setDisabled(true);
	}
	else if (responseFromServer.status_code >= 400 && responseFromServer.status_code < 500)
	{
		std::string text = "Error: " + std::to_string(responseFromServer.status_code) + "\n" + responseFromServer.text;

		ui.updateUserDetailsMessageLabel->setText(text.c_str());
		ui.updateUserDetailsMessageLabel->show();
	}
	else
	{
		emit ServerCrashedSignalTriviador();
	}
}

void Triviador::on_backToProfileButton_released()
{
	ui.stackedWidget->setCurrentIndex(1);

	UpdateUserDetails();
}


/*AVAILABLE LOBBIES*/

void Triviador::on_backToMenuPushButton_2_released()
{
	ui.stackedWidget->setCurrentIndex(0);
}

void Triviador::on_twoPlayersPushButton_released()
{
	if (ui.twoPlayersPushButton->autoExclusive() == false)
		TurnAutoExclusiveButtonsForCustomMode(true);

	if(ui.playersSpinBox->isHidden() == false)
		HideCustomModeSettings();

	ui.joinLobbyPushButton->show();

	buttonSender = ui.twoPlayersPushButton;
}

void Triviador::on_threePlayersPushButton_released()
{
	if (ui.twoPlayersPushButton->autoExclusive() == false)
		TurnAutoExclusiveButtonsForCustomMode(true);
	
	if (ui.playersSpinBox->isHidden() == false)
		HideCustomModeSettings();

	ui.joinLobbyPushButton->show();

	buttonSender = ui.threePlayersPushButton;
}

void Triviador::on_fourPlayersPushButton_released()
{
	if (ui.twoPlayersPushButton->autoExclusive() == false)
		TurnAutoExclusiveButtonsForCustomMode(true);
	
	if (ui.playersSpinBox->isHidden() == false)
		HideCustomModeSettings();

	ui.joinLobbyPushButton->show();

	buttonSender = ui.fourPlayersPushButton;
}

void Triviador::TurnAutoExclusiveButtonsForCustomMode(bool state)
{
	ui.twoPlayersPushButton->setAutoExclusive(state);
	ui.threePlayersPushButton->setAutoExclusive(state);
	ui.fourPlayersPushButton->setAutoExclusive(state);

	ui.customModePushButton->setAutoExclusive(state);
}

void Triviador::SetCheckedButtonsForLobby(bool state)
{
	ui.twoPlayersPushButton->setChecked(state);
	ui.threePlayersPushButton->setChecked(state);
	ui.fourPlayersPushButton->setChecked(state);

	ui.customModePushButton->setChecked(state);
}

void Triviador::on_customModePushButton_released()
{
	if (ui.twoPlayersPushButton->autoExclusive() == false)
		TurnAutoExclusiveButtonsForCustomMode(true);
	
	ShowCustomModeSettings();

	ui.joinLobbyPushButton->show();

	buttonSender = ui.customModePushButton;
}

void Triviador::ShowCustomModeSettings()
{
	ui.playersSpinBox->show();
	ui.roundsSpinBox->show();
	ui.mapHeightSpinBox->show();
	ui.mapWidthSpinBox->show();

	ui.playersLabel->show();
	ui.roundsLabel->show();
	ui.mapHeightLabel->show();
	ui.mapWidthLabel->show();
}

void Triviador::HideCustomModeSettings()
{
	ui.playersSpinBox->hide();
	ui.roundsSpinBox->hide();
	ui.mapHeightSpinBox->hide();
	ui.mapWidthSpinBox->hide();

	ui.playersLabel->hide();
	ui.roundsLabel->hide();
	ui.mapHeightLabel->hide();
	ui.mapWidthLabel->hide();
}

void Triviador::on_playersSpinBox_valueChanged(int arg1)
{
	ui.roundsSpinBox->setMinimum(arg1);
	ui.roundsSpinBox->setMaximum(arg1 * 3 / 2);
	ui.roundsSpinBox->setValue(arg1);

	ui.mapWidthSpinBox->setMinimum(arg1 + 1);
	ui.mapWidthSpinBox->setMaximum(arg1 * 2);
	ui.mapWidthSpinBox->setValue(arg1 + 1);

	ui.mapHeightSpinBox->setMinimum(arg1 + 1);
	ui.mapHeightSpinBox->setMaximum(arg1 * 2);
	ui.mapHeightSpinBox->setValue(arg1 + 1);
}

void Triviador::on_joinLobbyPushButton_released()
{
	ui.stackedWidget->setCurrentIndex(4);

	ui.playersListWidget->clear();

	QString buttonText = buttonSender->text(); // retrive the text from the button clicked

	std::string lobbyType;

	m_numberOfRounds = 0;
	m_mapHeight = 0;
	m_mapWidth = 0;

	if (buttonText == "2 players")
	{
		lobbyType = "2players";

		m_numberOfPlayers = 2;
	}	
	else if (buttonText == "3 players")
	{
		lobbyType = "3players";

		m_numberOfPlayers = 3;
	}
	else if (buttonText == "4 players")
	{
		lobbyType = "4players";

		m_numberOfPlayers = 4;
	}
	else if (buttonText == "custom")
	{
		lobbyType = "customMode";

		m_numberOfPlayers = ui.playersSpinBox->value();
		m_numberOfRounds = ui.roundsSpinBox->value();
		m_mapHeight = ui.mapHeightSpinBox->value();
		m_mapWidth = ui.mapWidthSpinBox->value();
	}

	std::string link = m_ip + "/getAvailableLobby/?gameType=" + lobbyType;

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	if (responseFromServer.status_code >= 200 && responseFromServer.status_code < 300)
	{
		auto availableLobby = crow::json::load(responseFromServer.text);

		lobbyID = availableLobby["lobbyID"].i();
		int currentNumberOfPlayers = availableLobby["currentNumberOfPlayers"].i();
		int maximNumberOfPlayers = availableLobby["maximNumberOfPlayers"].i();
		std::string roomNumber = availableLobby["roomNumber"].s();

		ui.roomNumberLobbyLabel->setText(QString::fromStdString(availableLobby["roomNumber"].s()));

		ui.idLobbyLabel->setText(QString::fromStdString(std::to_string(availableLobby["lobbyID"].i())));

		link = m_ip + "/getFirstEmptyPlayerSeatID/?lobbyID=" + std::to_string(lobbyID);

		responseFromServer = cpr::Get(cpr::Url(link));

		if (responseFromServer.status_code >= 200 && responseFromServer.status_code < 300)
		{
			auto lobbyDetails = crow::json::load(responseFromServer.text);

			m_firstEmptyPlayerSeatID = lobbyDetails.i();

			link = m_ip +
				"/joinLobby/?lobbyID=" + std::to_string(lobbyID) +
				"&firstEmptyPlayerSeatID=" + std::to_string(m_firstEmptyPlayerSeatID) +
				"&playerUsername=" + m_playerUsername;

			responseFromServer = cpr::Get(cpr::Url(link));

			if (responseFromServer.status_code >= 500)
			{
				emit ServerCrashedSignalTriviador();
			}
		}
		else if (responseFromServer.status_code >= 500)
		{
			emit ServerCrashedSignalTriviador();
		}
	}
	else if (responseFromServer.status_code >= 400 && responseFromServer.status_code < 500)
	{
		CreateNewLobby(lobbyType);
	}
	else
	{
		emit ServerCrashedSignalTriviador();
	}
	
	ui.playersDetailsLobbyLabel->setText(buttonText + " lobby");
}


/*LOBBY DETAILS*/

void Triviador::on_backToLobbyPushButton_released()
{
	std::string link = m_ip + "/leaveLobby/?lobbyID=" + std::to_string(lobbyID) + "&firstEmptyPlayerSeatID=" + std::to_string(m_firstEmptyPlayerSeatID);

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	if (responseFromServer.status_code >= 200 && responseFromServer.status_code < 300)
	{
		ui.stackedWidget->setCurrentIndex(3);

		UpdateLobbiesDetails();

		TurnAutoExclusiveButtonsForCustomMode(false);

		SetCheckedButtonsForLobby(false);
	}
	else if (responseFromServer.status_code >= 500)
	{
		emit ServerCrashedSignalTriviador();
	}
}

void Triviador::on_readyGameLobbyPushButton_released()
{
	ui.readyGameLobbyPushButton->setDisabled(true);
	ui.backToLobbyPushButton->setDisabled(true);

	std::string link = m_ip + "/increaseNumberOfReadyPlayers/?lobbyID=" + std::to_string(lobbyID);

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	if (responseFromServer.status_code >= 500)
	{
		emit ServerCrashedSignalTriviador();
	}
}

void Triviador::on_startGameLobbyPushButton_released()
{	
	triviadorGame = new Game(m_ip, m_playerUsername, m_numberOfPlayers, m_numberOfRounds, m_mapHeight, m_mapWidth);

	ui.stackedWidget->insertWidget(4, triviadorGame);

	ui.stackedWidget->setCurrentIndex(4);
}

void Triviador::TimerMethodToUpdateLobbyDetails()
{
	timerToUpdateLobbyDetails->setInterval(1000);
	timerToUpdateLobbyDetails->setTimerType(Qt::PreciseTimer);

	timerToUpdateLobbyDetails->start();

	if (ui.stackedWidget->currentIndex() == 3)
		UpdateLobbiesDetails();
	else if (ui.stackedWidget->currentIndex() == 4)
	{
		UpdateCurrentLobbyPlayers();
		
		if (CheckIfLobbyIsReadyToBegin() == true)
		{
			if (ui.startGameLobbyPushButton->isHidden() == true)
				ui.startGameLobbyPushButton->show();

			ui.startGameLobbyPushButton->setEnabled(true);
		}
		else
		{
			ui.startGameLobbyPushButton->setDisabled(true);
		}
	}

	if (timerToUpdateLobbyDetails->remainingTimeAsDuration() <= std::chrono::milliseconds(1))
	{		
		timerToUpdateLobbyDetails->stop();

		timerToUpdateLobbyDetails->disconnect();

		TimerMethodToUpdateLobbyDetails();
	}
}

void Triviador::UpdateLobbiesDetails()
{
	SetLobbyDetails("2players");
	SetLobbyDetails("3players");
	SetLobbyDetails("4players");
	SetLobbyDetails("customMode");
}

void Triviador::UpdateCurrentLobbyPlayers()
{
	std::string link = m_ip + "/getLobbyDetails/?lobbyID=" + std::to_string(lobbyID);

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	ui.playersListWidget->clear();

	if (responseFromServer.status_code >= 200 && responseFromServer.status_code < 300)
	{
		auto currentLobbyDetails = crow::json::load(responseFromServer.text);

		int maximNumberOfPlayers = currentLobbyDetails["maximNumberOfPlayers"].i();

		for (int playerIndex = 1; playerIndex <= maximNumberOfPlayers; playerIndex++)
		{
			std::string playerUsername = currentLobbyDetails["player" + std::to_string(playerIndex)].s();

			if (playerUsername.empty() == false)
			{
				ui.playersListWidget->addItem(QString::fromStdString(playerUsername));
			}
		}
	}
	else if (responseFromServer.status_code >= 500 && responseFromServer.status_code < 600)
	{
		emit ServerCrashedSignalTriviador();
	}
}

bool Triviador::CheckIfLobbyIsReadyToBegin()
{
	std::string link = m_ip + "/isLobbyReady/?lobbyID=" + std::to_string(lobbyID);

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	if (responseFromServer.status_code >= 200 && responseFromServer.status_code < 300)
	{
		//auto isLobbyReady = crow::json::load(responseFromServer.text);

		if (responseFromServer.text == "Game is ready to begin!")
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (responseFromServer.status_code >= 500 && responseFromServer.status_code < 600)
	{
		emit ServerCrashedSignalTriviador();
	}
}

void Triviador::SetLobbyDetails(const std::string& lobbyType)
{	
	std::string link = m_ip + "/getAvailableLobby/?gameType=" + lobbyType;

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	if (responseFromServer.status_code >= 200 && responseFromServer.status_code < 400)
	{
		auto availableLobby = crow::json::load(responseFromServer.text);

		if (lobbyType == "2players")
		{
			ui.currentNumberOfPlayers2Label->setText(std::to_string(availableLobby["currentNumberOfPlayers"].i()).c_str());
			ui.maximNumberOfPlayers2Label->setText("/ " + QString(std::to_string(availableLobby["maximNumberOfPlayers"].i()).c_str()));
		}
		
		if (lobbyType == "3players")
		{
			ui.currentNumberOfPlayers3Label->setText(std::to_string(availableLobby["currentNumberOfPlayers"].i()).c_str());
			ui.maximNumberOfPlayers3Label->setText("/ " + QString(std::to_string(availableLobby["maximNumberOfPlayers"].i()).c_str()));
		}
		
		if (lobbyType == "4players")
		{
			ui.currentNumberOfPlayers4Label->setText(std::to_string(availableLobby["currentNumberOfPlayers"].i()).c_str());
			ui.maximNumberOfPlayers4Label->setText("/ " + QString(std::to_string(availableLobby["maximNumberOfPlayers"].i()).c_str()));
		}
		
		if (lobbyType == "customMode")
		{
			ui.currentNumberOfPlayersCustomModeLabel->setText(std::to_string(availableLobby["currentNumberOfPlayers"].i()).c_str());
			ui.maximNumberOfPlayersCustomModeLabel->setText("/ " + QString(std::to_string(availableLobby["maximNumberOfPlayers"].i()).c_str()));
		}
	}
	else if (responseFromServer.status_code >= 400 && responseFromServer.status_code < 500)
	{
		CreateNewLobby(lobbyType);
	}
	else
	{		
		emit ServerCrashedSignalTriviador();
	}
}

void Triviador::CreateNewLobby(const std::string& lobbyType)
{
	std::string link = m_ip + "/createNewLobby/?gameType=" + lobbyType;

	auto responseFromServer = cpr::Get(cpr::Url(link));

	if (responseFromServer.status_code >= 200 && responseFromServer.status_code < 400)
	{
		if (lobbyType == "2players")
		{
			ui.twoPlayersLobbyLabel->setText("New lobby created!");
		}

		if (lobbyType == "3players")
		{
			ui.threePlayersLobbyLabel->setText("New lobby created!");
		}

		if (lobbyType == "4players")
		{
			ui.fourPlayersLobbyLabel->setText("New lobby created!");
		}

		if (lobbyType == "customMode")
		{
			ui.customModeLobbyLabel->setText("New lobby created!");
		}

		SetLobbyDetails(lobbyType);
	}
	else
	{
		emit ServerCrashedSignalTriviador();
	}
}