#include "Game.h"

Game::Game()
{
	ui.setupUi(this);
    w.show();
}

Game::~Game()
{
}

uint16_t Game::GetNumberOfPlayers()
{
	QString numberOfPlayers = ui.numberOfPlayersLineEdit->text();
	return static_cast<uint16_t>(numberOfPlayers.toInt());
}

bool Game::ClickedOnRegion(const QPointF& coordClick, const QPointF& coordRegion)
{
    if ((coordClick.x() > coordRegion.x()) && (coordClick.x() < coordRegion.x() + 50) &&
        (coordClick.y() > coordRegion.y()) && (coordClick.y() < coordRegion.y() + 50))
        return true;
    return false;
}

void Game::AddNewSelectedRegion(const QPointF& coordPos)
{
    m_selectedRegions.push_back(coordPos);
}


void Game::paintEvent(QPaintEvent*)
{    
	QPainter painter(this); 

    painter.setRenderHint(QPainter::Antialiasing, true);

    DrawMap(painter);

    //coloring the selected regions of the map
    for (const auto& coordRegion : m_selectedRegions)
    {
        QRect square(coordRegion.x(), coordRegion.y(), 50, 50);
        painter.fillRect(square, Qt::cyan);
    }
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

void Game::mouseReleaseEvent(QMouseEvent* ev)
{
    if (ev->button() == Qt::LeftButton && w.GetCanChooseTerritory())
    {
        QPointF clickPosition = ev->position();

        for (size_t i = 0; i < m_map.GetMapSize().first; i++)
        {
            for (size_t j = 0; j < m_map.GetMapSize().second; j++)
            {
                QPointF regionCoordinates(300 + 50 * j, 70 + 50 * i);

                if (ClickedOnRegion(clickPosition, regionCoordinates))
                    AddNewSelectedRegion(regionCoordinates);
            }
        }
    }
    w.SetCanChooseTerritory(false);
}

void Game::on_numberOfPlayersLineEdit_textChanged(const QString& arg1)
{
	/*QString test = QString("%1").arg(ui.numberOfPlayersLineEdit->text());
	ui.numberOfPlayersLabel->setText(test);*/

	m_map.SetNumberOfPlayers(GetNumberOfPlayers());
    w.SetNumberOfPlayers(GetNumberOfPlayers());

	m_map.CreateMap();

    //when a new map is created, we need to clear the colored regions of the previous map
    m_selectedRegions.clear();
}