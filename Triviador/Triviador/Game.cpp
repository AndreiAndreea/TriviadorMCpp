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

void Game::paintEvent(QPaintEvent*)
{    
	QPainter painter(this); 

    painter.setRenderHint(QPainter::Antialiasing, true);

    DrawMap(painter);

}

void Game::DrawMap(QPainter& painter)
{
    for (size_t i = 0; i < m_map.GetMapSize().first; i++)
    {
        for (size_t j = 0; j < m_map.GetMapSize().second; j++)
        {
            QRect square(300 + 50 * j, 70 + 50 * i, 50, 50);

            QPen pen;

            pen.setColor(Qt::black);

            painter.setPen(pen);
            painter.drawRect(square);
        }
    }

    update();
}

void Game::on_numberOfPlayersLineEdit_textChanged(const QString& arg1)
{
	/*QString test = QString("%1").arg(ui.numberOfPlayersLineEdit->text());
	ui.numberOfPlayersLabel->setText(test);*/

	m_map.SetNumberOfPlayers(GetNumberOfPlayers());

	m_map.CreateMap();

}