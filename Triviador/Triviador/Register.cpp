#include "Register.h"
#include "Login.h"
#include <regex>

Register::Register()
{
	ui.setupUi(this);
	
	ui.passwordLineEdit->setEchoMode(QLineEdit::Password);

	ui.usernameErrorLabel->hide();
	ui.emailErrorLabel->hide();
	ui.incompleteFieldsErrorLabel->hide();
	ui.invalidDataErrorLabel->hide();
	ui.submitDataErrorLabel->hide();

	newUsernameIsValid = false;
	newPasswordIsValid = false;
	newEmailIsValid = false;
}

Register::~Register()
{

}


std::string Register::GetCurrentDate()
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

bool CheckEmail(const std::string& email)
{
	std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
	return regex_match(email, pattern);
}

std::vector<Player> Register::GetPlayers()
{
	return m_players;
}

void Register::showPlayersInFile()
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

void Register::on_usernamePushButton_released()
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

void Register::on_passwordPushButton_released()
{
	if (ui.passwordPushButton->isEnabled())
	{
		QString text = ui.passwordLineEdit->text();
		std::string password = text.toStdString();

		m_currentPlayer.SetPassword(password);
		newPasswordIsValid = true;
	}
}

void Register::on_emailPushButton_released()
{
	if (ui.emailPushButton->isEnabled())
	{
		QString text = ui.emailLineEdit->text();
		std::string email = text.toStdString();

		//check email valid format
		if (CheckEmail(email))
		{
			//check if the email is unique
			bool isUniqueEmail = true;

			for (const auto& player : m_players)
				if (email == player.GetEmail())
					bool isUniqueEmail = false;

			if (isUniqueEmail)
			{
				ui.emailErrorLabel->hide();
				m_currentPlayer.SetEmail(email);
				newEmailIsValid = true;
			}
			else
			{
				ui.emailErrorLabel->show();
				newEmailIsValid = false;
			}
		}
		else
		{
			ui.emailErrorLabel->show();
			newEmailIsValid = false;
		}

	}
}

void Register::on_displayPasswordPushButton_released()
{
	if (ui.displayPasswordPushButton->isEnabled())
	{
		ui.passwordLineEdit->setEchoMode(QLineEdit::Normal);
	}
}

void Register::on_submitDataPushButton_released()
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

			Login* loginWindow = new Login();
			loginWindow->show();
			this->close();
		}
		else if (!isUniqueUsername && (!newUsernameIsValid || !newPasswordIsValid || !newEmailIsValid))
		{
			ui.submitDataErrorLabel->show();
			ui.incompleteFieldsErrorLabel->show();
			ui.invalidDataErrorLabel->show();
		}
		else if (!isUniqueUsername)
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
