#pragma once

#include <QMainWindow>
#include "ui_LobbyID.h"

class LobbyID : public QMainWindow
{
	Q_OBJECT

public:
	LobbyID(QWidget *parent = nullptr);
	~LobbyID();
	void onJoinRoomButtonPress();
	void setUserName(const std::string& username);
	bool correctRoomID();

private:
	Ui::LobbyIDClass m_ui;
	std::string m_username;
	bool correctRoomIDFlag = false;
};
