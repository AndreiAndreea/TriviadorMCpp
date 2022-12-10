#include "Triviador.h"

#include<QDebug>

Triviador::Triviador(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	ui.menuWidget->show();
	
	ui.profileWidget->hide();
}

Triviador::~Triviador()
{
	
}

void Triviador::on_profilePushButton_released()
{
	ui.menuWidget->hide();
	
	ui.profileWidget->show();
}
void Triviador::on_backToMenuPushButton_released()
{
	ui.menuWidget->show();
	
	ui.profileWidget->hide();
}

void Triviador::on_quitPushButton_released()
{
	close();
}
