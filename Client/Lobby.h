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
private:
	Ui::Lobby m_ui;
	std::string m_username;
	std::string m_roomID;
	bool m_isOwner;
private:
	void getRoomID();
};
#endif // !LOBBY_H
