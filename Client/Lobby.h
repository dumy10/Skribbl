#ifndef LOBBY_H
#define LOBBY_H

#include <QMainWindow>
#include <ui_Lobby.h>

class Lobby : public QMainWindow
{
	Q_OBJECT

public:
	Lobby(const std::string& username, int playerIndex = 0, bool isOwner = false, QWidget* parent = nullptr);
	~Lobby();

private slots:
	void OnCreateLobbyButtonPress();
	void OnStartGameButtonPress();
	void OnBackButtonPress();

private:
	void GetRoomID();
	void DisplayPlayer(const std::string& username, int index);

private:
	Ui::Lobby m_ui;
	std::string m_username;
	std::string m_roomID;
	bool m_isOwner;
	int m_playerIndex;
};
#endif // !LOBBY_H
