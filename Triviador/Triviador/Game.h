#pragma once

#include <QDialog>

#include "ui_Game.h"

#include <QFile>
#include <QTextStream>

#include "Map.h"

class Game : public QDialog
{
	Q_OBJECT

private:
	Ui::Game ui;

public:
	explicit Game();
	~Game();

public:
	uint16_t GetNumberOfPlayers();

private:
	void paintEvent(QPaintEvent*);
	void DrawMap(QPainter& painter);

private slots:
	void on_numberOfPlayersLineEdit_textChanged(const QString& arg1);

private:
	Map m_map;

};

