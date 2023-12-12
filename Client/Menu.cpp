#include "Menu.h"
#include "Lobby.h"
#include"LobbyID.h"

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
	connect(m_ui.createRoom, SIGNAL(clicked()), this, SLOT(onCreateButtonClicked()));
	connect(m_ui.joinRoom, SIGNAL(clicked()), this, SLOT(onJoinButtonClicked()));
}

//consructor of menu with string parameters, to be used with move semantics


Menu::~Menu()
{}

void Menu::onCreateButtonClicked()
{
	// open new lobby
	Lobby* lobby = new Lobby(std::move(m_username), true);
	lobby->show();
	this->close();
}

void Menu::onJoinButtonClicked()
{
	LobbyID* lobbyID = new LobbyID();
	lobbyID->show();
	lobbyID->setUserName(m_username);

	this->close();
}
