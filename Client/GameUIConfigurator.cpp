#include "GameUIConfigurator.h"

GameUIConfigurator::GameUIConfigurator(GameStateManager* stateManager, ChatManager* chatManager,
	DrawingToolsUIManager* drawingToolsManager, QLabel* wordLabel, QWidget* drawingArea)
	: m_stateManager(stateManager)
	, m_chatManager(chatManager)
	, m_drawingToolsManager(drawingToolsManager)
	, m_wordLabel(wordLabel)
	, m_drawingArea(drawingArea)
{
}

void GameUIConfigurator::ConfigureForDrawer(const QString& word) noexcept
{
	m_chatManager->SetInputReadOnly(true);
	m_drawingArea->setEnabled(true);
	m_drawingToolsManager->SetDrawingUIVisibility(true);
	m_wordLabel->setText(word);
}

void GameUIConfigurator::ConfigureForGuesser(const QString& word) noexcept
{
	m_chatManager->SetInputReadOnly(m_stateManager->HasGuessedWord());
	m_drawingArea->setEnabled(false);
	m_drawingToolsManager->SetDrawingUIVisibility(false);
	m_wordLabel->setText(m_stateManager->HasGuessedWord() ? word : GetMaskedWord(word));
}

QString GameUIConfigurator::GetMaskedWord(const QString& word) const noexcept
{
	QString masked;
	masked.reserve(word.size() * 2);

	for (const QChar& ch : word) {
		masked += (ch == ' ') ? " " : "_ ";
	}

	return masked;
}
