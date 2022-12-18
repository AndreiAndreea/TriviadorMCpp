#pragma once

#include <QDialog>

#include "ui_Game.h"

#include <QFile>
#include <QTextStream>

#include "Map.h"
#include "GameElementsGenerator.h"

#include <cstdlib>

class Game : public QDialog
{
	Q_OBJECT

private:
	Ui::Game ui;
	GameElementsGenerator w;

public:
	explicit Game();
	~Game();

public:
	bool ClickedOnRegion(const QPointF& coordClick, const QPointF& coordRegion);
	void AddNewSelectedRegion(const QPointF& coordPos);

private:
	void GenerateRandomColor();
	void paintEvent(QPaintEvent*);
	void DrawMap(QPainter& painter);

	void mouseReleaseEvent(QMouseEvent* ev);

	QString CheckMapSize(uint16_t mapHeight ,uint16_t mapWidth, uint16_t numberOfPlayers);

private slots:
	void on_playersSpinBox_valueChanged(int arg1);
	void on_twoPlayersPushButton_released();
	void on_threePlayersPushButton_released();
	void on_fourPlayersPushButton_released();
	void on_customModePushButton_released();
	void on_finishGameModeSetupPushButton_released();

private:
	bool gameModeIsSet;

	Map m_map;

	QColor m_usedColor;
	
	std::vector<QColor> m_colorList = { Qt::yellow, Qt:: blue, Qt::green, Qt::cyan, Qt::magenta, Qt::red, Qt::darkGreen, Qt::darkCyan, Qt::darkMagenta, Qt::darkRed };
	
	std::vector<QPointF> m_selectedRegions;
};

