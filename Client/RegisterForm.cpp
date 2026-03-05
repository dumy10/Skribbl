#include "RegisterForm.h"
#include <regex>
#include <QTime>
#include <QCoreApplication>
#include "../PasswordHashing/Hashing.h"

#include "utils.h"
#include "RoutingManager.h"

RegisterForm::RegisterForm(QWidget* parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);

	m_ui.errorLabel->hide();

	connect(m_ui.registerButton, SIGNAL(clicked()), this, SLOT(OnRegisterButtonClicked()));
	connect(m_ui.backButton, SIGNAL(clicked()), this, SLOT(OnBackButtonClicked()));

}

void RegisterForm::AddUserToDatabase(const std::string& username, const std::string& password, const std::string& email) const 
{
	cpr::Response response = RoutingManager::AddUser(username, password, email);

	if (Utils::IsResponseSuccessful(response, 201)) {
		throw std::exception("Could not add user to database");
	}
}
void RegisterForm::OnBackButtonClicked() noexcept
{
	emit NavigateToLogin();
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

		Utils::CheckUsernameForRegistration(username.toUtf8().constData());
		Utils::CheckEmailPattern(email.toUtf8().constData());
		Utils::CheckPasswordPattern(password.toUtf8().constData());

		AddUserToDatabase(username.toUtf8().constData(), hashedPassStr, email.toUtf8().constData());
	} catch (std::exception& e) {
		m_ui.errorLabel->show();
		m_ui.errorLabel->setText(e.what());
		Utils::WaitForSeconds(2);
		m_ui.errorLabel->hide();
		return;
	}
	m_ui.errorLabel->show();
	m_ui.errorLabel->setStyleSheet("QLabel { color : rgb(221, 242, 253); }");
	m_ui.errorLabel->setText("You have successfully registered.");

	Utils::WaitForSeconds(2);
	emit NavigateToLogin();
}