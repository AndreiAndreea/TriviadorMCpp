#include "Login.h"
#include "Triviador.h"

#include <sqlite_orm/sqlite_orm.h>

using namespace sqlite_orm;

Login::Login()
{
	ui.setupUi(this);
}

Login::~Login()
{
}

const std::string& Login::GetUsername() const
{
	return ui.usernameLineEdit->text().toStdString();
}

const std::string& Login::GetPassword() const
{
	return ui.passwordLineEdit->text().toStdString();
}

void Login::on_loginPushButton_released()
{
	if (ui.loginPushButton->isEnabled())
	{
		//check if the username and password are correct
		isCorrectUsernameAndPassword = true;

		auto storage = make_storage("triviador.sqlite",
			make_table(
				"Users",
				make_column("id", &UserPlayer::id, autoincrement(), primary_key()),
				make_column("username", &UserPlayer::username),
				make_column("password", &UserPlayer::password),
				make_column("email", &UserPlayer::email),
				make_column("accountCreationDate", &UserPlayer::accountCreationDate),
				make_column("score", &UserPlayer::score),
				make_column("playedGames", &UserPlayer::playedGames),
				make_column("wonGames", &UserPlayer::wonGames)
			));

		storage.sync_schema();

		auto players = storage.get_all<UserPlayer>();

		for (const auto& player : players)
			if (GetUsername() != player.username || GetPassword() != player.password)
				isCorrectUsernameAndPassword = false;

		if (isCorrectUsernameAndPassword)
		{
			Triviador* t= new Triviador;
			t->show();
			this->close();
		}
		else
		{
			ui.loginErrorLabel->show();
		}
	}
}