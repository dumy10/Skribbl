#include "Menu.h"
#include "Lobby.h"
#include "utils.h"
#include <crow.h>
#include <cpr/cpr.h>

/*
TODO:
- connect to server
- create a room / save it in the database (Create Room button), send the room id to the server (can be created as a game)
- let other players join the room (max 4 players) with the same room id if the room is not full and the room id is correct
- add player to the room and send the player name to the server
*/

Menu::Menu(const std::string& username, QWidget* parent)
	: QMainWindow(parent),
	m_username(username)
{
	m_ui.setupUi(this);

	m_ui.roomCode->hide();
	m_ui.joinGame->hide();
	m_ui.errorLabel->hide();

	connect(m_ui.createRoom, SIGNAL(clicked()), this, SLOT(onCreateButtonClicked()));
	connect(m_ui.joinRoom, SIGNAL(clicked()), this, SLOT(onJoinButtonClicked()));
	connect(m_ui.joinGame, SIGNAL(clicked()), this, SLOT(onJoinGameButtonClicked()));
}

Menu::~Menu()
{

}

void Menu::onCreateButtonClicked()
{
	// open new lobby
	Lobby* lobby = new Lobby(std::move(m_username), true);
	lobby->show();
	this->close();
	this->deleteLater();

}

void Menu::onJoinButtonClicked()
{
	m_ui.joinGame->show();
	m_ui.roomCode->show();
	m_ui.errorLabel->show();
}

void Menu::onJoinGameButtonClicked()
{
	std::string roomID = m_ui.roomCode->text().toUtf8().constData();

	try
	{
		if (roomID == "")
			throw std::exception("Room ID cannot be empty");

		// send request to server to check if the room exists
	
		// put player in the room


	}
	catch (const std::exception& exception)
	{
		m_ui.errorLabel->setText(exception.what());
	}



}
