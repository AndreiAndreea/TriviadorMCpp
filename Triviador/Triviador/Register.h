#pragma once

#include <QDialog>

#include "ui_Register.h"
#include "Player.h"

#include <ctime>
#include <iomanip>

#include <QFile>
#include <QTextStream>

#include <sqlite_orm/sqlite_orm.h>

using namespace sqlite_orm;

class Register : public QDialog
{
	Q_OBJECT

private:
	Ui::Register ui;

public:
	explicit Register();
	~Register();

	//explicit Login(QWidget* parent);

private:
	std::string GetCurrentDate();

	const std::string GetInputUsernameByPlayer() const;
	const std::string GetInputPasswordByPlayer() const;
	const std::string GetInputEmailByPlayer() const;

private slots:

	void on_displayPasswordPushButton_pressed();
	void on_displayPasswordPushButton_released();

	void on_submitDataPushButton_released();

private:
	Player m_currentPlayer;
};