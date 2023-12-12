#include "LobbyID.h"
#include <cpr/cpr.h>
#include <crow.h>
#include "Lobby.h"

LobbyID::LobbyID(QWidget* parent)
{
	m_ui.setupUi(this);
	connect(m_ui.JoinRoomButton, &QPushButton::clicked, this, &LobbyID::onJoinRoomButtonPress);
	//there is a button in LobbyID.ui called JoinRoomButton that should call a function called onJoinRoomButtonPress
}

LobbyID::~LobbyID()
{
}

void LobbyID::onJoinRoomButtonPress()
{
	//this function should get the roomID from the QtextEdit box and send it to the server , if it is valid it should open a new Lobby window , if not
	//it should show an error message and return to the Menu.ui window
	//it gets the roomID from the text box using m_ui.RoomID->text().toStdString() and it should send it to the server to verify it
	//if the roomID is valid it should open a new Lobby window and close the LobbyID window
	//RoomId is a QtextEdit in LobbyID.ui

	std::string roomID = m_ui.lineEdit->text().toUtf8().constData();

	cpr::Response response = cpr::Get(
		cpr::Url{ "http://localhost:18080/joinRoomID" },
		cpr::Payload{ {"RoomID",roomID}}
	);

	if (response.status_code != 200)
	{
		m_ui.lineEdit->clear();
		m_ui.lineEdit->setPlaceholderText("Invalid Room ID");
	}
	else {
		//open new Lobby window , should be modified in the future to connect to an existing lobby
		correctRoomIDFlag = true;
		Lobby* lobby = new Lobby(std::move(m_username), false);
		lobby->show();
		this->close();
	}


}

void LobbyID::setUserName(const std::string& username)
{
	m_username = username;
}

bool LobbyID::correctRoomID()
{
	return correctRoomIDFlag;
}
