#pragma once

#include "ui_Triviador.h"

class Triviador : public QMainWindow
{
    Q_OBJECT

public:
    Triviador(QWidget* parent = nullptr);
    ~Triviador();

private:
	Ui::TriviadorClass ui;

private slots:
    void on_profilePushButton_released();
    void on_backToMenuPushButton_released();
	void on_quitPushButton_released();
};
