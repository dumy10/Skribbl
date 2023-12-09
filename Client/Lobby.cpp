#include "Lobby.h"


Lobby::Lobby(const std::string& username, QWidget* parent) :
	QMainWindow(parent), m_username(username)
{
	m_ui.setupUi(this);
	m_ui.player1->setText(QString::fromUtf8(username.data(), int(username.size())));
	m_ui.roomOwnerField->setText(QString::fromUtf8(username.data(), int(username.size())));
}

Lobby::~Lobby()
{
}