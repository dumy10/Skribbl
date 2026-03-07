#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include "ServerConnectForm.h"
#include "LoginForm.h"
#include "RegisterForm.h"
#include "Menu.h"
#include "Lobby.h"
#include "Game.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private slots:
	void ShowLoginForm();
	void ShowRegisterForm();
	void ShowMenu(const std::string& username);
	void ShowLobby(const std::string& username, int playerIndex, bool isOwner, const std::string& roomID);
	void ShowGame(const std::string& username, int playerIndex, bool isOwner, const std::string& roomID);
	void ReturnToServerConnect();

private:
	void closeEvent(QCloseEvent* event) override;

private:
	QStackedWidget* m_stackedWidget;
	
	ServerConnectForm* m_serverConnectForm;
	LoginForm* m_loginForm;
	RegisterForm* m_registerForm;
	Menu* m_menu;
	Lobby* m_lobby;
	Game* m_game;

	std::string m_currentUsername;
};
