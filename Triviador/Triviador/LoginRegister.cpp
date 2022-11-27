#include "LoginRegister.h"

LoginRegister::LoginRegister()
{
	ui.setupUi(this);

	ui.greetingLabel->show();
	ui.signInLabel->hide();
	ui.singUpLabel->hide();

	ui.usernameLabel->hide();
	ui.usernameLineEdit->hide();
	ui.usernamePushButton->hide();
	ui.usernameErrorLabel->hide();

	ui.passwordLabel->hide();
	ui.passwordLineEdit->hide();
	ui.passwordLineEdit->setEchoMode(QLineEdit::Password);
	ui.passwordPushButton->hide();
	ui.displayPasswordPushButton->hide();

	ui.emailLabel->hide();
	ui.emailLineEdit->hide();
	ui.emailPushButton->hide();

	newUsernameIsValid = false;
	newPasswordIsValid = false;
	newEmailIsValid = false;
}

LoginRegister::~LoginRegister()
{

}

void LoginRegister::on_signInButton_released()
{
	if (ui.signInButton->isEnabled())
	{
		ui.greetingLabel->hide();
		ui.signInButton->hide();
		ui.signUpButton->hide();
		ui.signInLabel->show();

		ui.usernameLabel->show();
		ui.usernameLineEdit->show();
		ui.usernamePushButton->show();

		ui.passwordLabel->show();
		ui.passwordLineEdit->show();
		ui.passwordPushButton->show();
		ui.displayPasswordPushButton->show();

		ui.emailLabel->show();
		ui.emailLineEdit->show();
		ui.emailPushButton->show();
	}
}

void LoginRegister::on_signUpButton_released()
{
	if (ui.signUpButton->isEnabled())
	{
		ui.greetingLabel->hide();
		ui.signInButton->hide();
		ui.signUpButton->hide();
		ui.singUpLabel->show();
		
		ui.usernameLabel->show();
		ui.usernameLineEdit->show();
		ui.usernamePushButton->show();

		ui.passwordLabel->show();
		ui.passwordLineEdit->show();
		ui.passwordPushButton->show();
		ui.displayPasswordPushButton->show();

		ui.emailLabel->show();
		ui.emailLineEdit->show();
		ui.emailPushButton->show();
	}
}

void LoginRegister::on_usernamePushButton_released()
{
	ui.usernameErrorLabel->hide();

	if (ui.usernamePushButton->isEnabled())
	{
		QString text = ui.usernameLineEdit->text();
		std::string username = text.toStdString();

		//check if the username is unique
		bool isUnique = true;

		for (const auto& player : m_players)
			if (username == player.GetUsername())
				isUnique = false;

		if (isUnique)
		{
			m_currentPlayer.SetUsername(username);
			newUsernameIsValid = true;
		}
		else
		{
			ui.usernameErrorLabel->show();
		}
	}
}

void LoginRegister::on_passwordPushButton_released()
{
	if (ui.passwordPushButton->isEnabled())
	{
		QString text = ui.passwordLineEdit->text();
		std::string password = text.toStdString();

		m_currentPlayer.SetPassword(password);
		newPasswordIsValid = true;
	}
}

void LoginRegister::on_emailPushButton_released()
{
	if (ui.emailPushButton->isEnabled())
	{
		QString text = ui.emailLineEdit->text();
		std::string email = text.toStdString();

		m_currentPlayer.SetEmail(email);
		newEmailIsValid = true;
	}
}

void LoginRegister::on_displayPasswordPushButton_released()
{
	if (ui.displayPasswordPushButton->isEnabled())
	{
		ui.passwordLineEdit->setEchoMode(QLineEdit::Normal);
	}
}
