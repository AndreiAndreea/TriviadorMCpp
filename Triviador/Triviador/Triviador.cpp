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

void Triviador::on_playGamePushButton_released()
{
	ui.stackedWidget->setCurrentIndex(3);

	if (ui.customModePushButton->isChecked())
		HideCustomModeSettings();

	UpdateLobbiesDetails();

	TurnAutoExclusiveButtonsForCustomMode(false);

	SetCheckedButtonsForLobby(false);

	ui.joinLobbyPushButton->hide();
}

void Triviador::on_profilePushButton_released()
{
	UpdateUserDetails();
}

void Triviador::on_backToMenuPushButton_released()
{
	ui.stackedWidget->setCurrentIndex(0);
}

void Triviador::on_profileSettingsPushButton_released()
{
	ui.stackedWidget->setCurrentIndex(2);
}

void Triviador::on_backToProfileButton_released()
{
	ui.stackedWidget->setCurrentIndex(1);

	UpdateUserDetails();
}

void Triviador::on_backToMenuPushButton_2_released()
{
	ui.stackedWidget->setCurrentIndex(0);
}

void Triviador::on_quitPushButton_clicked()
{
	std::string link = m_ip + "/logoutuser/?username=" + m_playerUsername;

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	emit BackToLoginSignal();
}

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

	ui.changeEmailMessageLabel->show();
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

void Triviador::on_twoPlayersPushButton_released()
{
	if (ui.twoPlayersPushButton->autoExclusive() == false)
		TurnAutoExclusiveButtonsForCustomMode(true);

	if(ui.playersSpinBox->isHidden() == false)
		HideCustomModeSettings();

	ui.joinLobbyPushButton->show();
}

void Triviador::on_threePlayersPushButton_released()
{
	if (ui.twoPlayersPushButton->autoExclusive() == false)
		TurnAutoExclusiveButtonsForCustomMode(true);
	
	if (ui.playersSpinBox->isHidden() == false)
		HideCustomModeSettings();

	ui.joinLobbyPushButton->show();
}

void Triviador::on_fourPlayersPushButton_released()
{
	if (ui.twoPlayersPushButton->autoExclusive() == false)
		TurnAutoExclusiveButtonsForCustomMode(true);
	
	if (ui.playersSpinBox->isHidden() == false)
		HideCustomModeSettings();

	ui.joinLobbyPushButton->show();
}

void Triviador::on_customModePushButton_released()
{
	if (ui.twoPlayersPushButton->autoExclusive() == false)
		TurnAutoExclusiveButtonsForCustomMode(true);
	
	ShowCustomModeSettings();

	ui.joinLobbyPushButton->show();
}

void Triviador::on_playersSpinBox_valueChanged(int arg1)
{
	ui.roundsSpinBox->setMinimum(arg1 - 1);
	ui.roundsSpinBox->setMaximum(arg1 * 3 / 2);
	ui.roundsSpinBox->setValue(arg1 + 2);

	ui.mapWidthSpinBox->setMinimum(arg1 - 1);
	ui.mapWidthSpinBox->setMaximum(arg1 * 2);
	ui.mapWidthSpinBox->setValue(arg1 + 3);

	ui.mapHeightSpinBox->setMinimum(arg1 - 1);
	ui.mapHeightSpinBox->setMaximum(arg1 * 2);
	ui.mapHeightSpinBox->setValue(arg1 + 1);
}

void Triviador::on_joinLobbyPushButton_released()
{
	//get the number of players & rounds & map size
	
	ui.stackedWidget->setCurrentIndex(4);
}

void Triviador::on_backToLobbyPushButton_released()
{
	//clear the lobby settings
	TurnAutoExclusiveButtonsForCustomMode(false);

	SetCheckedButtonsForLobby(false);
	
	ui.stackedWidget->setCurrentIndex(3);
}

void Triviador::on_readyGameLobbyPushButton_released()
{
	ui.readyGameLobbyPushButton->setDisabled(true);
	ui.backToLobbyPushButton->setDisabled(true);

	ui.startGameLobbyPushButton->show();
}

void Triviador::on_startGameLobbyPushButton_released()
{	
	triviadorGame = new Game(m_ip, m_playerUsername);

	ui.stackedWidget->insertWidget(4, triviadorGame);
	
	ui.stackedWidget->setCurrentIndex(4);
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

void Triviador::UpdateLobbiesDetails()
{
	GetLobbyDetails("2players");
	GetLobbyDetails("3players");
	GetLobbyDetails("4players");
	GetLobbyDetails("customMode");
}

void Triviador::GetLobbyDetails(const std::string& lobbyType)
{	
	std::string link = m_ip + "/getAvailableLobby/?gameType=" + lobbyType;

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	if (responseFromServer.status_code >= 200 && responseFromServer.status_code < 400)
	{
		auto db_lobbies = crow::json::load(responseFromServer.text);

		if (lobbyType == "2players")
		{
			ui.currentNumberOfPlayers2Label->setText(std::to_string(db_lobbies["currentNumberOfPlayers"].i()).c_str());
			ui.maximNumberOfPlayers2Label->setText("/ " + QString(std::to_string(db_lobbies["maximNumberOfPlayers"].i()).c_str()));
		}
		
		if (lobbyType == "3players")
		{
			ui.currentNumberOfPlayers3Label->setText(std::to_string(db_lobbies["currentNumberOfPlayers"].i()).c_str());
			ui.maximNumberOfPlayers3Label->setText("/ " + QString(std::to_string(db_lobbies["maximNumberOfPlayers"].i()).c_str()));
		}
		
		if (lobbyType == "4players")
		{
			ui.currentNumberOfPlayers4Label->setText(std::to_string(db_lobbies["currentNumberOfPlayers"].i()).c_str());
			ui.maximNumberOfPlayers4Label->setText("/ " + QString(std::to_string(db_lobbies["maximNumberOfPlayers"].i()).c_str()));
		}
		
		if (lobbyType == "customMode")
		{
			ui.currentNumberOfPlayersCustomModeLabel->setText(std::to_string(db_lobbies["currentNumberOfPlayers"].i()).c_str());
			ui.maximNumberOfPlayersCustomModeLabel->setText("/ " + QString(std::to_string(db_lobbies["maximNumberOfPlayers"].i()).c_str()));
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

		GetLobbyDetails(lobbyType);
	}
	else
	{
		emit ServerCrashedSignalTriviador();
	}
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
