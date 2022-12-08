#pragma once

#include <QDialog>

#include "ui_Game.h"

#include <QFile>
#include <QTextStream>

#include "Map.h"
#include "GameElementsGenerator.h"

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
	uint16_t GetNumberOfPlayers();
	bool ClickedOnRegion(const QPointF& coordClick, const QPointF& coordRegion);
	void AddNewSelectedRegion(const QPointF& coordPos);

private:
	void paintEvent(QPaintEvent*);
	void DrawMap(QPainter& painter);

	void mouseReleaseEvent(QMouseEvent* ev);

private slots:
	void on_numberOfPlayersLineEdit_textChanged(const QString& arg1);

private:
	Map m_map;

	std::vector<QPointF> m_selectedRegions;
};

