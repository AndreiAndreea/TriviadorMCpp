#include "Register.h"
#include "Login.h"

#include <regex>

Register::Register()
{
	ui.setupUi(this);

	ui.passwordLineEdit->setEchoMode(QLineEdit::Password);

	ui.emailErrorLabel->hide();

	ui.incompleteFieldsErrorLabel->hide();
	ui.submitDataErrorLabel->hide();

	ui.displayPasswordPushButton->setFlat(true);
	ui.displayPasswordPushButton->setStyleSheet("QPushButton { background-color: transparent; }");

	ui.displayPasswordPushButton->setIcon(QIcon("resources/img/icons/eye-off.png"));
}

Register::~Register()
{

}

std::string Register::GetCurrentDate()
{
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	std::ostringstream oss;
	oss << std::put_time(&tm, "%d/%m/%Y at %H:%M:%S");
	return oss.str();
}

const std::string Register::GetInputUsernameByPlayer() const
{
	return ui.usernameLineEdit->text().toStdString();
}

const std::string Register::GetInputPasswordByPlayer() const
{
	return ui.passwordLineEdit->text().toStdString();
}

const std::string Register::GetInputEmailByPlayer() const
{
	return ui.emailLineEdit->text().toStdString();
}

bool isValidEmail(const std::string& email)
{
	std::regex validEmailPattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");

	return regex_match(email, validEmailPattern);
}

void Register::on_displayPasswordPushButton_pressed()
{
	ui.displayPasswordPushButton->setIcon(QIcon("resources/img/icons/eye-on.png"));

	ui.passwordLineEdit->setEchoMode(QLineEdit::Normal);
}

void Register::on_displayPasswordPushButton_released()
{
	ui.displayPasswordPushButton->setIcon(QIcon("resources/img/icons/eye-off.png"));

	ui.passwordLineEdit->setEchoMode(QLineEdit::Password);
}

void Register::on_submitDataPushButton_released()
{
	ui.submitDataErrorLabel->hide();
	ui.incompleteFieldsErrorLabel->hide();


	if (GetInputUsernameByPlayer().empty() || GetInputPasswordByPlayer().empty() || GetInputEmailByPlayer().empty())
	{
		ui.incompleteFieldsErrorLabel->show();
	}
	else
	{
		ui.incompleteFieldsErrorLabel->hide();

		if (isValidEmail(GetInputEmailByPlayer()) == false)
			ui.emailErrorLabel->show();
		else
		{
			ui.emailErrorLabel->hide();
			
			auto storage = make_storage("triviador.sqlite",
				make_table(
					"Users",
					make_column("id", &Player::GetID, &Player::SetID, autoincrement(), primary_key()),
					make_column("username", &Player::GetUsername, &Player::SetUsername),
					make_column("password", &Player::GetPassword, &Player::SetUsername),
					make_column("email", &Player::GetEmail, &Player::SetEmail),
					make_column("accountCreationDate", &Player::GetAccountCreationDate, &Player::SetAccountCreationDate),
					make_column("score", &Player::GetTotalScore, &Player::SetTotalScore),
					make_column("playedGames", &Player::GetPlayedGames, &Player::SetPlayedGames),
					make_column("wonGames", &Player::GetWonGames, &Player::SetWonGames)
				));

			storage.sync_schema();

			auto players = storage.get_all<Player>(where(c(&Player::GetUsername) == GetInputUsernameByPlayer() || c(&Player::GetEmail) == GetInputEmailByPlayer()));

			if (players.size() != 0)
			{
				ui.submitDataErrorLabel->setText("Error: An account was found with the same username or email. Change the username or the email and try again.");

				ui.submitDataErrorLabel->show();
			}
			else
			{
				m_currentPlayer.SetUsername(GetInputUsernameByPlayer());
				m_currentPlayer.SetPassword(GetInputPasswordByPlayer());
				m_currentPlayer.SetEmail(GetInputEmailByPlayer());

				m_currentPlayer.SetAccountCreationDate(GetCurrentDate());

				storage.insert(m_currentPlayer);

				ui.submitDataErrorLabel->setText("Your account has been created successfully. You will be send to login page in a few moments.");
				ui.submitDataErrorLabel->show();

				//TO-DO 
				//Add a timer for a few seconds and after the timer is out, redirect the player to the login page.

				Login* loginWindow = new Login();
				loginWindow->show();
				this->close();
			}
		}
	}
}
