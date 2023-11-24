#include "RegisterForm.h"
#include <regex>
RegisterForm::RegisterForm(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	connect(ui.registerButton, SIGNAL(clicked()), this, SLOT(onRegisterButtonClicked()));

}

RegisterForm::~RegisterForm()
{}
void RegisterForm::checkEmailPattern(const std::string& email)
{
	if(email=="")
		throw std::exception("Email cannot be empty");
	const std::regex emailPattern("^([a-zA-Z0-9_\\ \\.+)@([a-zA-Z0-9_\\-\\.]+)\\.([a-zA-Z]{2,5})$");
	if (!std::regex_match(email, emailPattern))
		throw std::exception("Invalid email");
}
void RegisterForm::onRegisterButtonClicked()
{
	QString username = ui.usernameField->text();
	QString password = ui.passwordField->text();
	QString email = ui.emailField->text();
	try{
		checkEmailPattern(email.toStdString());
	}
	catch (std::exception& e)
	{
		ui.errorLabel->setText(e.what());
		return;
	
	}
	if (username.isEmpty() || password.isEmpty())
	{
		ui.errorLabel->setText("Please fill in all fields.");
	
		return;
	}
	
}