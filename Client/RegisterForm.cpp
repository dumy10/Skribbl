#include "RegisterForm.h"
#include "LoginForm.h"
#include <regex>
#include <QTime>
#include <QCoreApplication>
#include "../PasswordHashing/Hashing.h"

#include "utils.h"
#include <cpr/cpr.h>

RegisterForm::RegisterForm(QWidget* parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);

	m_ui.errorLabel->hide();

	connect(m_ui.registerButton, SIGNAL(clicked()), this, SLOT(OnRegisterButtonClicked()));
	connect(m_ui.backButton, SIGNAL(clicked()), this, SLOT(OnBackButtonClicked()));

}

void RegisterForm::CheckUsername(const std::string& username) const
{
	if (username.empty()) {
		throw std::exception("Username cannot be empty");
	}

	cpr::Response response = cpr::Get(cpr::Url{ Server::GetUrl() + "/checkUsername" },
		cpr::Payload{ {"username", username} }
	);

	if (response.status_code != 200 && response.status_code != 404) {
		throw std::exception("Server error");
	}

	if (response.text == "EXISTING") {
		throw std::exception("Username already taken");
	}
}

void RegisterForm::CheckEmailPattern(const std::string& email) const
{

	if (email.empty()) {
		throw std::exception("Email cannot be empty");
	}

	const std::regex emailPattern("^([a-zA-Z0-9_\\ \\.+)@([a-zA-Z0-9_\\-\\.]+)\\.([a-zA-Z]{2,5})$");

	if (!std::regex_match(email, emailPattern)) {
		throw std::exception("Email should be like: example@mail.com");
	}
}

void RegisterForm::CheckPasswordPattern(const std::string& password) const
{
	if (password.empty()) {
		throw std::exception("Password cannot be empty");
	}

	if (password.length() < 6) {
		throw std::exception("Password must be at least 6 characters long");
	}

	const std::regex passwordPattern("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)[a-zA-Z\\d]{6,}$");
	if (!std::regex_match(password, passwordPattern)) {
		throw std::exception("The Password needs to contain at least 1 upper case,\n one lower case and a number");
	}
}

void RegisterForm::AddUserToDatabase(const std::string& username, const std::string& password, const std::string& email) const 
{
	cpr::Response response = cpr::Post(cpr::Url{ Server::GetUrl() + "/addUser" },
		cpr::Payload{ {"username", username}, {"password", password}, {"email", email} }
	);

	if (response.status_code != 201) {
		throw std::exception("Could not add user to database");
	}
}

void RegisterForm::WaitForSeconds(int seconds) const noexcept
{
	QTime delayTime = QTime::currentTime().addSecs(seconds);
	while (QTime::currentTime() < delayTime) {
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
	}
}

void RegisterForm::OnBackButtonClicked() noexcept
{
	this->close();
	this->deleteLater();
	LoginForm* loginForm = new LoginForm();
	loginForm->show();
}

void RegisterForm::OnRegisterButtonClicked() noexcept
{
	QString username = m_ui.usernameField->text();
	QString password = m_ui.passwordField->text();
	QString email = m_ui.emailField->text();
	try {
		std::string hashedPass = password.toUtf8().data();
		Hasher::HashPassword(hashedPass.c_str());
		std::string hashedPassStr{ hashedPass };

		CheckUsername(username.toUtf8().constData());
		CheckEmailPattern(email.toUtf8().constData());
		CheckPasswordPattern(password.toUtf8().constData());

		AddUserToDatabase(username.toUtf8().constData(), hashedPassStr, email.toUtf8().constData());
	} catch (std::exception& e) {
		m_ui.errorLabel->show();
		m_ui.errorLabel->setText(e.what());
		WaitForSeconds(2);
		m_ui.errorLabel->hide();
		return;
	}
	m_ui.errorLabel->show();
	m_ui.errorLabel->setStyleSheet("QLabel { color : rgb(221, 242, 253); }");
	m_ui.errorLabel->setText("You have successfully registered.");

	WaitForSeconds(2);
	this->close();
	this->deleteLater();
	LoginForm* loginForm = new LoginForm();
	loginForm->show();
}