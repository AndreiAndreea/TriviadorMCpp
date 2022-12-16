#include "Triviador.h"

Triviador::Triviador(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	ui.menuWidget->show();

	ui.profileWidget->hide();

	ui.profileSettingsWidget->hide();

	ui.changeUsernameLineEdit->hide();
	ui.changePasswordLineEdit->hide();
	ui.changeEmailLineEdit->hide();
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

void Triviador::on_quitPushButton_released()
{
	close();
}

void Triviador::on_changeUsernamePushButton_released()
{
	ui.changeUsernameLineEdit->show();

	std::string newUsername = ui.changeUsernameLineEdit->text().toLocal8Bit().constData();
}

void Triviador::on_changePasswordPushButton_released()
{
	ui.changePasswordLineEdit->show();

	std::string newUsername = ui.changePasswordLineEdit->text().toLocal8Bit().constData();
}

void Triviador::on_changeEmailPushButton_released()
{
	ui.changeEmailLineEdit->show();

	std::string newUsername = ui.changeEmailLineEdit->text().toLocal8Bit().constData();
}
