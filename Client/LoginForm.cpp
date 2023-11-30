#include "LoginForm.h"
#include "RegisterForm.h"
#include "Menu.h"
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QTime>

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

void LoginForm::waitForSeconds(int seconds)
{
	QTime delayTime = QTime::currentTime().addSecs(seconds);
	while (QTime::currentTime() < delayTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
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
		m_ui.errorLabel->setStyleSheet("QLabel { color : rgb(221, 242, 253); }");
		m_ui.errorLabel->setText("You have successfully registered.");
		waitForSeconds(5);
		Menu* menu = new Menu();
		menu->show();
		this->close();

		return;
	}


}

void LoginForm::onRegisterButtonClicked()
{
	RegisterForm* registerForm = new RegisterForm();
	registerForm->show();
	this->close();
}