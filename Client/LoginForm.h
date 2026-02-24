#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_LoginForm.h"

class LoginForm : public QMainWindow
{
    Q_OBJECT

public:
    LoginForm(QWidget *parent = nullptr);
    ~LoginForm() = default;

private:
    Ui::LoginFormClass m_ui;

private:
    void WaitForSeconds(int seconds) const noexcept;
    void CheckUsername(const std::string& username) const;
    void CheckPassword(const std::string& password) const;
    void ValidateUserLogin(const std::string& username, const std::string& password) const;

private slots:
    void OnLoginButtonClicked() noexcept;
    void OnRegisterButtonClicked() noexcept;
};
