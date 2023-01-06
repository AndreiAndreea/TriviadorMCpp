#pragma once

#include <QDialog>

#include "ui_Game.h"

#include <QFile>
#include <QTextStream>

#include "Map.h"
#include "GameElementsGenerator.h"

#include <cstdlib>

#include <cpr/cpr.h>

class Game : public QDialog
{
	Q_OBJECT

private:
	Ui::Game ui;
	GameElementsGenerator w;

public:
	explicit Game();
	
	Game(const std::string& ip, const std::string& username, const uint16_t numberOfPlayers, const uint16_t numberOfRounds, const uint16_t mapHeight, const uint16_t mapWidth);
	
	~Game();

public:
	bool ClickedOnRegion(const QPointF& coordClick, const QPointF& coordRegion);
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
	
	std::vector<QColor> m_colorList = { Qt::yellow, Qt:: blue, Qt::green, Qt::cyan, Qt::magenta, Qt::red, Qt::darkGreen, Qt::darkCyan, Qt::darkMagenta, Qt::darkRed };
	
	std::vector<QPointF> m_selectedRegions;

	QTimer* transferTimer;

	std::string m_ip;
	std::string m_playerUsername;
};

