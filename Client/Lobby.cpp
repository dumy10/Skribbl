#include "Lobby.h"
#include <crow.h>
#include <cpr/cpr.h>


Lobby::Lobby(const std::string& username, QWidget* parent) :
	QMainWindow(parent), m_username(username)
{
	m_ui.setupUi(this);
	m_ui.player1->setText(QString::fromUtf8(username.data(), int(username.size())));
	m_ui.roomOwnerField->setText(QString::fromUtf8(username.data(), int(username.size())));
	getRoomID();
}

Lobby::~Lobby()
{
}

void Lobby::getRoomID()
{
	cpr::Response response = cpr::Get(
		cpr::Url{ "http://localhost:18080/roomID" }
	);

	if (response.status_code != 200)
		throw std::exception(response.text.c_str());

	m_ui.roomIdField->setText(QString::fromUtf8(response.text.data(), int(response.text.size())));
}

