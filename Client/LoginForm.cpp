#include "LoginForm.h"
#include "RegisterForm.h"
#include <QMessageBox>


LoginForm::LoginForm(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);


    connect(ui.loginButton, SIGNAL(clicked()), this, SLOT(onLoginButtonClicked()));
    connect(ui.registerButton, SIGNAL(clicked()), this, SLOT(onRegisterButtonClicked()));
}

LoginForm::~LoginForm()
{
}

void LoginForm::onLoginButtonClicked()
{
    QString username = ui.usernameField->text();
    QString password = ui.passwordField->text();

    if (username.isEmpty() || password.isEmpty())
    {
        ui.errorLabel->setText("Please fill in all fields.");
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
    RegisterForm *registerForm = new RegisterForm();
	registerForm->show();
    this->close();
}