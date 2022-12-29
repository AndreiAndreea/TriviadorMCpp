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

	ui.changeUsernameLineEdit->hide();
	ui.changePasswordLineEdit->hide();
	ui.changeEmailLineEdit->hide();

	ui.userErrorLabel->hide();

	ui.changeUsernameMessageLabel->hide();
	ui.changePasswordMessageLabel->hide();
	ui.changeEmailMessageLabel->hide();

	ui.updateUserDetailsMessageLabel->hide();

	ui.saveProfileSettingsPushButton->setDisabled(true);

	ui.stackedWidget->setCurrentIndex(0);
}

Triviador::~Triviador()
{
	
}

void Triviador::on_playGamePushButton_released()
{
	triviadorGame = new Game(m_ip, m_playerUsername);

	ui.stackedWidget->insertWidget(3, triviadorGame);
	
	ui.stackedWidget->setCurrentIndex(3);
}

void Triviador::on_profilePushButton_released()
{
	updateUserDetails();
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

	updateUserDetails();
}

void Triviador::on_quitPushButton_released()
{
	std::string link = m_ip + "/logoutuser/?username=" + m_playerUsername;

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));

	close();
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
	else
	{
		std::string text = "Error: " + std::to_string(responseFromServer.status_code) + "\n" + responseFromServer.text;
		
		ui.updateUserDetailsMessageLabel->setText(text.c_str()); 
		ui.updateUserDetailsMessageLabel->show();
	}
}

void Triviador::updateUserDetails()
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
	else
	{
		ui.userErrorLabel->setText("Error: " + QString::fromStdString(std::to_string(responseFromServer.status_code)) + "\n" + QString::fromStdString(responseFromServer.text));
		ui.userErrorLabel->show();
	}
}
