#include "LoginForm.h"
#include "utils.h"
#include "RoutingManager.h"
#include <QTime>
#include <regex>

LoginForm::LoginForm(QWidget* parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);

	m_ui.errorLabel->hide();

	connect(m_ui.loginButton, SIGNAL(clicked()), this, SLOT(OnLoginButtonClicked()));
	connect(m_ui.registerButton, SIGNAL(clicked()), this, SLOT(OnRegisterButtonClicked()));
}

void LoginForm::ValidateUserLogin(const std::string& username, const std::string& password) const
{
	Utils::CheckUsernameForLogin(username);
	Utils::CheckPasswordPattern(password);

	cpr::Response loginRequest = RoutingManager::LoginUser(username, password);

	if (Utils::IsResponseSuccessful(loginRequest, 409)) {
		throw std::exception("This account is already logged in from another instance.");
	}

	if (!Utils::IsResponseSuccessful(loginRequest, 204)) {
		throw std::exception("Username or password are invalid.");
	}
}

void LoginForm::OnLoginButtonClicked() noexcept
{
	std::string username = m_ui.usernameField->text().toUtf8().constData();
	std::string password = m_ui.passwordField->text().toUtf8().constData();

	try {
		ValidateUserLogin(username, password);
		m_ui.errorLabel->show();
		m_ui.errorLabel->setStyleSheet("QLabel { color : rgb(221, 242, 253); }");
		m_ui.errorLabel->setText("You have successfully logged into your account.");
		Utils::WaitForSeconds(1);

		emit NavigateToMenu(username);
	} catch (const std::exception& exception) {
		m_ui.errorLabel->show();
		m_ui.errorLabel->setText(exception.what());
		Utils::WaitForSeconds(2);
		m_ui.errorLabel->hide();
	}
}

void LoginForm::OnRegisterButtonClicked() noexcept
{
	emit NavigateToRegister();
}
