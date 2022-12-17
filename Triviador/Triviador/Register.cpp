#include "Register.h"
#include "Login.h"
#include "Game.h"

#include <regex>

Register::Register()
{
	
}

Register::Register(const std::string& serverIP, const std::string& serverPort)
{
	m_serverIP = serverIP;
	m_serverPort = serverPort;

	m_ip = "http://" + m_serverIP + ":" + m_serverPort;

	ui.setupUi(this);

	ui.passwordLineEdit->setEchoMode(QLineEdit::Password);

	ui.emailErrorLabel->hide();

	ui.incompleteFieldsErrorLabel->hide();
	ui.submitDataErrorLabel->hide();

	ui.displayPasswordPushButton->setFlat(true);
	ui.displayPasswordPushButton->setStyleSheet("QPushButton { background-color: transparent; }");

	ui.displayPasswordPushButton->setIcon(QIcon("resources/img/icons/eye-off.png"));

	ui.progressBar->hide();
}

Register::~Register()
{

}

std::string Register::GetCurrentDate()
{
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	std::ostringstream oss;
	oss << std::put_time(&tm, "%d/%m/%Y at %H:%M:%S");
	return oss.str();
}

const std::string Register::GetInputUsernameByPlayer() const
{
	return ui.usernameLineEdit->text().toStdString();
}

const std::string Register::GetInputPasswordByPlayer() const
{
	return ui.passwordLineEdit->text().toStdString();
}

const std::string Register::GetInputEmailByPlayer() const
{
	return ui.emailLineEdit->text().toStdString();
}

bool isValidEmail(const std::string& email)
{
	std::regex validEmailPattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");

	return regex_match(email, validEmailPattern);
}

void Register::on_displayPasswordPushButton_pressed()
{
	ui.displayPasswordPushButton->setIcon(QIcon("resources/img/icons/eye-on.png"));

	ui.passwordLineEdit->setEchoMode(QLineEdit::Normal);
}

void Register::on_displayPasswordPushButton_released()
{
	ui.displayPasswordPushButton->setIcon(QIcon("resources/img/icons/eye-off.png"));

	ui.passwordLineEdit->setEchoMode(QLineEdit::Password);
}

void Register::on_submitDataPushButton_released()
{
	std::string usernameFromUser = ui.usernameLineEdit->text().toLocal8Bit().constData();
	std::string passwordFromUser = ui.passwordLineEdit->text().toLocal8Bit().constData();
	std::string emailFromUser = ui.emailLineEdit->text().toLocal8Bit().constData();
	std::string currentDate = GetCurrentDate();
	
	if (usernameFromUser.empty() || passwordFromUser.empty() || emailFromUser.empty())
	{
		ui.incompleteFieldsErrorLabel->show();
		ui.submitDataErrorLabel->hide();
		ui.emailErrorLabel->hide();
	}
	else if (!isValidEmail(emailFromUser))
	{
		ui.emailErrorLabel->show();
		ui.incompleteFieldsErrorLabel->hide();
		ui.submitDataErrorLabel->hide();
	}
	else
	{
		auto currentDateEncoded = curl_easy_escape(nullptr, currentDate.c_str(), 0);
		
		std::string link = m_ip + "/registeruser/?username=" + usernameFromUser + "&password=" + passwordFromUser + "&email=" + emailFromUser + "&accountCreationDate=" + currentDateEncoded;

		cpr::Response responseFromServer = cpr::Get(cpr::Url{ link });

		if (responseFromServer.status_code == 200)
		{
			ui.submitDataErrorLabel->setText("Your account has been created successfully. You will be sent to login page in a few moments.");
			ui.submitDataErrorLabel->show();

			ui.progressBar->show();
			StartTimer();
		}
		else if (responseFromServer.status_code == 400)
		{
			ui.submitDataErrorLabel->setText("The username or email you entered is already in use. Please try again.");
			ui.submitDataErrorLabel->show();
		}
		else
		{
			ui.submitDataErrorLabel->setText("An error has occurred. Please try again.");
			ui.submitDataErrorLabel->show();
		}
	}
}

void Register::on_registerBackPushButton_released()
{
	if (ui.registerBackPushButton->isEnabled())
	{
		LoginRegister* r = new LoginRegister;
		r->show();

		this->close();
	}
}

void Register::OnTimerTick()
{
	ui.progressBar->setValue(ui.progressBar->value() + 1);

	if (ui.progressBar->value() >= 100)
	{
		Login* loginWindow = new Login(m_ip);
		loginWindow->show();

		timer->disconnect();

		this->close();
	}
}

void Register::StartTimer()
{
	ui.progressBar->setValue(0);

	timer = new QTimer(this);

	timer->setInterval(30);
	timer->setTimerType(Qt::PreciseTimer);

	connect(timer, SIGNAL(timeout()), this, SLOT(OnTimerTick()));

	timer->start();
}
