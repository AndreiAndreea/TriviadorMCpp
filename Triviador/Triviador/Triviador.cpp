#include "Triviador.h"

Triviador::Triviador(QWidget* parent)
	: QMainWindow(parent)
{

}

Triviador::Triviador(const std::string& ip, const std::string& playerUsername)
{
	m_ip = ip;
	m_playerUsername = playerUsername;

	ui.setupUi(this);

	ui.menuWidget->show();

	ui.profileWidget->hide();

	ui.profileSettingsWidget->hide();

	ui.changeUsernameLineEdit->hide();
	ui.changePasswordLineEdit->hide();
	ui.changeEmailLineEdit->hide();

	ui.updateUserDetailsMessageLabel->hide();
}

Triviador::~Triviador()
{
	
}

void Triviador::on_playGamePushButton_released()
{
	Game* triviaGame = new Game;
	triviaGame->show();

	close();
}

void Triviador::on_profilePushButton_released()
{
	ui.menuWidget->hide();
	
	ui.profileWidget->show();
}

void Triviador::on_backToMenuPushButton_released()
{
	ui.menuWidget->show();
	
	ui.profileWidget->hide();
}

void Triviador::on_profileSettingsPushButton_released()
{
	ui.profileWidget->hide();

	ui.profileSettingsWidget->show();
}

void Triviador::on_backToMenuFromProfileSettingsButton_released()
{
	ui.menuWidget->show();

	ui.profileSettingsWidget->hide();

	ui.changeUsernameLineEdit->hide();
	ui.changePasswordLineEdit->hide();
	ui.changeEmailLineEdit->hide();

	ui.updateUserDetailsMessageLabel->hide();
}

void Triviador::on_quitPushButton_released()
{
	close();
}

void Triviador::on_changeUsernamePushButton_released()
{
	ui.changeUsernameLineEdit->show();

	ui.updateUserDetailsMessageLabel->hide();

	std::string newUsername = ui.changeUsernameLineEdit->text().toLocal8Bit().constData();
}

void Triviador::on_changePasswordPushButton_released()
{
	ui.changePasswordLineEdit->show();

	ui.updateUserDetailsMessageLabel->hide();

	std::string newUsername = ui.changePasswordLineEdit->text().toLocal8Bit().constData();
}

void Triviador::on_changeEmailPushButton_released()
{
	ui.changeEmailLineEdit->show();

	ui.updateUserDetailsMessageLabel->hide();

	std::string newUsername = ui.changeEmailLineEdit->text().toLocal8Bit().constData();
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
	}
	else
	{
		std::string text = "Error: " + std::to_string(responseFromServer.status_code) + "\n" + responseFromServer.text;
		
		ui.updateUserDetailsMessageLabel->setText(text.c_str()); 
		ui.updateUserDetailsMessageLabel->show();
	}
	
}
