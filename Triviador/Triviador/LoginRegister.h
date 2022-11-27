#pragma once

#include <QDialog>

#include "ui_LoginRegister.h"

class LoginRegister : public QDialog
{
	Q_OBJECT

private:
	Ui::LoginRegister ui;

public:
	explicit LoginRegister();
	~LoginRegister();

	//explicit Login(QWidget* parent);

private slots:
	void on_signInButton_released();
	void on_signUpButton_released();
;
};

