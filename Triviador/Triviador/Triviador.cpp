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

	m_isRoomReadyToBegin = false;

	m_startingGame = GameState::NotStarted;

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

	ui.joinRoomPushButton->hide();

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

	SetCheckedButtonsForRoom(false);

	ui.joinRoomPushButton->hide();

	TimerMethodToUpdateRoomDetails();

	connect(timerToUpdateRoomDetails, SIGNAL(timeout()), this, SLOT(TimerMethodToUpdateRoomDetails()));
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
	ui.gamesHistoryTableWidget->setRowCount(5);
	ui.gamesHistoryTableWidget->setColumnCount(9);
	
	QFont font = ui.gamesHistoryTableWidget->horizontalHeader()->font();
	font.setBold(true);
	ui.gamesHistoryTableWidget->horizontalHeader()->setFont(font);
	ui.gamesHistoryTableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
	ui.gamesHistoryTableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{"
		"border-top:0px solid #D8D8D8;"
		"border-left:0px solid #D8D8D8;"
		"border-right:1px solid #D8D8D8;"
		"border-bottom: 1px solid #D8D8D8;"
		"background-color:white;"
		"padding:4px;}");
	
	ui.gamesHistoryTableWidget->verticalHeader()->setFont(font);
	ui.gamesHistoryTableWidget->verticalHeader()->setStyleSheet("QHeaderView::section {border: 1px solid #6c6c6c;}");
	
	ui.gamesHistoryTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Room Number"));
	ui.gamesHistoryTableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Winner"));
	ui.gamesHistoryTableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Number of Players"));
	ui.gamesHistoryTableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Player 1"));
	ui.gamesHistoryTableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Player 2"));
	ui.gamesHistoryTableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Player 3"));
	ui.gamesHistoryTableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem("Player 4"));
	ui.gamesHistoryTableWidget->setHorizontalHeaderItem(7, new QTableWidgetItem("Player 5"));
	ui.gamesHistoryTableWidget->setHorizontalHeaderItem(8, new QTableWidgetItem("Player 6"));
	ui.gamesHistoryTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	std::string link = m_ip + "/getMatchHistory/?username=" + m_playerUsername;

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	if (responseFromServer.status_code >= 200 && responseFromServer.status_code < 300)
	{
		auto db = crow::json::load(responseFromServer.text);
		int row = 0;
		
		for (auto& db_user: db)
		{
			std::string text = std::string(db_user["room_number"]);
			QTableWidgetItem* item0 = new QTableWidgetItem(QString::fromStdString(text.c_str()));
			ui.gamesHistoryTableWidget->setItem(row, 0, item0);

			text = std::string(db_user["winner"]);
			QTableWidgetItem* item1 = new QTableWidgetItem(QString::fromStdString(text.c_str()));
			ui.gamesHistoryTableWidget->setItem(row, 1, item1);

			text = std::string(db_user["number_of_players"]);
			QTableWidgetItem* item2 = new QTableWidgetItem(QString::fromStdString(text.c_str()));
			ui.gamesHistoryTableWidget->setItem(row, 2, item2);

			text = std::string(db_user["player1"]);
			QTableWidgetItem* item3 = new QTableWidgetItem(QString::fromStdString(text.c_str()));
			ui.gamesHistoryTableWidget->setItem(row, 3, item3);

			text = std::string(db_user["player2"]);
			QTableWidgetItem* item4 = new QTableWidgetItem(QString::fromStdString(text.c_str()));
			ui.gamesHistoryTableWidget->setItem(row, 4, item4);

			text = std::string(db_user["player3"]);
			QTableWidgetItem* item5 = new QTableWidgetItem(QString::fromStdString(text.c_str()));
			ui.gamesHistoryTableWidget->setItem(row, 5, item5);

			text = std::string(db_user["player4"]);
			QTableWidgetItem* item6 = new QTableWidgetItem(QString::fromStdString(text.c_str()));
			ui.gamesHistoryTableWidget->setItem(row, 6, item6);

			text = std::string(db_user["player5"]);
			QTableWidgetItem* item7 = new QTableWidgetItem(QString::fromStdString(text.c_str()));
			ui.gamesHistoryTableWidget->setItem(row, 7, item7);

			text = std::string(db_user["player6"]);
			QTableWidgetItem* item8 = new QTableWidgetItem(QString::fromStdString(text.c_str()));
			ui.gamesHistoryTableWidget->setItem(row, 8, item8);
			
			ui.gamesHistoryTableWidget->resizeColumnsToContents();
			for (int i = 0; i < 9; i++)
				ui.gamesHistoryTableWidget->item(row, i)->setTextAlignment(Qt::AlignCenter);

			row++;
		}
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

	if (ui.playersSpinBox->isHidden() == false)
		HideCustomModeSettings();

	ui.joinRoomPushButton->show();

	buttonSender = ui.twoPlayersPushButton;
}

