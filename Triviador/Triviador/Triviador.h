#pragma once

#include "ui_Triviador.h"

#include "Game.h"

#include<QDebug>

class Triviador : public QMainWindow
{
    Q_OBJECT

public:
    Triviador(QWidget* parent = nullptr);
    ~Triviador();

private:
	Ui::TriviadorClass ui;

private slots:
    void on_playGamePushButton_released();
    void on_backToMenuPushButton_released();
    void on_quitPushButton_released();
    
    void on_profilePushButton_released();
    void on_profileSettingsPushButton_released();

    void on_changeUsernamePushButton_released();
    void on_changePasswordPushButton_released();
    void on_changeEmailPushButton_released();

};
