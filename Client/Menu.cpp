#include "Menu.h"
#include "Lobby.h"
#include "utils.h"
#include <crow.h>
#include <cpr/cpr.h>
#include <QTime>

Menu::Menu(const std::string& username, QWidget* parent)
	: QMainWindow(parent),
	m_username(username)
{
	m_ui.setupUi(this);

	m_ui.roomCode->hide();
	m_ui.joinGame->hide();
	m_ui.errorLabel->hide();

	connect(m_ui.createRoom, SIGNAL(clicked()), this, SLOT(OnCreateButtonClicked()));
	connect(m_ui.joinRoom, SIGNAL(clicked()), this, SLOT(OnJoinButtonClicked()));
	connect(m_ui.joinGame, SIGNAL(clicked()), this, SLOT(OnJoinGameButtonClicked()));
}

Menu::~Menu()
{

}

void Menu::OnCreateButtonClicked()
{
	// open new lobby
	Lobby* lobby = new Lobby(std::move(m_username), 1, true);
	lobby->show();
	this->close();
	this->deleteLater();

}

void Menu::WaitForSeconds(int seconds)
{
	QTime delayTime = QTime::currentTime().addSecs(seconds);
	while (QTime::currentTime() < delayTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void Menu::OnJoinButtonClicked()
{
	m_ui.joinGame->show();
	m_ui.roomCode->show();
}

void Menu::OnJoinGameButtonClicked()
{
	std::string roomID = m_ui.roomCode->text().toUtf8().constData();

	try
	{
		if (roomID == "")
			throw std::exception("Room ID cannot be empty");

		// send request to server to check if the room exists
		cpr::Response response = cpr::Get(
			cpr::Url{ Server::GetUrl() + "/checkRoomID" },
			cpr::Payload{ {"roomID", roomID} }
		);

		if (response.status_code != 200)
			throw std::exception(response.text.c_str());

		// get the number of players that are already in the room to put the player in the correct slot
		int playerIndex = std::stoi(response.text.c_str()) + 1;

		// put player in the room (game)
		auto req = cpr::Post(
			cpr::Url{ Server::GetUrl() + "/joinRoom" },
			cpr::Payload{
				{"roomID", roomID},
				{"username", m_username},
				{"currentPlayers", std::to_string(playerIndex)}
			}
		);

		if(req.status_code != 200)
			throw std::exception(req.text.c_str());

		// open new lobby for the player
		Lobby* lobby = new Lobby(std::move(m_username), playerIndex, false, roomID);
		lobby->show();
		this->close();
		this->deleteLater();
	}
	catch (const std::exception& exception)
	{
		m_ui.errorLabel->show();
		m_ui.errorLabel->setText(exception.what());
		WaitForSeconds(2);
		m_ui.errorLabel->setText("");
		m_ui.errorLabel->hide();
	}
}
