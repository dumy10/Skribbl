#include "RoomUpdateHandler.h"
#include "utils.h"

RoomUpdateHandler::RoomUpdateHandler(GameStateManager* stateManager, PlayerDisplayManager* playerDisplayManager, 
	ChatManager* chatManager, GameTimerManager* timerManager, DrawingToolsUIManager* drawingToolsManager,
	QLabel* wordLabel, QLabel* drawerLabel, QLabel* roundLabel, QWidget* drawingArea, QWidget* parent)
	: QObject(parent),
	m_stateManager(stateManager),
	m_playerDisplayManager(playerDisplayManager),
	m_chatManager(chatManager),
	m_timerManager(timerManager),
	m_wordLabel(wordLabel),
	m_drawerLabel(drawerLabel),
	m_roundLabel(roundLabel),
	m_drawingArea(drawingArea),
	m_parentWidget(parent)
{
	// Initialize UI configurator
	m_uiConfigurator = std::make_unique<GameUIConfigurator>(m_stateManager, m_chatManager, 
		drawingToolsManager, m_wordLabel, m_drawingArea);
}

void RoomUpdateHandler::SetNavigateToMenuCallback(std::function<void(const std::string&)> callback)
{
	m_navigateToMenuCallback = std::move(callback);
}

void RoomUpdateHandler::SetOnTimeEndCallback(std::function<void()> callback)
{
	m_onTimeEndCallback = std::move(callback);
}

void RoomUpdateHandler::SetEndGameCallback(std::function<void()> callback)
{
	m_endGameCallback = std::move(callback);
}

void RoomUpdateHandler::SetSendDrawingCallback(std::function<void()> callback)
{
	m_sendDrawingCallback = std::move(callback);
}

void RoomUpdateHandler::SetClearDrawingCallback(std::function<void()> callback)
{
	m_clearDrawingCallback = std::move(callback);
}

void RoomUpdateHandler::SetGetDrawingWidgetCallback(std::function<DrawingWidget* ()> callback)
{
	m_getDrawingWidgetCallback = std::move(callback);
}

void RoomUpdateHandler::SetUpdateWidgetCallback(std::function<void()> callback)
{
	m_updateWidgetCallback = std::move(callback);
}

bool RoomUpdateHandler::HandleGameEnded(const RoomUpdateData& data)
{
	const bool onlyPlayerLeft = data.currentPlayerCount == 1;
	const bool gameEnded = data.gameEnded || onlyPlayerLeft;

	if (onlyPlayerLeft && m_endGameCallback) {
		m_endGameCallback();
	}

	if (gameEnded) {
		m_drawerLabel->setText(onlyPlayerLeft ? "You are the only player left in the room" : "Game has ended");

		// Update UI for countdown
		m_chatManager->SetInputReadOnly(true);
		m_drawingArea->setEnabled(false);
		m_wordLabel->setText("Going back to the menu in 10 seconds");

		m_timerManager->StartCountdownTimer(COUNTDOWN_SECONDS, "Going back to the menu in 10 seconds", [this]() {
			if (m_navigateToMenuCallback) {
				m_navigateToMenuCallback(m_stateManager->GetUsername());
			}
			if (m_clearDrawingCallback) {
				m_clearDrawingCallback();
			}
			if (m_stateManager->IsOwner() && m_endGameCallback) {
				m_endGameCallback();
			}
			});
		return true;
	}

	return false;
}

void RoomUpdateHandler::UpdateDrawingPlayerAndWord(const RoomUpdateData& data)
{
	// Update drawing player
	if (!data.drawingPlayer.empty()) {
		m_drawerLabel->setText(Utils::ToQString(data.drawingPlayer) + " is currently drawing");
		m_stateManager->SetDrawing(m_stateManager->GetUsername() == data.drawingPlayer);
	}

	// Update current word
	if (!data.currentWord.empty()) {
		const QString word = Utils::ToQString(data.currentWord);
		m_stateManager->IsDrawing() ? m_uiConfigurator->ConfigureForDrawer(word) : m_uiConfigurator->ConfigureForGuesser(word);
	}
}

void RoomUpdateHandler::HandleTimeLeft(const RoomUpdateData& data)
{
	if (data.timeLeft.empty()) {
		return;
	}

	m_timerManager->UpdateTimeDisplay(Utils::ToQString(data.timeLeft));

	if (std::stoi(data.timeLeft) == 0) {
		m_stateManager->ResetGuessedWord();
		if (m_clearDrawingCallback) {
			m_clearDrawingCallback();
		}
		if (m_stateManager->IsOwner() && m_onTimeEndCallback) {
			m_onTimeEndCallback();
		}
	}
}

void RoomUpdateHandler::UpdateDrawingImage(const RoomUpdateData& data)
{
	// Use data.timeLeft instead of current display time to stay synchronized with server
	const int timeLeft = data.timeLeft.empty() ? 0 : std::stoi(data.timeLeft);

	if (timeLeft != 0) {
		if (m_stateManager->IsDrawing()) {
			if (m_sendDrawingCallback) {
				m_sendDrawingCallback();
			}
		}
		else if (!data.drawingImage.empty()) {
			const QByteArray byteArray = QByteArray::fromBase64(data.drawingImage.c_str());
			QImage receivedImage{ 621, 491, QImage::Format_ARGB32 };
			receivedImage.loadFromData(byteArray, "PNG");

			if (m_getDrawingWidgetCallback) {
				if (auto* drawingArea = m_getDrawingWidgetCallback()) {
					drawingArea->SetImage(receivedImage);
				}
			}
			if (m_updateWidgetCallback) {
				m_updateWidgetCallback();
			}
		}
	}
	// Note: Drawing is cleared in HandleTimeLeft when time reaches 0
}

void RoomUpdateHandler::HandleAllPlayersGuessed(const RoomUpdateData& data)
{
	if (!data.allPlayersGuessed) {
		return;
	}

	// Update UI for countdown
	m_chatManager->SetInputReadOnly(true);
	m_drawingArea->setEnabled(false);
	m_wordLabel->setText("All players guessed. Moving to next round in 10 seconds.");

	m_timerManager->StartCountdownTimer(COUNTDOWN_SECONDS, "All players guessed. Moving to next round in 10 seconds.", [this]() {
		if (m_onTimeEndCallback) {
			m_onTimeEndCallback();
		}
		if (m_clearDrawingCallback) {
			m_clearDrawingCallback();
		}
		m_stateManager->ResetGuessedWord();
		m_timerManager->StartUpdateTimer(UPDATE_INTERVAL_MS);
		});
}

void RoomUpdateHandler::HandleRoomUpdate(const RoomUpdateData& data)
{
	// Check and handle game ended
	if (HandleGameEnded(data)) {
		return;
	}

	// Update drawing player and word display
	UpdateDrawingPlayerAndWord(data);

	// Update round number
	if (!data.roundNumber.empty()) {
		QString roundNumber = "Round: " + Utils::ToQString(data.roundNumber);
		m_roundLabel->setText(roundNumber);
	}

	// Update time left and handle time end
	HandleTimeLeft(data);

	// Update chat
	if (!data.chat.empty()) {
		m_chatManager->UpdateChat(data.chat);
	}

	// Update drawing image
	UpdateDrawingImage(data);

	// Update players and scores
	if (!data.players.empty()) {
		std::vector<std::string> players = split(data.players, ",");
		if (!players.empty()) {
			m_playerDisplayManager->UpdatePlayerList(players, data.playerScores);
		}
	}

	// Check and handle all players guessed
	HandleAllPlayersGuessed(data);
}
