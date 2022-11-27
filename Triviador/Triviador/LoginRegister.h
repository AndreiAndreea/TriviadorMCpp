#pragma once

#include <QDialog>

#include "ui_LoginRegister.h"

#include <QFile>
#include <QTextStream>

class LoginRegister : public QDialog
{
	Q_OBJECT

private:
	Ui::LoginRegister ui;

public:
	explicit LoginRegister();
	~LoginRegister();

	//explicit Login(QWidget* parent); - Cosmin

private:
	
private slots:
	void on_signInButton_released();
	void on_signUpButton_released();

private:
};

