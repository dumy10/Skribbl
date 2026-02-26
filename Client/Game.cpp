#include "Game.h"
#include "utils.h"
#include "RoutingManager.h"
#include "NetworkWorker.h"

#include <QScrollBar>
#include <QBuffer>
#include <ranges>

const std::unordered_map<QString, QColor> Game::COLOR_PALETTE = {
	{"green", Qt::green},
	{"red", Qt::red},
	{"blue", Qt::blue},
	{"orange", QColor(255, 165, 0)},
	{"brown", QColor(165, 42, 42)},
	{"purple", QColor(128, 0, 128)},
	{"black", QColor(0, 0, 0)},
	{"white", QColor(255, 255, 255)},
	{"yellow", QColor(255, 255, 0)},
	{"grey", QColor(128, 128, 128)},
	{"turquoise", QColor(64, 224, 208)},
	{"pink", QColor(255, 192, 203)}
};

Game::Game(const std::string& username, int playerIndex, bool isOwner, const std::string& m_roomID, QWidget* parent)
	: QWidget(parent), m_username(username), m_isOwner(isOwner), m_playerIndex(playerIndex), m_roomID(m_roomID)
{
	// Register custom types for signal/slot communication across threads
	qRegisterMetaType<RoomUpdateData>("RoomUpdateData");
	qRegisterMetaType<QImage>("QImage");
	
	m_drawingArea = std::make_unique<DrawingWidget>(this);
	m_ui.setupUi(this);

	HidePlayers();
	DisplayPlayer(m_username, m_playerIndex, "0");

	m_updateTimer = std::make_unique<QTimer>(this);

	m_guessedWord = false;
	m_currentBrushSizeIndex = 0;
	
	// Initialize network worker thread
	m_workerThread = new QThread(this);
	m_networkWorker = new NetworkWorker(m_roomID, m_username);
	m_networkWorker->moveToThread(m_workerThread);
	
	// Connect worker signals
	connect(m_networkWorker, &NetworkWorker::RoomUpdateReceived, this, &Game::OnRoomUpdateReceived);
	connect(m_networkWorker, &NetworkWorker::MessageSent, this, &Game::OnMessageSent);
	connect(m_networkWorker, &NetworkWorker::PlayerScoreReceived, this, &Game::OnPlayerScoreReceived);
	
	// Start the worker thread
	m_workerThread->start();
	
	StartTimer();

	connect(m_ui.Clear, &QPushButton::clicked, this, &Game::ClearDrawingArea);
	connect(m_ui.Verde, &QPushButton::clicked, this, [this]() { SetPenColor(COLOR_PALETTE.at("green")); });
	connect(m_ui.Rosu, &QPushButton::clicked, this, [this]() { SetPenColor(COLOR_PALETTE.at("red")); });
	connect(m_ui.Albastru, &QPushButton::clicked, this, [this]() { SetPenColor(COLOR_PALETTE.at("blue")); });
	connect(m_ui.Orange, &QPushButton::clicked, this, [this]() { SetPenColor(COLOR_PALETTE.at("orange")); });
	connect(m_ui.Brown, &QPushButton::clicked, this, [this]() { SetPenColor(COLOR_PALETTE.at("brown")); });
	connect(m_ui.Purple, &QPushButton::clicked, this, [this]() { SetPenColor(COLOR_PALETTE.at("purple")); });
	connect(m_ui.White, &QPushButton::clicked, this, [this]() { SetPenColor(COLOR_PALETTE.at("white")); });
	connect(m_ui.Black, &QPushButton::clicked, this, [this]() { SetPenColor(COLOR_PALETTE.at("black")); });
	connect(m_ui.Grey, &QPushButton::clicked, this, [this]() { SetPenColor(COLOR_PALETTE.at("grey")); });
	connect(m_ui.Yellow, &QPushButton::clicked, this, [this]() { SetPenColor(COLOR_PALETTE.at("yellow")); });
	connect(m_ui.Pink, &QPushButton::clicked, this, [this]() { SetPenColor(COLOR_PALETTE.at("pink")); });
	connect(m_ui.Turquoise, &QPushButton::clicked, this, [this]() { SetPenColor(COLOR_PALETTE.at("turquoise")); });
	connect(m_ui.Bucket, &QPushButton::clicked, this, &Game::OnFillButtonClicked);
	connect(m_ui.BrushSize, &QPushButton::clicked, this, &Game::ChangeBrushSize);
	connect(m_ui.Undo, &QPushButton::clicked, this, &Game::OnUndoButtonClicked);
	connect(m_ui.SendMesageButton, &QPushButton::clicked, this, &Game::OnSendButtonClicked);
	connect(m_ui.LeaveGame, &QPushButton::clicked, this, &Game::OnLeaveButtonClicked);
	connect(this, SIGNAL(PlayerQuit()), this, SLOT(OnPlayerQuit()));
	connect(m_updateTimer.get(), SIGNAL(timeout()), this, SLOT(UpdateRoomInformation()));
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
	
	auto clearImageRequest = RoutingManager::ClearDrawingImage(m_roomID);
}

