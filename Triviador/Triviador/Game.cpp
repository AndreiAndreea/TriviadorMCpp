#include "Game.h"

Game::Game()
{
}

Game::Game(const std::string& ip, const std::string& username, const uint16_t numberOfPlayers, const uint16_t numberOfRounds, const uint16_t mapHeight, const uint16_t mapWidth)
{
	ui.setupUi(this);
	
	m_ip = ip;
	m_playerUsername = username;

	m_selectedRegions.clear();

	QuestionsWindow = new GameElementsGenerator(m_ip, m_playerUsername);
	ui.stackedWidget->insertWidget(1, QuestionsWindow);

	QuestionsWindow->SetNumberOfPlayers(numberOfPlayers);

	if (numberOfRounds == 0 && mapHeight == 0 && mapWidth == 0)
	{
		m_map.SetNumberOfPlayers(numberOfPlayers);

		m_map.CreateMap();
	}
	else
	{
		m_map.CreateMapCustomMode(mapHeight, mapWidth, numberOfPlayers, numberOfRounds);
	}
	
	ui.existingRegionLabel->hide();

	ui.progressBar->show();
	ui.progressBarLabel->show();

	GenerateRandomColor();

	StartInitializeQuestionsGeneratorTimer(); 

	ui.stackedWidget->setCurrentIndex(0);
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

void Game::StartInitializeQuestionsGeneratorTimer()
{
	initializeQuestionsGeneratorTimer = new QTimer(this);

	ui.progressBar->setValue(0);
	initializeQuestionsGeneratorTimer->setInterval(50);

	initializeQuestionsGeneratorTimer->setTimerType(Qt::PreciseTimer);

	connect(initializeQuestionsGeneratorTimer, SIGNAL(timeout()), this, SLOT(OnIinitializeQuestionsGeneratorTimerTick()));

	initializeQuestionsGeneratorTimer->start();
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

	if (ui.stackedWidget->currentIndex() == 1)
		painter.setOpacity(0.5);
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
	if (ev->button() == Qt::LeftButton && QuestionsWindow->GetCanChooseTerritory())
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
						QuestionsWindow->SetCanChooseTerritory(false);
					}
				}
			}
		}
	}
}

void Game::OnIinitializeQuestionsGeneratorTimerTick()
{
	ui.progressBar->setValue(ui.progressBar->value() + 1);

	if (ui.progressBar->value() >= 100)
	{
		ui.progressBar->hide();
		ui.progressBarLabel->hide();

		initializeQuestionsGeneratorTimer->disconnect();
		
		ui.stackedWidget->setCurrentIndex(1);
	}

}

void Game::closeEvent(QCloseEvent* e)
{
	std::string link = m_ip + "/logoutuser/?username=" + m_playerUsername;

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));
}