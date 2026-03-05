#include "Menu.h"
#include "utils.h"
#include "RoutingManager.h"
#include <QTime>

Menu::Menu(const std::string& username, QWidget* parent)
	: QWidget(parent),
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

void Menu::OnCreateButtonClicked() noexcept
{
	// open new lobby
	emit NavigateToLobby(m_username, 1, true, "");
}

void Menu::OnJoinButtonClicked() noexcept
{
	m_ui.joinGame->show();
	m_ui.roomCode->show();
}

void Menu::OnJoinGameButtonClicked()
{
	std::string roomID = m_ui.roomCode->text().toUtf8().constData();

	try {
		if (roomID.empty()) {
			throw std::exception("Room ID cannot be empty");
		}

		// send request to server to check if the room exists
		cpr::Response response = RoutingManager::CheckRoomID(roomID);

		if (!Utils::IsResponseSuccessful(response)) {
			throw std::exception(response.text.c_str());
		}

		// get the number of players that are already in the room to put the player in the correct slot
		int playerIndex = std::stoi(response.text.c_str()) + 1;

		// put player in the room (game)
		auto req = RoutingManager::JoinRoom(roomID, m_username);

		if (!Utils::IsResponseSuccessful(req)) {
			throw std::exception(req.text.c_str());
		}

		// open new lobby for the player
		emit NavigateToLobby(m_username, playerIndex, false, roomID);
	} catch (const std::exception& exception) {
		m_ui.errorLabel->show();
		m_ui.errorLabel->setText(exception.what());
		Utils::WaitForSeconds(2);
		m_ui.errorLabel->setText("");
		m_ui.errorLabel->hide();
	}
}
