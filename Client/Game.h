#ifndef GAME_H
#define GAME_H

#include "ui_Game.h"
#include "DrawingWidget.h"
#include "NetworkWorker.h"

#include <QWidget>
#include <QTimer>
#include <QThread>

#include <cpr/cpr.h>

#include <algorithm>
#include <array>
#include <unordered_map>
#include <functional>
#include <span>
#include <optional>

class Game : public QWidget {
	Q_OBJECT

signals:
	void PlayerQuit();
	void NavigateToMenu(const std::string& username);

public:
	explicit Game(const std::string& username, int playerIndex, bool isOwner = false, const std::string& m_roomID = "", QWidget* parent = nullptr);
	~Game();
	void StopTimer();

private slots:
	void ClearDrawingArea();
	void SetPenColor(QColor color);
	void OnFillButtonClicked();
	void OnSendButtonClicked();
	void OnUndoButtonClicked();
	void OnRedoButtonClicked();
	void UpdateRoomInformation();
	void OnPlayerQuit();
	void ChangeBrushSize(int size);
	void OnTimeEnd();
	void OnLeaveButtonClicked();
	
	// Drawing tool mode slots
	void OnLineToolClicked();
	void OnRectangleToolClicked();
	void OnCircleToolClicked();
	void OnPenToolClicked();
	void OnEraserToolClicked();
	void OnColorPickerClicked();
	void OnToggleAntiAliasingClicked();
	void OnColorPicked(const QColor& color);
	
	// Network worker response slots
	void OnRoomUpdateReceived(const RoomUpdateData& data);
	void OnMessageSent(bool success, bool correctGuess);

private:
	// Display helpers
	void DisplayPlayer(const std::string& username, int index, const std::string& score) noexcept;
	void DisplayPlayerCount(int count) noexcept;
	void HidePlayers() noexcept;
	void SetUIElementsVisibility(std::span<QWidget*> widgets, bool visible) noexcept;
	
	// Timer management
	void StartTimer() noexcept;
	void StartCountdownTimer(int seconds, const QString& message, std::function<void()> onComplete) noexcept;
	
	// Game control
	void EndGame() const noexcept;
	
	// Helpers
	DrawingWidget* GetDrawingWidget() const noexcept;
	std::optional<QString> GetResponseText(const cpr::Response& response) const noexcept;
	void SetDrawingUIVisibility(bool visible) noexcept;
	
	// UI Configuration helpers
	void ConfigureUIForDrawer(const QString& word) noexcept;
	void ConfigureUIForGuesser(const QString& word) noexcept;
	QString GetMaskedWord(const QString& word) const noexcept;
	
	// Drawing helpers
	void SendCurrentDrawing() const noexcept;
	void UpdateToolButtonStates();

private:
	Ui::GameClass m_ui;
	std::string m_username;
	std::string m_roomID;
	int m_playerIndex;
	bool m_isOwner;
	bool m_isDrawing;
	bool m_guessedWord;

private:
	std::unique_ptr<QTimer> m_updateTimer;
	
	// Network worker and thread
	QThread* m_workerThread;
	NetworkWorker* m_networkWorker;
	
	// Player score cache
	std::unordered_map<std::string, std::string> m_playerScores;
	
	// Drawing state tracking (using hash to avoid heap issues with QByteArray)
	mutable size_t m_lastSentDrawingHash{0};
	
	// Constants
	static constexpr int UPDATE_INTERVAL_MS = 200;
	static constexpr int COUNTDOWN_SECONDS = 10;
	
	// Color palette
	static const std::unordered_map<QString, QColor> COLOR_PALETTE;
};

#endif // GAME_H