void Game::StopTimer()
{
	if (m_updateTimer && m_updateTimer->isActive()) {
		m_updateTimer->stop();
	}
}

void Game::ClearDrawingArea()
{
	if (auto* drawingArea = GetDrawingWidget()) {
		drawingArea->ClearDrawing();
		m_lastSentDrawingHash = 0; // Reset cached drawing hash
	}
}

void Game::SetPenColor(QColor color)
{
	if (auto* drawingArea = GetDrawingWidget()) {
		drawingArea->SetPenColor(color);
		drawingArea->SetCurrentFillColor(color);
	}
}

void Game::OnSendButtonClicked()
{
	QString text = m_ui.textEdit->toPlainText();
	if (text.isEmpty() || text.size() < 3) {
		return;
	}

	// Send message asynchronously through worker
	QMetaObject::invokeMethod(m_networkWorker, [this, text]() {
		m_networkWorker->SendMessage(text);
	}, Qt::QueuedConnection);

	m_ui.chat->ensureCursorVisible();
	m_ui.textEdit->clear();
}

void Game::UpdateRoomInformation()
{
	// Simply trigger the worker to fetch data in the background
	// The worker will emit signals when data is ready
	QMetaObject::invokeMethod(m_networkWorker, &NetworkWorker::FetchRoomUpdate, Qt::QueuedConnection);
}

void Game::OnPlayerQuit()
{
	cpr::Response request = RoutingManager::CheckGameEnded(m_roomID);

	if (Utils::IsResponseSuccessful(request)) {
		return;
	}

	RoutingManager::LeaveRoom(m_roomID, m_username);
	ClearDrawingArea();
}

void Game::SetUIElementsVisibility(std::span<QWidget*> widgets, bool visible) noexcept
{
	for (auto* widget : widgets) {
		widget->setVisible(visible);
	}
}

void Game::DisplayPlayer(const std::string& username, int index, const std::string& score) noexcept
{
	if (index < 1 || index > 4) {
		return;
	}
	
	const std::array<std::pair<QLabel*, QLabel*>, 4> playerWidgets = {
		std::make_pair(m_ui.player1_3, m_ui.player1_score),
		std::make_pair(m_ui.player2_3, m_ui.player2_score),
		std::make_pair(m_ui.player3_3, m_ui.player3_score),
		std::make_pair(m_ui.player4_3, m_ui.player4_score)
	};
	
	auto& [nameLabel, scoreLabel] = playerWidgets[static_cast<size_t>(index - 1)];
	Utils::ShowLabelWithText(nameLabel, username);
	Utils::ShowLabelWithText(scoreLabel, score);
}

void Game::DisplayPlayerCount(int count) noexcept
{
	std::array<QWidget*, 4> nameLabels = {
		m_ui.player1_3, m_ui.player2_3, m_ui.player3_3, m_ui.player4_3
	};
	std::array<QWidget*, 4> scoreLabels = {
		m_ui.player1_score, m_ui.player2_score, m_ui.player3_score, m_ui.player4_score
	};
	
	Utils::SetWidgetVisibilityByCount(nameLabels, count);
	Utils::SetWidgetVisibilityByCount(scoreLabels, count);
}

void Game::HidePlayers() noexcept
{
	m_ui.player1_3->hide();
	m_ui.player2_3->hide();
	m_ui.player3_3->hide();
	m_ui.player4_3->hide();
	m_ui.player1_score->hide();
	m_ui.player2_score->hide();
	m_ui.player3_score->hide();
	m_ui.player4_score->hide();
}

