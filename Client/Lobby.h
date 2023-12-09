#ifndef LOBBY_H
#define LOBBY_H

#include <QMainWindow>
#include <ui_Lobby.h>

class Lobby : public QMainWindow
{
	Q_OBJECT

public:
	Lobby(const std::string& username, QWidget* parent = nullptr);
	~Lobby();

private slots:

private:
	Ui::Lobby m_ui;
	std::string m_username;
private:
	void getRoomID();
};
#endif // !LOBBY_H
