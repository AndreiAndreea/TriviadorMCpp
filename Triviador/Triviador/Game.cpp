#include "Game.h"

Game::Game()
{
	ui.setupUi(this);
	w.show();
	ui.existingRegionLabel->hide();
	ui.mapSizeErrorLabel->hide();
	
	ui.playersSpinBox->setRange(2, 9);
	ui.roundsSpinBox->setRange(2, 25);
	ui.mapHeightSpinBox->setRange(3, 25);
	ui.mapWidthSpinBox->setRange(3, 25);

	ui.playersSpinBox->hide();
	ui.roundsSpinBox->hide();
	ui.mapHeightSpinBox->hide();
	ui.mapWidthSpinBox->hide();
	
	ui.playersLabel->hide();
	ui.roundsLabel->hide();
	ui.mapHeightLabel->hide();
	ui.mapWidthLabel->hide();

	ui.finishGameModeSetupPushButton->hide();

	gameModeIsSet = false;

	GenerateRandomColor();
}

Game::~Game()
{
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


void Game::GenerateRandomColor()
{
	srand(time(0));

	int randomColorIndex = rand() % m_colorList.size();

	m_usedColor = m_colorList[randomColorIndex];
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
		painter.fillRect(square, m_usedColor);
	}
}

void Game::DrawMap(QPainter& painter)
{
	if (gameModeIsSet == true)
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
}

void Game::mouseReleaseEvent(QMouseEvent* ev)
{
	if (ev->button() == Qt::LeftButton && w.GetCanChooseTerritory())
	{
		int found = 0;
		QPointF clickPosition = ev->position();

		for (size_t i = 0; i < m_map.GetMapSize().first; i++)
		{
			for (size_t j = 0; j < m_map.GetMapSize().second; j++)
			{
				QPointF regionCoordinates(300 + 50 * j, 70 + 50 * i);

				if (ClickedOnRegion(clickPosition, regionCoordinates))
				{
					for (const auto region : m_selectedRegions)
						if (region == regionCoordinates)
							found = 1;
					if (found)
					{
						ui.existingRegionLabel->show();
						ui.existingRegionLabel->setText("Regiunea deja exista!");
					}
					else
					{
						AddNewSelectedRegion(regionCoordinates);
						ui.existingRegionLabel->hide();
						w.SetCanChooseTerritory(false);
					}
				}

			}
		}
	}
}

QString Game::CheckMapSize()
{
	uint16_t mapHeight = m_map.GetMapSize().first;
	uint16_t mapWidth = m_map.GetMapSize().second;
	uint16_t limit = m_map.GetNumberOfPlayers() + 1;

	if (mapHeight < limit || mapWidth < limit)
		return "Map dimensions have to be at least the number of players + 1 each!";
	return "";
}

void Game::on_twoPlayersPushButton_released()
{
	ui.threePlayersPushButton->setDisabled(true);
	ui.fourPlayersPushButton->setDisabled(true);
	ui.customModePushButton->setDisabled(true);

	m_map.SetNumberOfPlayers(2);
	w.SetNumberOfPlayers(2);

	gameModeIsSet = true;

	m_map.CreateMap();

	//when a new map is created, we need to clear the colored regions of the previous map
	m_selectedRegions.clear();
}

void Game::on_threePlayersPushButton_released()
{
	ui.twoPlayersPushButton->setDisabled(true);
	ui.fourPlayersPushButton->setDisabled(true);
	ui.customModePushButton->setDisabled(true);
	
	m_map.SetNumberOfPlayers(3);
	w.SetNumberOfPlayers(3);

	gameModeIsSet = true;

	m_map.CreateMap();

	//when a new map is created, we need to clear the colored regions of the previous map
	m_selectedRegions.clear();
}

void Game::on_fourPlayersPushButton_released()
{
	ui.twoPlayersPushButton->setDisabled(true);
	ui.threePlayersPushButton->setDisabled(true);
	ui.customModePushButton->setDisabled(true);
	
	m_map.SetNumberOfPlayers(4);
	w.SetNumberOfPlayers(4);

	gameModeIsSet = true;

	m_map.CreateMap();

	//when a new map is created, we need to clear the colored regions of the previous map
	m_selectedRegions.clear();
}

void Game::on_customModePushButton_released()
{
	ui.twoPlayersPushButton->setDisabled(true);
	ui.threePlayersPushButton->setDisabled(true);
	ui.fourPlayersPushButton->setDisabled(true);

	ui.playersSpinBox->show();
	ui.roundsSpinBox->show();
	ui.mapHeightSpinBox->show();
	ui.mapWidthSpinBox->show();

	ui.playersLabel->show();
	ui.roundsLabel->show();
	ui.mapHeightLabel->show();
	ui.mapWidthLabel->show();

	ui.finishGameModeSetupPushButton->show();

	w.SetNumberOfPlayers(ui.playersSpinBox->value());

	m_map.SetNumberOfPlayers(ui.playersSpinBox->value());	
	m_map.SetNumberOfRounds(ui.roundsSpinBox->value());
}

void Game::on_finishGameModeSetupPushButton_released()
{
	uint16_t mapHeight = ui.mapHeightSpinBox->value();
	uint16_t mapWidth = ui.mapWidthSpinBox->value();
	m_map.SetMapSize(mapHeight, mapWidth);

	QString mapSizeErrorText = CheckMapSize();
	ui.mapSizeErrorLabel->setText(mapSizeErrorText);
	ui.mapSizeErrorLabel->show();

	if (mapSizeErrorText == "")
	{
		gameModeIsSet = true;
		m_map.CreateMap();
	}
}
