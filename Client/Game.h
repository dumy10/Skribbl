#ifndef GAME_H
#define GAME_H

#include "ui_Game.h"
#include "DrawingWidget.h"
#include "NetworkWorker.h"
#include "GameStateManager.h"
#include "PlayerDisplayManager.h"
#include "ChatManager.h"
#include "DrawingToolsUIManager.h"
#include "GameTimerManager.h"
#include "RoomUpdateHandler.h"
#include "IGameObserver.h"

#include <QWidget>
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

#include <cpr/cpr.h>

#include <algorithm>
#include <array>
#include <unordered_map>
#include <functional>
#include <span>
#include <optional>
#include <memory>

// Forward declarations
class ConnectionSetup;

class Game : public QWidget {
	Q_OBJECT

	friend class ConnectionSetup;

signals:
	void PlayerQuit();
	void NavigateToMenu(const std::string& username);

public:
	explicit Game(const std::string& username, int playerIndex, bool isOwner = false, const std::string& m_roomID = "", QWidget* parent = nullptr);
	~Game();
	
	// Public interface for external control
	void StopTimer();

	// Observer Management
	void AddObserver(IGameObserver* observer);
	void RemoveObserver(IGameObserver* observer);

	// Notification template for observer pattern
	template <typename F, typename... Args>
	void Notify(F func, Args&&... args);

private slots:
	// UI event handlers
	void OnSendButtonClicked();
	void OnLeaveButtonClicked();
	void OnPlayerQuit();
	
	// Network worker response slots
	void OnRoomUpdateReceived(const RoomUpdateData& data);
	void OnMessageSent(bool success, bool correctGuess);
	
	// Timer slot
	void OnUpdateTimerTick();

private:
	// Game control
	void EndGame() const noexcept;
	void OnTimeEnd();
	
	// Helpers
	DrawingWidget* GetDrawingWidget() const noexcept;
	void SendCurrentDrawing() const noexcept;
	void ClearDrawing() noexcept;  // Wrapper that clears drawing and resets hash
	
	// Setup helpers
	void SetupConnections();
	void InitializeManagers();

private:
	Ui::GameClass m_ui;
	
	// Managers (encapsulate responsibilities)
	std::unique_ptr<GameStateManager> m_stateManager;
	std::unique_ptr<PlayerDisplayManager> m_playerDisplayManager;
	std::unique_ptr<ChatManager> m_chatManager;
	std::unique_ptr<DrawingToolsUIManager> m_drawingToolsManager;
	std::unique_ptr<GameTimerManager> m_timerManager;
	std::unique_ptr<RoomUpdateHandler> m_roomUpdateHandler;
	std::unique_ptr<ConnectionSetup> m_connectionSetup;
	
	// Network worker and thread
	QThread* m_workerThread;
	NetworkWorker* m_networkWorker;
	
	// Drawing state tracking (using hash to avoid heap issues with QByteArray)
	mutable size_t m_lastSentDrawingHash{0};
	
	// UI element references
	QLabel* m_wordLabel{nullptr};
	QLabel* m_drawerLabel{nullptr};
	QLabel* m_roundLabel{nullptr};
	QWidget* m_drawingArea{nullptr};
	
	// Observer pattern
	std::vector<IGameObserver*> m_observers;
	mutable QMutex m_observersMutex;
	EGameState m_currentGameState{EGameState::Unknown};
	
	// Constants
	static constexpr int UPDATE_INTERVAL_MS = 200;
};

template <typename F, typename... Args>
void Game::Notify(F func, Args&&... args)
{
	std::vector<IGameObserver*> observersCopy;
	{
		QMutexLocker locker(&m_observersMutex);
		observersCopy = m_observers;
	}

	// Execute callbacks without holding the lock
	for (auto observer : observersCopy) {
		if (observer) {
			(observer->*func)(std::forward<Args>(args)...);
		}
	}
}

#endif // GAME_H
