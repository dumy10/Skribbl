#include "RegisterForm.h"
#include "LoginForm.h"
#include <regex>
#include <QTime>
#include <QCoreApplication>
#include <QPainter>
#include <QPixmap>
#include <cpr/cpr.h>
#include <crow.h>

RegisterForm::RegisterForm(QWidget* parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);

	connect(m_ui.registerButton, SIGNAL(clicked()), this, SLOT(onRegisterButtonClicked()));

}

RegisterForm::~RegisterForm()
{
	//empty
}


void RegisterForm::checkEmailPattern(const std::string& email)
{
	if (email == "")
		throw std::exception("Email cannot be empty");
	const std::regex emailPattern("^([a-zA-Z0-9_\\ \\.+)@([a-zA-Z0-9_\\-\\.]+)\\.([a-zA-Z]{2,5})$");
	if (!std::regex_match(email, emailPattern))
		throw std::exception("Invalid email");
}


void RegisterForm::checkPasswordPattern(const std::string& password)
{
	if (password == "")
		throw std::exception("Password cannot be empty");
	if (password.length() < 6)
		throw std::exception("Password must be at least 6 characters long");

	const std::regex passwordPattern("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)[a-zA-Z\\d]{6,}$");
	if (!std::regex_match(password, passwordPattern))
		throw std::exception("Invalid password");
}

void RegisterForm::waitForSeconds(int seconds)
{
	QTime delayTime = QTime::currentTime().addSecs(seconds);
	while (QTime::currentTime() < delayTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void RegisterForm::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QPixmap background("D:\Namenotfound1.git\Client\pattern.png");
	painter.drawPixmap(0, 0, width(), height(), background);
}


void RegisterForm::onRegisterButtonClicked()
{

	QString username = m_ui.usernameField->text();
	QString password = m_ui.passwordField->text();
	QString email = m_ui.emailField->text();
	try {
		//checkUser(username.toStdString()); //tbi with database -> check if user exists
		checkEmailPattern(email.toStdString());
		checkPasswordPattern(password.toStdString());
	}
	catch (std::exception& e)
	{
		m_ui.errorLabel->setText(e.what());
		return;

	}
	if (username.isEmpty())
	{
		m_ui.errorLabel->setText("Please fill in all fields.");
		return;
	}
	m_ui.errorLabel->setStyleSheet("QLabel { color : rgb(221, 242, 253); }");
	m_ui.errorLabel->setText("You have successfully registered.");
	waitForSeconds(5);
	this->close();
	LoginForm* loginForm = new LoginForm();
	loginForm->show();


}