#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include "ui_Game.h"
#include "DrawingWidget.h"
#include <QTimer>

class Game : public QMainWindow {
	Q_OBJECT

signals:
	void PlayerQuit();

public:
	explicit Game(const std::string& username, int playerIndex, bool isOwner = false, const std::string& m_roomID = "", QWidget* parent = nullptr);
	~Game();

private slots:
	void ClearDrawingArea();
	void SetPenColorGreen();
	void SetPenColorRed();
	void SetPenColorBlue();
	void SetPenColorOrange();
	void SetPenColorBrown();
	void SetPenColorPurple();
	void SetPenColorBlack();
	void SetPenColorWhite();
	void SetPenColorYellow();
	void SetPenColorGrey();
	void SetPenColorTurquoise();
	void SetPenColorPink();
	void OpenSettings();
	void OnFillButtonClicked();
	void OnSendButtonClicked();
	void OnUndoButtonClicked();
	void UpdateRoomInformation();
	void OnPlayerQuit();
	void isDrawing();
	void showDrawingUI();
	void hideDrawingUI();

private:
	void DisplayPlayer(const std::string& username, int index, const std::string& score);
	void DisplayPlayerCount(int count);
	void HidePlayers();
	void StartTimer();
	void closeEvent(QCloseEvent* event) override;

private:
	Ui::GameClass m_ui;
	std::string m_username;
	std::string m_roomID;
	int m_playerIndex;
	bool m_isOwner;
	bool m_isDrawing = true;
	bool m_guessedWord;

private:
	std::shared_ptr<DrawingWidget> m_drawingArea;
	std::unique_ptr<QTimer> m_updateTimer;
};

#endif // GAME_H