void Game::StartTimer() noexcept
{
	m_updateTimer->start(UPDATE_INTERVAL_MS);
}



void Game::OnFillButtonClicked()
{
	if (auto* drawingArea = GetDrawingWidget()) {
		drawingArea->ToggleFillMode();
	}
}

void Game::OnUndoButtonClicked()
{
	if (auto* drawingArea = GetDrawingWidget()) {
		drawingArea->Undo();
	}
}

void Game::ChangeBrushSize()
{
	if (auto* drawingArea = GetDrawingWidget()) {
		m_currentBrushSizeIndex = (m_currentBrushSizeIndex + 1) % BRUSH_SIZES.size(); 
		drawingArea->SetPenWidth(BRUSH_SIZES[m_currentBrushSizeIndex]);
	}
}

void Game::OnTimeEnd()
{
	if (!m_isOwner) {
		return;
	}

	QMetaObject::invokeMethod(m_networkWorker, &NetworkWorker::StartNextRound, Qt::QueuedConnection);
	ClearDrawingArea();
}

void Game::OnLeaveButtonClicked()
{
	cpr::Response request = RoutingManager::CheckGameEnded(m_roomID);

	if (request.status_code == 200) {
		emit NavigateToMenu(m_username);
		return;
	}

	// Send request to server to remove the player from the room (game) - async
	QMetaObject::invokeMethod(m_networkWorker, &NetworkWorker::LeaveRoom, Qt::QueuedConnection);

	emit NavigateToMenu(m_username);
	ClearDrawingArea();
}



void Game::EndGame() const noexcept
{
	auto endGameRequest = RoutingManager::EndGame(m_roomID);
}



DrawingWidget* Game::GetDrawingWidget() const noexcept
{
	return qobject_cast<DrawingWidget*>(m_ui.drawingArea);
}

std::optional<QString> Game::GetResponseText(const cpr::Response& response) const noexcept
{
	if (Utils::IsResponseSuccessful(response)) {
		return Utils::ToQString(response.text);
	}

	return std::nullopt;
}

void Game::StartCountdownTimer(int seconds, const QString& message, std::function<void()> onComplete) noexcept
{
	StopTimer();
	int time = seconds;
	
	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, [=]() mutable {
		m_ui.timer->display(time);
		time--;
		
		m_ui.textEdit->setReadOnly(true);
		m_ui.drawingArea->setEnabled(false);
		m_ui.wordLabel->setText(message);
		
		if (time <= 0) {
			timer->stop();
			timer->deleteLater();
			if (onComplete) {
				onComplete();
			}
		}
	});
	
	timer->start(1000);
}

void Game::SetDrawingUIVisibility(bool visible) noexcept
{
	const std::array<QWidget*, 17> drawingTools = {
		m_ui.Clear, m_ui.Verde, m_ui.Rosu, m_ui.Albastru,
		m_ui.Orange, m_ui.Brown, m_ui.Purple, m_ui.White,
		m_ui.Black, m_ui.Grey, m_ui.Yellow, m_ui.Pink,
		m_ui.Turquoise, m_ui.SettingsButton, m_ui.Bucket,
		m_ui.BrushSize, m_ui.Undo
	};
	
	for (auto* widget : drawingTools) {
		widget->setVisible(visible);
	}
}

void Game::ConfigureUIForDrawer(const QString& word) noexcept
{
	m_ui.textEdit->setReadOnly(true);
	m_ui.drawingArea->setEnabled(true);
	SetDrawingUIVisibility(true);
	m_ui.wordLabel->setText(word);
}

void Game::ConfigureUIForGuesser(const QString& word) noexcept
{
	m_ui.textEdit->setReadOnly(m_guessedWord);
	m_ui.drawingArea->setEnabled(false);
	SetDrawingUIVisibility(false);
	m_ui.wordLabel->setText(m_guessedWord ? word : GetMaskedWord(word));
}

QString Game::GetMaskedWord(const QString& word) const noexcept
{
	QString masked;
	masked.reserve(word.size() * 2);
	
	for (const QChar& ch : word) {
		masked += (ch == ' ') ? " " : "_ ";
	}
	
	return masked;
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
	QMetaObject::invokeMethod(m_networkWorker, [this, image]() {
		m_networkWorker->SendDrawingData(image);
	}, Qt::QueuedConnection);
}

