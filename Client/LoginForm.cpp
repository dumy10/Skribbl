#include "LoginForm.h"
#include "RegisterForm.h"
#include <QMessageBox>




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

void LoginForm::onLoginButtonClicked()
{
	QString username = m_ui.usernameField->text();
	QString password = m_ui.passwordField->text();

	if (username.isEmpty() || password.isEmpty())
	{
		m_ui.errorLabel->setText("Please fill in all fields.");
		return;
	}

	if (username == "admin" && password == "admin")
	{
		QMessageBox::information(this, "Success", "You have successfully logged in.");
		return;
	}


}

void LoginForm::onRegisterButtonClicked()
{
	RegisterForm* registerForm = new RegisterForm();
	registerForm->show();
	this->close();
}