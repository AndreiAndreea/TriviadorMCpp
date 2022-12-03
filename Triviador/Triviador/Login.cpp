#include "Login.h"
#include "Triviador.h"

#include <sqlite_orm/sqlite_orm.h>

using namespace sqlite_orm;

Login::Login()
{
	ui.setupUi(this);

	ui.passwordLineEdit->setEchoMode(QLineEdit::Password);

	ui.displayPasswordPushButton->setFlat(true);
	ui.displayPasswordPushButton->setStyleSheet("QPushButton { background-color: transparent; }");
	
	ui.displayPasswordPushButton->setIcon(QIcon("resources/img/icons/eye-off.png"));

	ui.loginErrorLabel->hide();
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
	auto storage = make_storage("triviador.sqlite",
		make_table(
			"Users",
			make_column("id", &Player::GetID, &Player::SetID, autoincrement(), primary_key()),
			make_column("username", &Player::GetUsername, &Player::SetUsername),
			make_column("password", &Player::GetPassword, &Player::SetPassword),
			make_column("email", &Player::GetEmail, &Player::SetEmail),
			make_column("accountCreationDate", &Player::GetAccountCreationDate, &Player::SetAccountCreationDate),
			make_column("score", &Player::GetTotalScore, &Player::SetTotalScore),
			make_column("playedGames", &Player::GetPlayedGames, &Player::SetPlayedGames),
			make_column("wonGames", &Player::GetWonGames, &Player::SetWonGames)
		));

	storage.sync_schema();

	auto player = storage.get_all<Player>(where(c(&Player::GetUsername) == GetUsername() && c(&Player::GetUsername) == GetPassword()));

	if (player.size() == 1)
	{
		Triviador* t = new Triviador;
		t->show();
	
		this->close();
	}
	else if (player.size() == 0)
	{
		ui.loginErrorLabel->setText("Username or password incorrect!");
		ui.loginErrorLabel->show();
	}
}