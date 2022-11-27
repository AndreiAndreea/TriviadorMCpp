#pragma once

#include <QDialog>

#include "ui_Login.h"

#include <QFile>
#include <QTextStream>

class Login : public QDialog
{
	Q_OBJECT

private:
	Ui::Login ui;

public:
	explicit Login();
	~Login();
};