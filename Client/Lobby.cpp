#include "Lobby.h"
#include "Game.h"
#include "Menu.h"

#include "utils.h"
#include <crow.h>
#include <cpr/cpr.h>


/*
TODO:
- connect to server
- set the owner of the room (the one that created it)
- create a room / save it in the database (Create Room button), send the room id to the server (can be created as a game)
- let other players join the room (max 4 players) with the same room id
- display the players in the room
- display the room id
- display the owner of the room

*/


Lobby::Lobby(const std::string& username, bool isOwner, QWidget* parent) :
	QMainWindow(parent), m_username(username), m_isOwner(isOwner), m_playerIndex(0)
{
	m_ui.setupUi(this);

	m_ui.player1_2->hide();
	m_ui.player2_2->hide();
	m_ui.player3_2->hide();
	m_ui.player4_2->hide();


	connect(m_ui.createLobby, SIGNAL(clicked()), this, SLOT(onCreateLobbyButtonPress()));
	connect(m_ui.startGame, SIGNAL(clicked()), this, SLOT(onStartGameButtonPress()));
	connect(m_ui.backButton, SIGNAL(clicked()), this, SLOT(onBackButtonPress()));
}

Lobby::~Lobby()
{
}

void Lobby::GetRoomID()
{
	cpr::Response response = cpr::Get(
		cpr::Url{ Server::GetUrl() + "/roomID" }
	);

	if (response.status_code != 200)
		throw std::exception(response.text.c_str());

	m_roomID = response.text;
	m_ui.roomIdField->setText(QString::fromUtf8(response.text.data(), int(response.text.size())));
}

void Lobby::DisplayPlayer(const std::string& username, int index)
{
	switch (index)
	{
	case 1:
		m_ui.player1_2->show();
		m_ui.player1_2->setText(QString::fromUtf8(username.data(), int(username.size())));
		m_ui.roomOwnerField_2->setText(QString::fromUtf8(username.data(), int(username.size())));
		break;
	case 2:
		m_ui.player2_2->show();
		m_ui.player2_2->setText(QString::fromUtf8(username.data(), int(username.size())));
		break;
	case 3:
		m_ui.player3_2->show();
		m_ui.player3_2->setText(QString::fromUtf8(username.data(), int(username.size())));
		break;
	case 4:
		m_ui.player4_2->show();
		m_ui.player4_2->setText(QString::fromUtf8(username.data(), int(username.size())));
		break;
	default:
		break;
	}
}

void Lobby::onCreateLobbyButtonPress()
{
	if (!m_isOwner)
		return;

	QString numberOfPlayers = m_ui.comboBox->itemText(m_ui.comboBox->currentIndex());
	GetRoomID();
	m_playerIndex = 1;
	m_ui.playerNumber->setText(numberOfPlayers);
	m_ui.roomOwnerField_2->setText(QString::fromUtf8(m_username.data(), int(m_username.size())));
	m_ui.roomIdField_2->setText(QString::fromUtf8(m_roomID.data(), int(m_roomID.size())));
	m_ui.player1_2->setText(QString::fromUtf8(m_username.data(), int(m_username.size())));
	m_ui.stackedWidget->setCurrentIndex(1);
	DisplayPlayer(m_username, m_playerIndex);

	// send request to server to create a room (game) with the room id, the max number of players and how many players are already in the room
	cpr::Response response = cpr::Get(
		cpr::Url{ Server::GetUrl() + "/createRoom" },
		cpr::Payload{
			{ "gameCode", m_roomID },
			{ "username", m_username },
			{ "maxPlayers", std::to_string(numberOfPlayers[0].digitValue()) },
			{ "currentPlayers", std::to_string(m_playerIndex + 1) }
		}
	);



}

void Lobby::onStartGameButtonPress()
{
	// send request to server to start the game 

	Game* game = new Game(std::move(m_username), m_isOwner, m_playerIndex);
	game->show();
	this->close();
	this->deleteLater();

}

void Lobby::onBackButtonPress()
{
	Menu* menuBar = new Menu(std::move(m_username));
	menuBar->show();
	this->close();
	this->deleteLater();
}
