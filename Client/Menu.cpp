#include "Menu.h"
#include "Lobby.h"
#include "utils.h"
#include <crow.h>
#include <cpr/cpr.h>
#include <QTime>

/*
TODO:
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
	m_ui.errorLabel->show();
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

		// put player in the room


	}
	catch (const std::exception& exception)
	{
		m_ui.errorLabel->setText(exception.what());
		WaitForSeconds(5);
		m_ui.errorLabel->setText("");
	}
}
