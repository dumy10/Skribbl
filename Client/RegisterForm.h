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
	Ui::RegisterFormClass m_ui;

private:
	void CheckUsername(const std::string& username);
	void CheckEmailPattern(const std::string& email);
	void CheckPasswordPattern(const std::string& password);
	void AddUserToDataBase(const std::string& username, const std::string& password, const std::string& email);
	void WaitForSeconds(int seconds);

private slots:
	void OnRegisterButtonClicked();
	void OnBackButtonClicked();
};
