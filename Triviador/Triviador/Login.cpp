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
			make_column("id", &User::id, autoincrement(), primary_key()),
			make_column("username", &User::username),
			make_column("password", &User::password),
			make_column("email", &User::email),
			make_column("accountCreationDate", &User::accountCreationDate),
			make_column("score", &User::score),
			make_column("playedGames", &User::playedGames),
			make_column("wonGames", &User::wonGames)
		));

	storage.sync_schema();

	auto player = storage.get_all<User>(where(c(&User::username) == GetUsername() && c(&User::password) == GetPassword()));

	if (player.size() == 1)
	{
		isCorrectUsernameAndPassword = true;

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