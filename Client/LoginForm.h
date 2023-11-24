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
    Ui::LoginFormClass ui;

private slots:
    void onLoginButtonClicked();
    void onRegisterButtonClicked();
};