void Game::OnRoomUpdateReceived(const RoomUpdateData& data)
{
	// Check game ended
	const bool onlyPlayerLeft = data.currentPlayerCount == 1;
	const bool gameEnded = data.gameEnded || onlyPlayerLeft;

	if (onlyPlayerLeft) {
		EndGame();
	}

	if (gameEnded) {
		m_ui.drawerLabel->setText(onlyPlayerLeft ? "You are the only player left in the room" : "Game has ended");
		
		StartCountdownTimer(COUNTDOWN_SECONDS, "Going back to the menu in 10 seconds", [this]() {
			emit NavigateToMenu(m_username);
			ClearDrawingArea();
			if (m_isOwner) {
				EndGame();
			}
		});
		return;
	}
	
	// Update drawing player and word
	if (!data.drawingPlayer.empty()) {
		m_ui.drawerLabel->setText(Utils::ToQString(data.drawingPlayer) + " is currently drawing");
		m_isDrawing = (m_username == data.drawingPlayer);
	}
	
	if (!data.currentWord.empty()) {
		const QString word = Utils::ToQString(data.currentWord);
		m_isDrawing ? ConfigureUIForDrawer(word) : ConfigureUIForGuesser(word);
	}
	
	// Update round number
	if (!data.roundNumber.empty()) {
		QString roundNumber = "Round: " + Utils::ToQString(data.roundNumber);
		m_ui.roundLabel->setText(roundNumber);
	}
	
	// Update time left
	if (!data.timeLeft.empty()) {
		m_ui.timer->display(Utils::ToQString(data.timeLeft));
		if (std::stoi(data.timeLeft) == 0) {
			m_guessedWord = false;
			ClearDrawingArea();
			if (m_isOwner) {
				OnTimeEnd();
			}
		}
	}
	
	// Update chat
	if (!data.chat.empty()) {
		std::string decodedString{ urlDecode(data.chat) };
		QString chat = QString::fromUtf8(decodedString.data(), static_cast<int>(decodedString.size()));
		m_ui.chat->setPlainText(chat);
		m_ui.chat->verticalScrollBar()->setValue(m_ui.chat->verticalScrollBar()->maximum());
	}
	
	// Update drawing image
	const int timeLeft = m_ui.timer->intValue();
	if (timeLeft != 0) {
		if (m_isDrawing) {
			SendCurrentDrawing();
		} else if (!data.drawingImage.empty()) {
			const QByteArray byteArray = QByteArray::fromBase64(data.drawingImage.c_str());
			QImage receivedImage{621, 491, QImage::Format_ARGB32};
			receivedImage.loadFromData(byteArray, "PNG");
			
			if (auto* drawingArea = GetDrawingWidget()) {
				drawingArea->SetImage(receivedImage);
			}
			update();
		}
	} else if (!m_isDrawing) {
		ClearDrawingArea();
	}
	
	// Update players and scores
	if (!data.players.empty()) {
		std::vector<std::string> players = split(data.players, ",");
		if (!players.empty()) {
			DisplayPlayerCount(players.size());
			
			for (int i = 0; i < players.size(); i++) {
				// Get score from the update data
				auto it = data.playerScores.find(players[i]);
				if (it != data.playerScores.end()) {
					DisplayPlayer(players[i], i + 1, it->second);
				} else {
					// Fallback: display with "0" if score not available
					DisplayPlayer(players[i], i + 1, "0");
				}
			}
		}
	}
	
	// Check if all players guessed
	if (data.allPlayersGuessed) {
		StartCountdownTimer(COUNTDOWN_SECONDS, "All players guessed. Moving to next round in 10 seconds.", [this]() {
			OnTimeEnd();
			ClearDrawingArea();
			m_guessedWord = false;
			m_updateTimer->start(UPDATE_INTERVAL_MS);
		});
	}
}

void Game::OnMessageSent(bool success, bool correctGuess)
{
	if (!success) {
		return;
	}
	
	if (correctGuess) {
		m_guessedWord = true;
	}
}

void Game::OnPlayerScoreReceived(const std::string& playerName, const std::string& score)
{
	// Cache the score
	m_playerScores[playerName] = score;
	
	// Find and update the player display
	// This will be updated in the next OnRoomUpdateReceived call
}
