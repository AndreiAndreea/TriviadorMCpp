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
	ui.emailErrorLabel->hide();

	ui.submitDataPushButton->hide();
	ui.submitDataErrorLabel->hide();
	ui.incompleteFieldsErrorLabel->hide();
	ui.invalidDataErrorLabel->hide();
	
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

		ui.submitDataPushButton->show();
	}
}

std::string LoginRegister::GetCurrentDate()
{
	std::time_t t = std::time(nullptr);
	std::tm* now = std::localtime(&t);
	std::string currentDate;

	currentDate += std::to_string((now->tm_year + 1900));
	currentDate += "-";

	currentDate += std::to_string((now->tm_mon + 1));
	currentDate += "-";

	currentDate += std::to_string(now->tm_mday);

	return currentDate;
}

std::vector<Player> LoginRegister::GetPlayers()
{
	return m_players;
}

void LoginRegister::showPlayersInFile()
{
	QString filename = "allPlayersOutput.txt";
	QFile file(filename);
	if (file.open(QIODevice::ReadWrite))
	{
		for (const auto& player : m_players)
		{
			QTextStream stream(&file);
			stream << "Player:\n";
			stream << QString::fromStdString(player.GetUsername()) << "\n";
			stream << QString::fromStdString(player.GetPassword()) << "\n";
			stream << QString::fromStdString(player.GetEmail()) << "\n";
			stream << QString::fromStdString(player.GetAccountCreationDate()) << "\n";
		}
	}
	file.close();
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

		ui.submitDataPushButton->show();
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
		isUniqueUsername = true;

		for (const auto& player : m_players)
			if (username == player.GetUsername())
				isUniqueUsername = false;

		if (isUniqueUsername)
		{
			m_currentPlayer.SetUsername(username);
			newUsernameIsValid = true;
		}
		else
		{
			ui.usernameErrorLabel->show();
			newUsernameIsValid = false;
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

		//check if the email is unique
		bool isUniqueEmail = true;

		for (const auto& player : m_players)
			if (email == player.GetEmail())
				bool isUniqueEmail = false;

		if (isUniqueEmail)
		{
			m_currentPlayer.SetEmail(email);
			newEmailIsValid = true;
		}
		else
		{
			ui.emailErrorLabel->show();
			newEmailIsValid = false;
		}
	}
}

void LoginRegister::on_displayPasswordPushButton_released()
{
	if (ui.displayPasswordPushButton->isEnabled())
	{
		ui.passwordLineEdit->setEchoMode(QLineEdit::Normal);
	}
}

void LoginRegister::on_submitDataPushButton_released()
{
	ui.submitDataErrorLabel->hide();
	ui.incompleteFieldsErrorLabel->hide();
	ui.invalidDataErrorLabel->hide();

	if (ui.submitDataPushButton->isEnabled())
	{
		if (newUsernameIsValid && newPasswordIsValid && newEmailIsValid)
		{
			m_currentPlayer.SetAccountCreationDate(GetCurrentDate());
			m_players.push_back(m_currentPlayer);

			QString filename = "playerOutput.txt";
			QFile file(filename);
			if (file.open(QIODevice::ReadWrite)) 
			{
				QTextStream stream(&file);
				stream << "NewPlayer:\n";
				stream << QString::fromStdString(m_currentPlayer.GetUsername()) << "\n";
				stream << QString::fromStdString(m_currentPlayer.GetPassword()) << "\n";
				stream << QString::fromStdString(m_currentPlayer.GetEmail()) << "\n";
				stream << QString::fromStdString(m_currentPlayer.GetAccountCreationDate()) << "\n";
			}
			file.close();
			showPlayersInFile();
		}
		else if(!isUniqueUsername && (!newUsernameIsValid || !newPasswordIsValid || !newEmailIsValid))
		{
			ui.submitDataErrorLabel->show();
			ui.incompleteFieldsErrorLabel->show();
			ui.invalidDataErrorLabel->show();
		}
		else if(!isUniqueUsername)
		{
			ui.submitDataErrorLabel->show();
			ui.invalidDataErrorLabel->show();
		}
		else
		{
			ui.submitDataErrorLabel->show();
			ui.incompleteFieldsErrorLabel->show();
		}
	}
}
