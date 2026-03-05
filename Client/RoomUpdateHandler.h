#pragma once

#include "NetworkWorker.h"
#include "GameStateManager.h"
#include "PlayerDisplayManager.h"
#include "ChatManager.h"
#include "GameTimerManager.h"
#include "DrawingWidget.h"
#include "GameUIConfigurator.h"

#include <QLabel>
#include <QWidget>
#include <functional>
#include <memory>

class RoomUpdateHandler : public QObject
{
	Q_OBJECT

public:
	RoomUpdateHandler(GameStateManager* stateManager, PlayerDisplayManager* playerDisplayManager, 
		ChatManager* chatManager, GameTimerManager* timerManager, DrawingToolsUIManager* drawingToolsManager,
		QLabel* wordLabel, QLabel* drawerLabel, QLabel* roundLabel, QWidget* drawingArea, QWidget* parent);
	
	~RoomUpdateHandler() = default;
	
	// Main handler for room updates
	void HandleRoomUpdate(const RoomUpdateData& data);
	
	// Set callback functions for actions that need to be performed in Game class
	void SetNavigateToMenuCallback(std::function<void(const std::string&)> callback);
	void SetOnTimeEndCallback(std::function<void()> callback);
	void SetEndGameCallback(std::function<void()> callback);
	void SetSendDrawingCallback(std::function<void()> callback);
	void SetClearDrawingCallback(std::function<void()> callback);
	void SetGetDrawingWidgetCallback(std::function<DrawingWidget*()> callback);
	void SetUpdateWidgetCallback(std::function<void()> callback);

private:
	// Sub-handlers for different aspects of room updates
	bool HandleGameEnded(const RoomUpdateData& data);
	void UpdateDrawingPlayerAndWord(const RoomUpdateData& data);
	void HandleTimeLeft(const RoomUpdateData& data);
	void UpdateDrawingImage(const RoomUpdateData& data);
	void HandleAllPlayersGuessed(const RoomUpdateData& data);
	
private:
	// Manager references (non-owning)
	GameStateManager* m_stateManager;
	PlayerDisplayManager* m_playerDisplayManager;
	ChatManager* m_chatManager;
	GameTimerManager* m_timerManager;
	
	// UI configurator
	std::unique_ptr<GameUIConfigurator> m_uiConfigurator;
	
	// UI element references (non-owning)
	QLabel* m_wordLabel;
	QLabel* m_drawerLabel;
	QLabel* m_roundLabel;
	QWidget* m_drawingArea;
	QWidget* m_parentWidget;
	
	// Callbacks for actions that need Game class context
	std::function<void(const std::string&)> m_navigateToMenuCallback;
	std::function<void()> m_onTimeEndCallback;
	std::function<void()> m_endGameCallback;
	std::function<void()> m_sendDrawingCallback;
	std::function<void()> m_clearDrawingCallback;
	std::function<DrawingWidget*()> m_getDrawingWidgetCallback;
	std::function<void()> m_updateWidgetCallback;
	
	// Constants
	static constexpr int COUNTDOWN_SECONDS = 10;
	static constexpr int UPDATE_INTERVAL_MS = 200;
};
