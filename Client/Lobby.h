#ifndef LOBBY_H
#define LOBBY_H

#include <QMainWindow>
#include <ui_Lobby.h>

class Lobby : public QMainWindow
{
	Q_OBJECT

public:
	Lobby(const std::string& username, bool isOwner = false, QWidget* parent = nullptr);
	~Lobby();

private slots:
	void onCreateLobbyButtonPress();
	void onStartGameButtonPress();
	void onBackButtonPress();
private:
	Ui::Lobby m_ui;
	std::string m_username;
	std::string m_roomID;
	bool m_isOwner;
	int m_playerIndex;
private:
	void GetRoomID();
	void DisplayPlayer(const std::string& username, int index);
};
#endif // !LOBBY_H
