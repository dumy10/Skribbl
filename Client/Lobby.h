#ifndef LOBBY_H
#define LOBBY_H

#include <QMainWindow>
#include <ui_Lobby.h>

class Lobby : public QMainWindow
{
	Q_OBJECT

public:
	Lobby(QWidget* parent = 0);
	~Lobby();

public slots:

private:
	Ui::Lobby m_ui;
};
#endif // !LOBBY_H
