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
	void checkEmailPattern(const std::string& email);
	void checkPasswordPattern(const std::string& password);
	void waitForSeconds(int seconds);
	void paintEvent(QPaintEvent* event);
private slots:
	void onRegisterButtonClicked();
};
