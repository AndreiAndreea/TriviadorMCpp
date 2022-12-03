#include "Game.h"

Game::Game()
{
	ui.setupUi(this);
}

Game::~Game()
{
}

uint16_t Game::GetNumberOfPlayers()
{
	QString numberOfPlayers = ui.numberOfPlayersLineEdit->text();

	return static_cast<uint16_t>(numberOfPlayers.toInt());
}

void Game::on_numberOfPlayersLineEdit_textChanged(const QString& arg1)
{
	/*QString test = QString("%1").arg(ui.numberOfPlayersLineEdit->text());
	ui.numberOfPlayersLabel->setText(test);*/

	m_map.SetNumberOfPlayers(GetNumberOfPlayers());

	m_map.CreateMap();

}