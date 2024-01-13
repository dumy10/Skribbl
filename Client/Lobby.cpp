#include "Lobby.h"
#include "Game.h"
#include "Menu.h"

#include "utils.h"
#include <crow.h>
#include <cpr/cpr.h>
#include <QTime>

Lobby::Lobby(const std::string& username, int playerIndex, bool isOwner, const std::string& roomID, QWidget* parent) :
	QMainWindow(parent), m_username(username), m_isOwner(isOwner), m_playerIndex(playerIndex), m_roomID(roomID)
{
	m_ui.setupUi(this);

	m_ui.player1_2->hide();
	m_ui.player2_2->hide();
	m_ui.player3_2->hide();
	m_ui.player4_2->hide();
	m_ui.errorLabel->hide();
	m_ui.startGame->hide();


	if (playerIndex != 1)
	{
		m_ui.stackedWidget->setCurrentIndex(1);
		DisplayRoomInformation();
	}

	DisplayPlayer(m_username, m_playerIndex);
	m_updateTimer = std::make_unique<QTimer>(this);

	if (!m_isOwner)
	{
		StartTimer();
		connect(m_updateTimer.get(), SIGNAL(timeout()), this, SLOT(CheckGameStarted()));
	}

	connect(m_ui.createLobby, SIGNAL(clicked()), this, SLOT(OnCreateLobbyButtonPress()));
	connect(m_ui.startGame, SIGNAL(clicked()), this, SLOT(OnStartGameButtonPress()));
	connect(m_ui.backButton, SIGNAL(clicked()), this, SLOT(OnBackButtonPress()));
	connect(m_updateTimer.get(), SIGNAL(timeout()), this, SLOT(UpdateRoomInformation()));
	connect(this, SIGNAL(PlayerLeft()), this, SLOT(OnPlayerLeft()));
}

Lobby::~Lobby()
{
	m_updateTimer->stop();
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

void Lobby::DisplayPlayerCount(int count)
{
	switch (count)
	{
	case 1:
		m_ui.player1_2->show();
		m_ui.player2_2->hide();
		m_ui.player3_2->hide();
		m_ui.player4_2->hide();
		break;
	case 2:
		m_ui.player1_2->show();
		m_ui.player2_2->show();
		m_ui.player3_2->hide();
		m_ui.player4_2->hide();
		break;
	case 3:
		m_ui.player1_2->show();
		m_ui.player2_2->show();
		m_ui.player3_2->show();
		m_ui.player4_2->hide();
		break;
	case 4:
		m_ui.player1_2->show();
		m_ui.player2_2->show();
		m_ui.player3_2->show();
		m_ui.player4_2->show();
		break;
	default:
		break;
	}
}

void Lobby::DisplayRoomInformation()
{
	auto req = cpr::Get(
		cpr::Url{ Server::GetUrl() + "/roomPlayers" },
		cpr::Payload{ {"roomID", m_roomID} }
	);

	m_ui.roomIdField_2->setText(QString::fromUtf8(m_roomID.data(), int(m_roomID.size())));

	auto req2 = cpr::Get(
		cpr::Url{ Server::GetUrl() + "/numberOfPlayers" },
		cpr::Payload{ {"roomID", m_roomID} }
	);

	m_ui.playerNumber->setText(QString::fromUtf8(req2.text.data(), int(req2.text.size())));
	m_ui.playerNumber->setText(m_ui.playerNumber->text() + " Players");
	std::vector<std::string> players = split(req.text, ",");

	for (int i = 0; i < players.size(); i++)
		DisplayPlayer(players[i], i + 1);

}

void Lobby::WaitForSeconds(int seconds)
{
	QTime delayTime = QTime::currentTime().addSecs(seconds);
	while (QTime::currentTime() < delayTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void Lobby::StartTimer()
{
	m_updateTimer->start(1000);
}

void Lobby::closeEvent(QCloseEvent* event)
{
	emit PlayerLeft();

	QMainWindow::closeEvent(event);
}

void Lobby::UpdateRoomInformation()
{
	auto req = cpr::Get(
		cpr::Url{ Server::GetUrl() + "/roomPlayers" },
		cpr::Payload{ {"roomID", m_roomID} }
	);

	if(req.status_code != 200)
		return;

	std::vector<std::string> players = split(req.text, ",");

	if (players.empty())
		return;

	if (players.size() != 1 && m_isOwner)
		m_ui.startGame->show();
	else
		m_ui.startGame->hide();

	DisplayPlayerCount(players.size());

	for (int i = 0; i < players.size(); i++)
		DisplayPlayer(players[i], i + 1);
}

void Lobby::OnPlayerLeft()
{
	// send request to server to remove the player from the room (game)
	auto req = cpr::Post(
		cpr::Url{ Server::GetUrl() + "/leaveRoom" },
		cpr::Payload{
			{"roomID", m_roomID},
			{"username", m_username}
		}
	);
}

void Lobby::CheckGameStarted()
{
	auto request = cpr::Get(
		cpr::Url{ Server::GetUrl() + "/gameStarted" },
		cpr::Payload{ {"roomID", m_roomID} }
	);

	if (request.status_code != 200)
		return;

	Game* game = new Game(std::move(m_username), m_playerIndex, m_isOwner, m_roomID);
	game->show();
	this->hide();
	this->deleteLater();
}

void Lobby::OnCreateLobbyButtonPress()
{
	try
	{
		QString numberOfPlayers = m_ui.comboBox->itemText(m_ui.comboBox->currentIndex());
		GetRoomID();
		m_ui.playerNumber->setText(numberOfPlayers);

		// send request to server to create a room (game) with the room id, the max number of players and how many players are already in the room
		cpr::Response response = cpr::Get(
			cpr::Url{ Server::GetUrl() + "/createRoom" },
			cpr::Payload{
				{ "gameCode", m_roomID },
				{ "username", m_username },
				{ "maxPlayers", std::to_string(numberOfPlayers[0].digitValue()) },
				{ "currentPlayers", std::to_string(m_playerIndex) }
			}
		);

		if (response.status_code != 200)
			throw std::exception(response.text.c_str());

		m_ui.roomOwnerField_2->setText(QString::fromUtf8(m_username.data(), int(m_username.size())));
		m_ui.roomIdField_2->setText(QString::fromUtf8(m_roomID.data(), int(m_roomID.size())));
		m_ui.stackedWidget->setCurrentIndex(1);
		StartTimer();
	}
	catch (const std::exception& exception)
	{
		m_ui.errorLabel->show();
		m_ui.errorLabel->setText(exception.what());
		WaitForSeconds(5);
		m_ui.errorLabel->setText("");
		m_ui.errorLabel->hide();
	}
}

void Lobby::OnStartGameButtonPress()
{
	auto req = cpr::Get(
		cpr::Url{ Server::GetUrl() + "/currentNumberOfPlayers" },
		cpr::Payload{ {"roomID", m_roomID} }
	);

	if (req.status_code != 200)
		return;
	int numberOfPlayers = std::stoi(req.text);

	if (numberOfPlayers > 1)
	{

		// send request to server to start the game for all the players in the room
		auto request = cpr::Post(
			cpr::Url{ Server::GetUrl() + "/startGame" },
			cpr::Payload{ {"roomID", m_roomID} }
		);

		if (request.status_code != 200)
			return;

		Game* game = new Game(std::move(m_username), m_playerIndex, m_isOwner, m_roomID);
		game->show();
		this->hide();
		this->deleteLater();
	}
}

void Lobby::OnBackButtonPress()
{
	Menu* menuBar = new Menu(std::move(m_username));
	menuBar->show();
	this->close();
	this->deleteLater();
}
