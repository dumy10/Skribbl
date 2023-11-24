#include "RegisterForm.h"

RegisterForm::RegisterForm(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	connect(ui.registerButton, SIGNAL(clicked()), this, SLOT(onRegisterButtonClicked()));

}

RegisterForm::~RegisterForm()
{}
void RegisterForm::onRegisterButtonClicked()
{
	QString username = ui.usernameField->text();
	QString password = ui.passwordField->text();

	if (username.isEmpty() || password.isEmpty())
	{
		ui.errorLabel->setText("Please fill in all fields.");
	
		return;
	}
	
}