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
	void CheckUsername(const std::string& username) const;
	void CheckEmailPattern(const std::string& email) const;
	void CheckPasswordPattern(const std::string& password) const;
	void AddUserToDatabase(const std::string& username, const std::string& password, const std::string& email) const;
	void WaitForSeconds(int seconds) const noexcept;

private slots:
	void OnRegisterButtonClicked() noexcept;
	void OnBackButtonClicked() noexcept;
};
