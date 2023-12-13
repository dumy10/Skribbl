#pragma once

#include <QMainWindow>
#include "ui_Menu.h"

class Menu : public QMainWindow
{
	Q_OBJECT

public:
	Menu(const std::string& username, QWidget* parent = nullptr);
	~Menu();
private:
	Ui::MenuClass m_ui;
	std::string m_username;
private slots:
	void onCreateButtonClicked();
	void onJoinButtonClicked();
	void onJoinGameButtonClicked();
};
