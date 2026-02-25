#pragma once

#include <QWidget>
#include "ui_RegisterForm.h"

class RegisterForm : public QWidget
{
	Q_OBJECT

signals:
	void NavigateToLogin();

public:
	RegisterForm(QWidget *parent = nullptr);
	~RegisterForm() = default;

private:
	Ui::RegisterFormClass m_ui;

private:
	void AddUserToDatabase(const std::string& username, const std::string& password, const std::string& email) const;

private slots:
	void OnRegisterButtonClicked() noexcept;
	void OnBackButtonClicked() noexcept;
};
