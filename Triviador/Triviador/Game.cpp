#include "Game.h"

Game::Game()
{
}

Game::Game(const std::string& ip, const std::string& username, const uint16_t numberOfPlayers, const uint16_t numberOfRounds, const uint16_t mapHeight, const uint16_t mapWidth, int roomID)
{
	ui.setupUi(this);

	m_ip = ip;
	m_playerUsername = username;
	m_roomID = roomID;

	m_selectedRegions.clear();

	QuestionsWindow = new GameElementsGenerator(m_ip, m_playerUsername, m_roomID);
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

	ExtractPlayerColor();

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

bool Game::CheckIfRegionsAreNeighbors(const QPointF& coordRegion1, const QPointF& coordRegion2)
{
	QRect region1(coordRegion1.x(), coordRegion1.y(), 50, 50);
	QRect region2(coordRegion2.x(), coordRegion2.y(), 50, 50);
	
	if (region1.intersects(region2))
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

void Game::ExtractPlayerColor()
{	
	std::string link = m_ip + "/getPlayerColor/?roomID=" + std::to_string(m_roomID) + "&username=" + m_playerUsername;

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));
	
	if (responseFromServer.status_code == 200)
	{
		auto color = crow::json::load(responseFromServer.text);
		std::string usedColor = color["color"].s();
		m_usedColor = QColor(usedColor.c_str());
	}
}

void Game::paintEvent(QPaintEvent*)
{
	if (ui.stackedWidget->currentIndex() == 0)
	{
		QPainter painter(this);

		painter.setRenderHint(QPainter::Antialiasing, true);

		DrawMap(painter);
		
		auto base = m_map.GetBaseRegion(m_playerUsername);
		QRect square(base.first, base.second, 50, 50);
		painter.fillRect(square, m_usedColor);
		
		for (const auto& region : m_map.GetUsedRegions())
		{
			if (region.second.first == m_playerUsername)
			{
				QRect square(region.first.first, region.first.second, 50, 50);
				painter.fillRect(square, m_usedColor);
			}
		}

		if (ui.stackedWidget->currentIndex() == 1)
			painter.setOpacity(0.5);
	}
}

void Game::DrawMap(QPainter& painter)
{
	for (size_t i = 0; i < m_map.GetMapSize().first; i++)
	{
		for (size_t j = 0; j < m_map.GetMapSize().second; j++)
		{
			QRect square(600 + 50 * j, 170 + 50 * i, 50, 50);

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
						ui.existingRegionLabel->setText("Regiunea este deja selectata!");
					}
					else
					{
						if (QuestionsWindow->GetIsBaseSelectionStageActive())
						{
							auto coords = std::make_pair(regionCoordinates.x(), regionCoordinates.y());
							m_map.AddBaseRegion(coords, m_playerUsername);
							
							//etapa impartirii bazelor se incheie cand fiecare player a selectat o baza
							if (m_map.GetNumberOfChosenBases() == m_map.GetNumberOfPlayers())
							{
								QuestionsWindow->SetIsBaseSelectionStageActive(false);
								//daca region selection stage e activ
								//atunci va trebui sa chemam tot o intrebare de tip numeric de pe server
								QuestionsWindow->SetIsRegionSelectionStageActive(true);
							}
						}
						if (QuestionsWindow->GetIsRegionSelectionStageActive())
						{
							auto coords = std::make_pair(regionCoordinates.x(), regionCoordinates.y());
							m_map.AddRegion(coords, m_playerUsername, 100);
							
							//etapa impartirii regiunilor se incheie cand toate regiunile hartii sunt atribuite
							if (m_selectedRegions.size() == m_map.GetMapSize().first * m_map.GetMapSize().second)
							{
								QuestionsWindow->SetIsRegionSelectionStageActive(false);
								QuestionsWindow->SetIsDuelStageActive(true);
							}
						}
						
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
	
		QuestionsWindow->TimerMethodToRequestDataFromServer(100);
	}
}

void Game::closeEvent(QCloseEvent* e)
{
	std::string link = m_ip + "/logoutuser/?username=" + m_playerUsername;

	cpr::Response responseFromServer = cpr::Get(cpr::Url(link));
}