void Triviador::on_threePlayersPushButton_released()
{
	if (ui.twoPlayersPushButton->autoExclusive() == false)
		TurnAutoExclusiveButtonsForCustomMode(true);

	if (ui.playersSpinBox->isHidden() == false)
		HideCustomModeSettings();

	ui.joinRoomPushButton->show();

	buttonSender = ui.threePlayersPushButton;
}

void Triviador::on_fourPlayersPushButton_released()
{
	if (ui.twoPlayersPushButton->autoExclusive() == false)
		TurnAutoExclusiveButtonsForCustomMode(true);

	if (ui.playersSpinBox->isHidden() == false)
		HideCustomModeSettings();

	ui.joinRoomPushButton->show();

	buttonSender = ui.fourPlayersPushButton;
}

void Triviador::TurnAutoExclusiveButtonsForCustomMode(bool state)
{
	ui.twoPlayersPushButton->setAutoExclusive(state);
	ui.threePlayersPushButton->setAutoExclusive(state);
	ui.fourPlayersPushButton->setAutoExclusive(state);

	ui.customModePushButton->setAutoExclusive(state);
}

void Triviador::SetCheckedButtonsForRoom(bool state)
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

	ui.joinRoomPushButton->show();

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

void Triviador::on_joinRoomPushButton_released()
{
	ui.stackedWidget->setCurrentIndex(4);

	ui.playersListWidget->clear();
	
	ui.progressBar->hide();
	ui.gameStartLabel->hide();

	QString buttonText = buttonSender->text(); // retrive the text from the button clicked

	std::string roomType;

	m_numberOfRounds = 0;
	m_mapHeight = 0;
	m_mapWidth = 0;

	if (buttonText == "2 players")
	{
		roomType = "2players";

		m_numberOfPlayers = 2;
	}
	else if (buttonText == "3 players")
	{
		roomType = "3players";

		m_numberOfPlayers = 3;
	}
	else if (buttonText == "4 players")
	{
		roomType = "4players";

		m_numberOfPlayers = 4;
	}
	else if (buttonText == "custom")
	{
		roomType = "customMode";

		m_numberOfPlayers = ui.playersSpinBox->value();
		m_numberOfRounds = ui.roundsSpinBox->value();
		m_mapHeight = ui.mapHeightSpinBox->value();
		m_mapWidth = ui.mapWidthSpinBox->value();
	}

	std::string link = m_ip + "/getAvailableRoom/?gameType=" + roomType;

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	if (responseFromServer.status_code >= 200 && responseFromServer.status_code < 300)
	{
		auto availableRoom = crow::json::load(responseFromServer.text);

		m_roomID = availableRoom["roomID"].i();
		int currentNumberOfPlayers = availableRoom["currentNumberOfPlayers"].i();
		int maximNumberOfPlayers = availableRoom["maximNumberOfPlayers"].i();
		std::string roomNumber = availableRoom["roomNumber"].s();

		ui.roomNumberRoomLabel->setText(QString::fromStdString(availableRoom["roomNumber"].s()));

		ui.idRoomLabel->setText(QString::fromStdString(std::to_string(availableRoom["roomID"].i())));

		link = m_ip + "/getFirstEmptyPlayerSeatID/?roomID=" + std::to_string(m_roomID);

		responseFromServer = cpr::Get(cpr::Url(link));

		if (responseFromServer.status_code >= 200 && responseFromServer.status_code < 300)
		{
			auto roomDetails = crow::json::load(responseFromServer.text);

			m_firstEmptyPlayerSeatID = roomDetails.i();

			link = m_ip +
				"/joinRoom/?roomID=" + std::to_string(m_roomID) +
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
		CreateNewRoom(roomType);
	}
	else
	{
		emit ServerCrashedSignalTriviador();
	}

	ui.playersDetailsRoomLabel->setText(buttonText + " room");
}


/*LOBBY DETAILS*/

void Triviador::on_backToRoomPushButton_released()
{
	std::string link = m_ip + "/leaveRoom/?roomID=" + std::to_string(m_roomID) + "&firstEmptyPlayerSeatID=" + std::to_string(m_firstEmptyPlayerSeatID);

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	if (responseFromServer.status_code >= 200 && responseFromServer.status_code < 300)
	{
		ui.stackedWidget->setCurrentIndex(3);

		UpdateLobbiesDetails();

		TurnAutoExclusiveButtonsForCustomMode(false);

		SetCheckedButtonsForRoom(false);
	}
	else if (responseFromServer.status_code >= 500)
	{
		emit ServerCrashedSignalTriviador();
	}
}

void Triviador::on_readyGameRoomPushButton_released()
{
	ui.readyGameRoomPushButton->setDisabled(true);
	ui.backToRoomPushButton->setDisabled(true);

	std::string link = m_ip + "/increaseNumberOfReadyPlayers/?roomID=" + std::to_string(m_roomID);

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	if (responseFromServer.status_code >= 500)
	{
		emit ServerCrashedSignalTriviador();
	}
}

void Triviador::TimerMethodToUpdateRoomDetails()
{
	timerToUpdateRoomDetails->setInterval(500);
	timerToUpdateRoomDetails->setTimerType(Qt::PreciseTimer);

	timerToUpdateRoomDetails->start();

	if (ui.stackedWidget->currentIndex() == 3)
		UpdateLobbiesDetails();
	else if (ui.stackedWidget->currentIndex() == 4)
	{
		UpdateCurrentRoomPlayers();

		if (m_startingGame == GameState::InProgress)
		{
			StartTransferToGameTimer();
			
			m_startingGame = GameState::Started;
		}
		else
		{
			if (m_startingGame != GameState::Started)
			{
				if (CheckIfRoomIsReadyToBegin() == true)
				{
					m_startingGame = GameState::InProgress;

					m_isRoomReadyToBegin = true;
				}
			}
			else
			{
				if (CheckIfRoomIsReadyToBegin() == false)
				{
					std::string link = m_ip + "/resetNumberOfReadyPlayers/?roomID=" + std::to_string(m_roomID);

					cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

					if (responseFromServer.status_code >= 200 && responseFromServer.status_code < 300)
					{
						ui.readyGameRoomPushButton->setDisabled(false);
						ui.backToRoomPushButton->setDisabled(false);
					}

					m_isRoomReadyToBegin = false;

					m_startingGame = GameState::NotStarted;

					transferToGameTimer->stop();

					transferToGameTimer->disconnect();
					
					ui.progressBar->hide();
					ui.gameStartLabel->hide();
				}
			}
		}
	}

	if (timerToUpdateRoomDetails->remainingTimeAsDuration() <= std::chrono::milliseconds(1))
	{
		timerToUpdateRoomDetails->stop();

		timerToUpdateRoomDetails->disconnect();

		TimerMethodToUpdateRoomDetails();
	}
}

void Triviador::StartTransferToGameTimer()
{
	ui.progressBar->setValue(0);

	transferToGameTimer->setInterval(30);
	transferToGameTimer->setTimerType(Qt::PreciseTimer);

	connect(transferToGameTimer, SIGNAL(timeout()), this, SLOT(OnTransferToGameTimerTick()));

	transferToGameTimer->start();
}

void Triviador::OnTransferToGameTimerTick()
{
	ui.progressBar->show();
	ui.gameStartLabel->show();
		
	if (ui.progressBar->value() < 100)
		ui.progressBar->setValue(ui.progressBar->value() + 1);
	else
	{
		triviadorGame = new Game(m_ip, m_playerUsername, m_numberOfPlayers, m_numberOfRounds, m_mapHeight, m_mapWidth, m_roomID);

		ui.stackedWidget->insertWidget(5, triviadorGame);

		ui.stackedWidget->setCurrentIndex(5);

		transferToGameTimer->stop();

		transferToGameTimer->disconnect();
	}
}

void Triviador::UpdateLobbiesDetails()
{
	SetRoomDetails("2players");
	SetRoomDetails("3players");
	SetRoomDetails("4players");
	SetRoomDetails("customMode");
}

void Triviador::UpdateCurrentRoomPlayers()
{
	std::string link = m_ip + "/getRoomDetails/?roomID=" + std::to_string(m_roomID);

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	ui.playersListWidget->clear();

	if (responseFromServer.status_code >= 200 && responseFromServer.status_code < 300)
	{
		auto currentRoomDetails = crow::json::load(responseFromServer.text);

		int maximNumberOfPlayers = currentRoomDetails["maximNumberOfPlayers"].i();

		for (int playerIndex = 1; playerIndex <= maximNumberOfPlayers; playerIndex++)
		{
			std::string playerUsername = currentRoomDetails["player" + std::to_string(playerIndex)].s();

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

bool Triviador::CheckIfRoomIsReadyToBegin()
{
	std::string link = m_ip + "/isRoomReady/?roomID=" + std::to_string(m_roomID);

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	if (responseFromServer.status_code >= 200 && responseFromServer.status_code < 300)
	{
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

	return false;
}

void Triviador::SetRoomDetails(const std::string& roomType)
{
	std::string link = m_ip + "/getAvailableRoom/?gameType=" + roomType;

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	if (responseFromServer.status_code >= 200 && responseFromServer.status_code < 400)
	{
		auto availableRoom = crow::json::load(responseFromServer.text);

		if (roomType == "2players")
		{
			ui.currentNumberOfPlayers2Label->setText(std::to_string(availableRoom["currentNumberOfPlayers"].i()).c_str());
			ui.maximNumberOfPlayers2Label->setText("/ " + QString(std::to_string(availableRoom["maximNumberOfPlayers"].i()).c_str()));
		}

		if (roomType == "3players")
		{
			ui.currentNumberOfPlayers3Label->setText(std::to_string(availableRoom["currentNumberOfPlayers"].i()).c_str());
			ui.maximNumberOfPlayers3Label->setText("/ " + QString(std::to_string(availableRoom["maximNumberOfPlayers"].i()).c_str()));
		}

		if (roomType == "4players")
		{
			ui.currentNumberOfPlayers4Label->setText(std::to_string(availableRoom["currentNumberOfPlayers"].i()).c_str());
			ui.maximNumberOfPlayers4Label->setText("/ " + QString(std::to_string(availableRoom["maximNumberOfPlayers"].i()).c_str()));
		}

		if (roomType == "customMode")
		{
			ui.currentNumberOfPlayersCustomModeLabel->setText(std::to_string(availableRoom["currentNumberOfPlayers"].i()).c_str());
			ui.maximNumberOfPlayersCustomModeLabel->setText("/ " + QString(std::to_string(availableRoom["maximNumberOfPlayers"].i()).c_str()));
		}
	}
	else if (responseFromServer.status_code >= 400 && responseFromServer.status_code < 500)
	{
		CreateNewRoom(roomType);
	}
	else
	{
		emit ServerCrashedSignalTriviador();
	}
}

void Triviador::CreateNewRoom(const std::string& roomType)
{
	std::string link = m_ip + "/createNewRoom/?gameType=" + roomType;

	auto responseFromServer = cpr::Get(cpr::Url(link));

	if (responseFromServer.status_code >= 200 && responseFromServer.status_code < 400)
	{
		if (roomType == "2players")
		{
			ui.twoPlayersRoomLabel->setText("New room created!");
		}

		if (roomType == "3players")
		{
			ui.threePlayersRoomLabel->setText("New room created!");
		}

		if (roomType == "4players")
		{
			ui.fourPlayersRoomLabel->setText("New room created!");
		}

		if (roomType == "customMode")
		{
			ui.customModeRoomLabel->setText("New room created!");
		}

		SetRoomDetails(roomType);
	}
	else
	{
		emit ServerCrashedSignalTriviador();
	}
}