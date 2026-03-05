#pragma once

#include "GameStateManager.h"
#include "ChatManager.h"
#include "DrawingToolsUIManager.h"

#include <QLabel>
#include <QWidget>
#include <QString>

/**
 * @brief Handles UI configuration for drawer and guesser modes.
 * 
 * This class manages the visual state of game UI based on player role,
 * including word display masking, input state, and drawing area enable state.
 */
class GameUIConfigurator
{
public:
	GameUIConfigurator(GameStateManager* stateManager, ChatManager* chatManager, 
		DrawingToolsUIManager* drawingToolsManager, QLabel* wordLabel, QWidget* drawingArea);
	
	~GameUIConfigurator() = default;
	
	// Configure UI based on player role
	void ConfigureForDrawer(const QString& word) noexcept;
	void ConfigureForGuesser(const QString& word) noexcept;
	
	// Helper method
	[[nodiscard]] QString GetMaskedWord(const QString& word) const noexcept;
	
private:
	// Manager references (non-owning)
	GameStateManager* m_stateManager;
	ChatManager* m_chatManager;
	DrawingToolsUIManager* m_drawingToolsManager;
	
	// UI element references (non-owning)
	QLabel* m_wordLabel;
	QWidget* m_drawingArea;
};
