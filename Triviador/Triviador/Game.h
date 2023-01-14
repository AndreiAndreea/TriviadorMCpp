#pragma once

#include <QDialog>

#include "ui_Game.h"

#include "Map.h"
#include "GameElementsGenerator.h"

class Game : public QDialog
{
	Q_OBJECT

private:
	Ui::Game ui;

public:
	explicit Game();
	
	Game(const std::string& ip, const std::string& username, const uint16_t numberOfPlayers, const uint16_t numberOfRounds, const uint16_t mapHeight, const uint16_t mapWidth, int roomID);
	
	~Game();

public:
	bool ClickedOnRegion(const QPointF& coordClick, const QPointF& coordRegion);
	bool CheckIfRegionsAreNeighbors(const QPointF& coordRegion1, const QPointF& coordRegion2);
	void AddNewSelectedRegion(const QPointF& coordPos);

	void StartInitializeQuestionsGeneratorTimer();

private:
	void GenerateRandomColor();

	void paintEvent(QPaintEvent*);
	
	void DrawMap(QPainter& painter);

	void mouseReleaseEvent(QMouseEvent* ev);

private slots:
	void closeEvent(QCloseEvent* e);

	void OnIinitializeQuestionsGeneratorTimerTick();

private:
	QTimer* initializeQuestionsGeneratorTimer;

	Map m_map;

	QColor m_usedColor;
	
	std::vector<QPointF> m_selectedRegions;

	QTimer* transferTimer;

	std::string m_ip;
	std::string m_playerUsername;

	int m_roomID;

private:
	GameElementsGenerator* QuestionsWindow;
};

