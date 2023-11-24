#include "RegisterForm.h"
#include <regex>
RegisterForm::RegisterForm(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.registerButton, SIGNAL(clicked()), this, SLOT(onRegisterButtonClicked()));

}

RegisterForm::~RegisterForm()
{
	//empty
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
	if(password.length()<6)
		throw std::exception("Password must be at least 6 characters long");

	const std::regex passwordPattern("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)[a-zA-Z\\d]{6,}$");
	if (!std::regex_match(password, passwordPattern))
		throw std::exception("Invalid password");
}


void RegisterForm::onRegisterButtonClicked()
{

	QString username = ui.usernameField->text();
	QString password = ui.passwordField->text();
	QString email = ui.emailField->text();
	try {
		//checkUser(username.toStdString()); //tbi with database -> check if user exists
		checkEmailPattern(email.toStdString());
		checkPasswordPattern(password.toStdString());
	}
	catch (std::exception& e)
	{
		ui.errorLabel->setText(e.what());
		return;

	}
	if (username.isEmpty())
	{
		ui.errorLabel->setText("Please fill in all fields.");
		return;
	}
	ui.errorLabel->setStyleSheet("QLabel { color : rgb(221, 242, 253); }");
	ui.errorLabel->setText("You have successfully registered.");
}