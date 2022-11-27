#include "LoginRegister.h"

#include "Register.h"
#include "Login.h"

#include <regex>

LoginRegister::LoginRegister()
{
	ui.setupUi(this);
}

LoginRegister::~LoginRegister()
{

}

void LoginRegister::on_signInButton_released()
{
	if (ui.signInButton->isEnabled())
	{
		Login* loginWindow = new Login();
		loginWindow->show();
		this->hide();
	}
}

void LoginRegister::on_signUpButton_released()
{
	if (ui.signUpButton->isEnabled())
	{
		Register* registerWindow = new Register();
		registerWindow->show();
		this->hide();
	}
}