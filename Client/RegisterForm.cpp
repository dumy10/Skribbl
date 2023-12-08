#include "RegisterForm.h"
#include "LoginForm.h"
#include <regex>
#include <QTime>
#include <QCoreApplication>
#include <QDebug>
#include "Hashing.h"

//#include <cpr/cpr.h>
//#include <crow.h>

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

void RegisterForm::CheckUsername(const std::string& username)
{
	if(username == "")
		throw std::exception("Username cannot be empty");
	/*
	Send a request to the server to check if the username is already taken
	*/

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

/*
TODO:
- check if user exists
- Hasher getting a null password passed
- add user to database 
*/

void RegisterForm::onRegisterButtonClicked()
{

	QString username = m_ui.usernameField->text();
	QString password = m_ui.passwordField->text();
	QString email = m_ui.emailField->text();
	std::string stringPassword = password.toUtf8().data();
	try {
		QMessageLogger logger;
		logger.debug() << password;
		Hasher::HashPassword(stringPassword);
		//logger.warning("Hashed password: %s", hashedPwd.c_str());
		//checkUser(username.toUtf8().constData()); //tbi with database -> check if user exists
		checkEmailPattern(email.toUtf8().constData());
		checkPasswordPattern(password.toUtf8().constData());
	}
	catch (std::exception& e)
	{
		m_ui.errorLabel->setText(e.what());
		return;

	}
	if (username.isEmpty())
	{
		m_ui.errorLabel->setText("Username can't be empty.");
		return;
	}

	m_ui.errorLabel->setStyleSheet("QLabel { color : rgb(221, 242, 253); }");
	m_ui.errorLabel->setText("You have successfully registered.");
	waitForSeconds(5);
	//this->close();
	//LoginForm* loginForm = new LoginForm();
	//loginForm->show();

}