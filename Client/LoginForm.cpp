#include "LoginForm.h"
#include "RegisterForm.h"
#include "Menu.h"
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QTime>
#include <regex>

#include <cpr/cpr.h>
#include <crow.h>

LoginForm::LoginForm(QWidget* parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);


	connect(m_ui.loginButton, SIGNAL(clicked()), this, SLOT(onLoginButtonClicked()));
	connect(m_ui.registerButton, SIGNAL(clicked()), this, SLOT(onRegisterButtonClicked()));
}

LoginForm::~LoginForm()
{
}

void LoginForm::WaitForSeconds(int seconds)
{
	QTime delayTime = QTime::currentTime().addSecs(seconds);
	while (QTime::currentTime() < delayTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void LoginForm::CheckUsername(const std::string& username)
{
	if (username == "")
		throw std::exception("Username cannot be empty");

	cpr::Response response = cpr::Get(
		cpr::Url{ "http://localhost:18080/checkUsername" },
		cpr::Payload{ {"username", username} }
	);

	if (response.status_code != 200)
		throw std::exception(response.text.c_str());

}

void LoginForm::CheckPassword(const std::string& password)
{
	if (password == "")
		throw std::exception("Password cannot be empty");

	if (password.length() < 6)
		throw std::exception("Password must be at least 6 characters long");

	const std::regex passwordPattern("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)[a-zA-Z\\d]{6,}$");
	if (!std::regex_match(password, passwordPattern))
		throw std::exception("The Password needs to contain at least 1 upper case,\n one lower case and a number");
}


void LoginForm::ValidateUserLogin(const std::string& username, const std::string& password)
{
	CheckUsername(username);
	CheckPassword(password);

	cpr::Response loginRequest = cpr::Get(
		cpr::Url{ "http://localhost:18080/loginUser" },
		cpr::Payload{ {"username", username}, {"password", password} }
	);

	if(loginRequest.status_code != 200)
		throw std::exception(loginRequest.text.c_str());
}

void LoginForm::onLoginButtonClicked()
{
	QString username = m_ui.usernameField->text();
	QString password = m_ui.passwordField->text();

	try
	{
		ValidateUserLogin(username.toUtf8().constData(), password.toUtf8().constData());

		m_ui.errorLabel->setStyleSheet("QLabel { color : rgb(221, 242, 253); }");
		m_ui.errorLabel->setText("You have successfully logged into your account.");
		WaitForSeconds(5);
		Menu* menu = new Menu();
		menu->show();
		this->close();
	}
	catch (const std::exception& exception)
	{
		m_ui.errorLabel->setText(exception.what());
	}

}

void LoginForm::onRegisterButtonClicked()
{
	RegisterForm* registerForm = new RegisterForm();
	registerForm->show();
	this->close();
}