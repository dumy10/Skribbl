#include "Game.h"
#include "utils.h"
#include "RoutingManager.h"
#include "NetworkWorker.h"
#include "ConnectionSetup.h"

#include <QScrollBar>
#include <QBuffer>
#include <QCheckBox>
#include <ranges>

Game::Game(const std::string& username, int playerIndex, bool isOwner, const std::string& m_roomID, QWidget* parent)
	: QWidget(parent)
{
	// Register custom types for signal/slot communication across threads
	qRegisterMetaType<RoomUpdateData>("RoomUpdateData");
	qRegisterMetaType<QImage>("QImage");
	qRegisterMetaType<QColor>("QColor");

	m_ui.setupUi(this);

	// Initialize managers
	m_stateManager = std::make_unique<GameStateManager>(username, playerIndex, isOwner, m_roomID);
	InitializeManagers();

	// Initialize network worker thread
	m_workerThread = new QThread(this);
	m_networkWorker = new NetworkWorker(m_stateManager->GetRoomID(), m_stateManager->GetUsername());
	m_networkWorker->moveToThread(m_workerThread);

	// Setup all connections
	SetupConnections();

	// Start the worker thread and timer
	m_workerThread->start();
	m_timerManager->StartUpdateTimer(UPDATE_INTERVAL_MS);

	// Initialize UI
	m_playerDisplayManager->HidePlayers();
	m_playerDisplayManager->DisplayPlayer(m_stateManager->GetUsername(), m_stateManager->GetPlayerIndex(), "0");
}

Game::~Game()
{
	StopTimer();

	// Clean up worker thread properly
	if (m_workerThread && m_workerThread->isRunning()) {
		m_workerThread->quit();
		m_workerThread->wait();
	}

	// Now safe to delete the worker since thread has stopped
	delete m_networkWorker;
	m_networkWorker = nullptr;

	auto clearImageRequest = RoutingManager::ClearDrawingImage(m_stateManager->GetRoomID());
}

void Game::StopTimer()
{
	if (m_timerManager) {
		m_timerManager->StopUpdateTimer();
	}
}

void Game::InitializeManagers()
{
	// Store UI element references
	m_wordLabel = m_ui.wordLabel;
	m_drawerLabel = m_ui.drawerLabel;
	m_roundLabel = m_ui.roundLabel;
	m_drawingArea = m_ui.drawingArea;

	std::array<QLabel*, 4> nameLabels = { m_ui.player1_3, m_ui.player2_3, m_ui.player3_3, m_ui.player4_3 };
	std::array<QLabel*, 4> scoreLabels = { m_ui.player1_score, m_ui.player2_score, m_ui.player3_score, m_ui.player4_score };
	m_playerDisplayManager = std::make_unique<PlayerDisplayManager>(nameLabels, scoreLabels);

	m_chatManager = std::make_unique<ChatManager>(m_ui.chat, m_ui.textEdit);

	auto* drawingWidget = GetDrawingWidget();
	m_drawingToolsManager = std::make_unique<DrawingToolsUIManager>(drawingWidget, this);

	m_timerManager = std::make_unique<GameTimerManager>(m_ui.timer, this);

	// Initialize room update handler
	m_roomUpdateHandler = std::make_unique<RoomUpdateHandler>(m_stateManager.get(), m_playerDisplayManager.get(), 
		m_chatManager.get(), m_timerManager.get(), m_drawingToolsManager.get(),
		m_wordLabel, m_drawerLabel, m_roundLabel, m_drawingArea, this);

	// Set up callbacks for actions that need Game class context
	m_roomUpdateHandler->SetNavigateToMenuCallback([this](const std::string& username) { emit NavigateToMenu(username); });
	m_roomUpdateHandler->SetOnTimeEndCallback([this]() { OnTimeEnd(); });
	m_roomUpdateHandler->SetEndGameCallback([this]() { EndGame(); });
	m_roomUpdateHandler->SetSendDrawingCallback([this]() { SendCurrentDrawing(); });
	m_roomUpdateHandler->SetClearDrawingCallback([this]() { ClearDrawing(); });
	m_roomUpdateHandler->SetGetDrawingWidgetCallback([this]() {	return GetDrawingWidget();	});
	m_roomUpdateHandler->SetUpdateWidgetCallback([this]() {	update(); });
}

