#include "LoginRegister.h"

LoginRegister::LoginRegister()
{
	ui.setupUi(this);

	ui.GreetingLabel->show();
	ui.signInLabel->hide();
	ui.singUpLabel->hide();
}

LoginRegister::~LoginRegister()
{

}

void LoginRegister::on_signInButton_released()
{
	if (ui.signInButton->isEnabled())
	{
		ui.GreetingLabel->hide();
		ui.signInButton->hide();
		ui.signUpButton->hide();
		ui.signInLabel->show();
	}
}

void LoginRegister::on_signUpButton_released()
{
	if (ui.signUpButton->isEnabled())
	{
		ui.GreetingLabel->hide();
		ui.signInButton->hide();
		ui.signUpButton->hide();
		ui.singUpLabel->show();
	}
}
