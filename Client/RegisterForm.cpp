#include "RegisterForm.h"
#include "LoginForm.h"
#include <regex>
#include <QTime>
#include <QCoreApplication>
#include <QDebug>
#include "Hashing.h"

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

void RegisterForm::CheckUsername(const std::string& username)
{
	if(username == "")
		throw std::exception("Username cannot be empty");

	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/checkUsername" },
				cpr::Parameters{ {"username", username} });

	if (response.status_code != 200)
		throw std::exception("Server error");

	if (response.text == "true")
		throw std::exception("Username already taken");

}

void RegisterForm::CheckEmailPattern(const std::string& email)
{

	if (email == "")
		throw std::exception("Email cannot be empty");

	const std::regex emailPattern("^([a-zA-Z0-9_\\ \\.+)@([a-zA-Z0-9_\\-\\.]+)\\.([a-zA-Z]{2,5})$");

	if (!std::regex_match(email, emailPattern))
		throw std::exception("Invalid email");

}

void RegisterForm::CheckPasswordPattern(const std::string& password)
{
	if (password == "")
		throw std::exception("Password cannot be empty");

	if (password.length() < 6)
		throw std::exception("Password must be at least 6 characters long");

	const std::regex passwordPattern("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)[a-zA-Z\\d]{6,}$");
	if (!std::regex_match(password, passwordPattern))
		throw std::exception("Invalid password");

}

void RegisterForm::AddUserToDataBase(const std::string& username, const std::string& password, const std::string& email)
{
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/addUser" },
				cpr::Parameters{ {"username", username}, {"password", password}, {"email", email} });

	if (response.status_code != 200)
		throw std::exception("Server error");

	if (response.text == "false")
		throw std::exception("Could not add user to database");
}

void RegisterForm::WaitForSeconds(int seconds)
{
	QTime delayTime = QTime::currentTime().addSecs(seconds);
	while (QTime::currentTime() < delayTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

/*
TODO:
- Hasher getting a null password passed
*/

void RegisterForm::onRegisterButtonClicked()
{

	QString username = m_ui.usernameField->text();
	QString password = m_ui.passwordField->text();
	QString email = m_ui.emailField->text();
	//std::string stringPassword = password.toUtf8().data();
	try {
		//QMessageLogger logger;
		//logger.debug() << password;
		//Hasher::HashPassword(stringPassword);
		//logger.warning("Hashed password: %s", hashedPwd.c_str());
		CheckUsername(username.toUtf8().constData()); 
		CheckEmailPattern(email.toUtf8().constData());
		CheckPasswordPattern(password.toUtf8().constData());

		AddUserToDataBase(username.toUtf8().constData(), password.toUtf8().constData(), email.toUtf8().constData());
	}
	catch (std::exception& e)
	{
		m_ui.errorLabel->setText(e.what());
		return;

	}

	m_ui.errorLabel->setStyleSheet("QLabel { color : rgb(221, 242, 253); }");
	m_ui.errorLabel->setText("You have successfully registered.");
	WaitForSeconds(5);
	this->close();
	LoginForm* loginForm = new LoginForm();
	loginForm->show();

}