#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_LoginForm.h"

class LoginForm : public QMainWindow
{
    Q_OBJECT

public:
    LoginForm(QWidget *parent = nullptr);
    ~LoginForm();

private:
    Ui::LoginFormClass m_ui;

private:
    void WaitForSeconds(int seconds);
    void CheckUsername(const std::string& username);
    void CheckPassword(const std::string& password);
    void ValidateUserLogin(const std::string& username, const std::string& password);

private slots:
    void OnLoginButtonClicked();
    void OnRegisterButtonClicked();

};
