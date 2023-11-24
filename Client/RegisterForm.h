#pragma once

#include <QMainWindow>
#include "ui_RegisterForm.h"

class RegisterForm : public QMainWindow
{
	Q_OBJECT

public:
	RegisterForm(QWidget *parent = nullptr);
	~RegisterForm();

private:
	Ui::RegisterFormClass ui;

private:
	void checkEmailPattern(const std::string& email);
	void checkPasswordPattern(const std::string& password);
private slots:
	void onRegisterButtonClicked();
};
