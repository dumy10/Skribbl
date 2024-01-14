#include "LoginForm.h"
#include "RegisterForm.h"
#include "Menu.h"
#include "utils.h"
#include <QTime>
#include <regex>

#include <cpr/cpr.h>
#include <crow.h>

LoginForm::LoginForm(QWidget* parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);

	m_ui.errorLabel->hide();

	connect(m_ui.loginButton, SIGNAL(clicked()), this, SLOT(OnLoginButtonClicked()));
	connect(m_ui.registerButton, SIGNAL(clicked()), this, SLOT(OnRegisterButtonClicked()));
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
		cpr::Url{ Server::GetUrl() + "/checkUsername" },
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
		cpr::Url{ Server::GetUrl() + "/loginUser" },
		cpr::Payload{ {"username", username}, {"password", password} }
	);

	if (loginRequest.status_code != 200)
		throw std::exception(loginRequest.text.c_str());
}

void LoginForm::OnLoginButtonClicked()
{
	std::string username = m_ui.usernameField->text().toUtf8().constData();
	std::string password = m_ui.passwordField->text().toUtf8().constData();

	try
	{
		ValidateUserLogin(username, password);
		m_ui.errorLabel->show();
		m_ui.errorLabel->setStyleSheet("QLabel { color : rgb(221, 242, 253); }");
		m_ui.errorLabel->setText("You have successfully logged into your account.");
		WaitForSeconds(1);

		Menu* menu = new Menu(std::move(username));
		menu->show();
		this->close();
		this->deleteLater();
	}
	catch (const std::exception& exception)
	{
		m_ui.errorLabel->show();
		m_ui.errorLabel->setText(exception.what());
		WaitForSeconds(2);
		m_ui.errorLabel->hide();
	}

}

void LoginForm::OnRegisterButtonClicked()
{
	RegisterForm* registerForm = new RegisterForm();
	registerForm->show();
	this->close();
	this->deleteLater();
}