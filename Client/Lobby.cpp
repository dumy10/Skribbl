#include "Lobby.h"
#include "utils.h"
#include "RoutingManager.h"
#include <QTime>

Lobby::Lobby(const std::string& username, int playerIndex, bool isOwner, const std::string& roomID, QWidget* parent) :
	QWidget(parent), m_username(username), m_isOwner(isOwner), m_playerIndex(playerIndex), m_roomID(roomID)
{
	m_ui.setupUi(this);

	m_ui.player1_2->hide();
	m_ui.player2_2->hide();
	m_ui.player3_2->hide();
	m_ui.player4_2->hide();
	m_ui.errorLabel->hide();
	m_ui.startGame->hide();


	if (playerIndex != 1) {
		m_ui.stackedWidget->setCurrentIndex(1);
		DisplayRoomInformation();
	}

	DisplayPlayer(m_username, m_playerIndex);
	m_updateTimer = std::make_unique<QTimer>(this);

	if (!m_isOwner) {
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

void Lobby::StopTimer()
{
	if (m_updateTimer && m_updateTimer->isActive()) {
		m_updateTimer->stop();
	}
}

void Lobby::GetRoomID()
{
	cpr::Response response = RoutingManager::GetNewRoomID();

	if (response.status_code != 201) {
		throw std::exception(response.text.c_str());
	}

	m_roomID = response.text;
	m_ui.roomIdField->setText(QString::fromUtf8(response.text.data(), int(response.text.size())));
}

void Lobby::DisplayPlayer(const std::string& username, int index) noexcept
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

void Lobby::DisplayPlayerCount(int count) noexcept
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

void Lobby::DisplayRoomInformation() noexcept
{
	auto req = RoutingManager::GetRoomPlayers(m_roomID);

	m_ui.roomIdField_2->setText(QString::fromUtf8(m_roomID.data(), int(m_roomID.size())));

	auto req2 = RoutingManager::GetNumberOfPlayers(m_roomID);

	m_ui.playerNumber->setText(QString::fromUtf8(req2.text.data(), int(req2.text.size())));
	m_ui.playerNumber->setText(m_ui.playerNumber->text() + " Players");
	std::vector<std::string> players = split(req.text, ",");

	for (int i = 0; i < players.size(); i++) {
		DisplayPlayer(players[i], i + 1);
	}

}

void Lobby::StartTimer()
{
	m_updateTimer->start(1000);
}

void Lobby::closeEvent(QCloseEvent* event)
{
	emit PlayerLeft();

	QWidget::closeEvent(event);
}

void Lobby::UpdateRoomInformation() noexcept
{
	auto req = RoutingManager::GetRoomPlayers(m_roomID);

	if (req.status_code != 200) {
		return;
	}

	std::vector<std::string> players = split(req.text, ",");

	if (players.empty()) {
		return;
	}

	if (players.size() != 1 && m_isOwner) {
		m_ui.startGame->show();
	} else {
		m_ui.startGame->hide();
	}

	DisplayPlayerCount(players.size());

	for (int i = 0; i < players.size(); i++) {
		DisplayPlayer(players[i], i + 1);
	}
}

void Lobby::OnPlayerLeft() const noexcept
{
	// send request to server to remove the player from the room (game)
	auto req = RoutingManager::LeaveRoom(m_roomID, m_username);
}

void Lobby::CheckGameStarted()
{
	auto request = RoutingManager::CheckGameStarted(m_roomID);

	if (request.status_code != 200) {
		return;
	}

	emit NavigateToGame(m_username, m_playerIndex, m_isOwner, m_roomID);
}

void Lobby::OnCreateLobbyButtonPress()
{
	try {
		QString numberOfPlayers = m_ui.comboBox->itemText(m_ui.comboBox->currentIndex());
		GetRoomID();
		m_ui.playerNumber->setText(numberOfPlayers);

		// send request to server to create a room (game) with the room id, the max number of players and how many players are already in the room
		cpr::Response response = RoutingManager::CreateRoom(
			m_roomID,
			m_username,
			std::to_string(numberOfPlayers[0].digitValue()),
			std::to_string(m_playerIndex)
		);

		if (response.status_code != 201) {
			throw std::exception(response.text.c_str());
		}

		m_ui.roomOwnerField_2->setText(QString::fromUtf8(m_username.data(), int(m_username.size())));
		m_ui.roomIdField_2->setText(QString::fromUtf8(m_roomID.data(), int(m_roomID.size())));
		m_ui.stackedWidget->setCurrentIndex(1);
		StartTimer();
	} catch (const std::exception& exception) {
		m_ui.errorLabel->show();
		m_ui.errorLabel->setText(exception.what());
		Utils::WaitForSeconds(5);
		m_ui.errorLabel->setText("");
		m_ui.errorLabel->hide();
	}
}

void Lobby::OnStartGameButtonPress()
{
	auto req = RoutingManager::GetCurrentNumberOfPlayers(m_roomID);

	if (req.status_code != 200) {
		return;
	}

	int numberOfPlayers = std::stoi(req.text);

	if (numberOfPlayers <= 1) {
		return;
	}

	// send request to server to start the game for all the players in the room
	auto request = RoutingManager::StartGame(m_roomID);

	if (request.status_code != 200) {
		return;
	}

	emit NavigateToGame(m_username, m_playerIndex, m_isOwner, m_roomID);
}

void Lobby::OnBackButtonPress() noexcept
{
	emit NavigateToMenu(m_username);
}