void Game::SetupConnections()
{
	// Initialize connection setup helper (after network worker is created)
	m_connectionSetup = std::make_unique<ConnectionSetup>(m_ui, this, m_networkWorker, m_timerManager.get(), m_drawingToolsManager.get());
	m_connectionSetup->SetupAllConnections();
}

void Game::OnSendButtonClicked()
{
	QString text = m_chatManager->GetInputText();
	if (text.isEmpty() || text.size() < 3) {
		return;
	}

	// Send message asynchronously through worker
	QMetaObject::invokeMethod(m_networkWorker, [this, text]() { m_networkWorker->SendMessage(text);	}, Qt::QueuedConnection);

	m_ui.chat->ensureCursorVisible();
	m_chatManager->ClearInput();
}

void Game::OnUpdateTimerTick()
{
	// Simply trigger the worker to fetch data in the background
	QMetaObject::invokeMethod(m_networkWorker, &NetworkWorker::FetchRoomUpdate, Qt::QueuedConnection);
}

void Game::OnPlayerQuit()
{
	cpr::Response request = RoutingManager::CheckGameEnded(m_stateManager->GetRoomID());

	if (Utils::IsResponseSuccessful(request)) {
		return;
	}

	RoutingManager::LeaveRoom(m_stateManager->GetRoomID(), m_stateManager->GetUsername());
	ClearDrawing();
}

void Game::OnLeaveButtonClicked()
{
	cpr::Response request = RoutingManager::CheckGameEnded(m_stateManager->GetRoomID());

	if (request.status_code == 200) {
		emit NavigateToMenu(m_stateManager->GetUsername());
		return;
	}

	// Send request to server to remove the player from the room (game) - async
	QMetaObject::invokeMethod(m_networkWorker, &NetworkWorker::LeaveRoom, Qt::QueuedConnection);

	emit NavigateToMenu(m_stateManager->GetUsername());
	ClearDrawing();
}

void Game::OnTimeEnd()
{
	if (!m_stateManager->IsOwner()) {
		return;
	}

	QMetaObject::invokeMethod(m_networkWorker, &NetworkWorker::StartNextRound, Qt::QueuedConnection);
	ClearDrawing();
}

void Game::EndGame() const noexcept
{
	auto endGameRequest = RoutingManager::EndGame(m_stateManager->GetRoomID());
}

DrawingWidget* Game::GetDrawingWidget() const noexcept
{
	return qobject_cast<DrawingWidget*>(m_ui.drawingArea);
}

void Game::ClearDrawing() noexcept
{
	m_drawingToolsManager->ClearDrawing();
	m_drawingToolsManager->ClearUndoHistory();
	m_lastSentDrawingHash = 0;  // Reset cached drawing hash
	
	// Ensure the widget is updated to show the cleared state
	if (auto* drawingArea = GetDrawingWidget()) {
		drawingArea->update();
	}
}

void Game::SendCurrentDrawing() const noexcept
{
	auto* drawingArea = GetDrawingWidget();
	if (!drawingArea) {
		return;
	}

	// Make a copy of the image to pass to worker thread
	QImage image = drawingArea->GetImage().copy();
	if (image.isNull()) {
		return;
	}

	// Encode to PNG and calculate hash
	QByteArray byteArray;
	QBuffer buffer(&byteArray);
	buffer.open(QIODevice::WriteOnly);
	image.save(&buffer, "PNG");

	// Calculate hash of the byte array
	const size_t currentHash = qHash(byteArray);

	// Only send if the image has changed
	if (m_lastSentDrawingHash != 0 && currentHash == m_lastSentDrawingHash) {
		return;
	}

	// Update last sent hash
	m_lastSentDrawingHash = currentHash;

	// Send image to worker - encoding already done
	QMetaObject::invokeMethod(m_networkWorker, [this, image]() { m_networkWorker->SendDrawingData(image); }, Qt::QueuedConnection);
}

void Game::OnRoomUpdateReceived(const RoomUpdateData& data)
{
	m_roomUpdateHandler->HandleRoomUpdate(data);
}

void Game::OnMessageSent(bool success, bool correctGuess)
{
	if (!success) {
		return;
	}

	if (correctGuess) {
		m_stateManager->SetGuessedWord(true);
	}
}
