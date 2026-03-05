#pragma once

#include "ui_Game.h"
#include "NetworkWorker.h"
#include "GameTimerManager.h"
#include "DrawingToolsUIManager.h"
#include "DrawingWidget.h"

#include <QObject>
#include <QPushButton>
#include <QSlider>
#include <QCheckBox>
#include <array>
#include <string_view>

// Forward declaration
class Game;

/**
 * @brief Handles all signal/slot connection setup for the Game class.
 * 
 * This class encapsulates connection logic for network signals, color buttons,
 * tool buttons, and game controls to keep Game class focused on game logic.
 */
class ConnectionSetup
{
public:
	ConnectionSetup(Ui::GameClass& ui, Game* game, NetworkWorker* networkWorker, GameTimerManager* timerManager, DrawingToolsUIManager* drawingToolsManager);
	
	~ConnectionSetup() = default;
	
	// Setup methods for different connection categories
	void SetupNetworkConnections();
	void SetupColorButtonConnections();
	void SetupToolButtonConnections();
	void SetupGameControlConnections();
	
	// Setup all connections
	void SetupAllConnections();
	
private:
	// UI and component references (non-owning)
	Ui::GameClass& m_ui;
	Game* m_game;
	NetworkWorker* m_networkWorker;
	GameTimerManager* m_timerManager;
	DrawingToolsUIManager* m_drawingToolsManager;
	
	// Helper to get drawing widget
	[[nodiscard]] DrawingWidget* GetDrawingWidget() const noexcept;
};
