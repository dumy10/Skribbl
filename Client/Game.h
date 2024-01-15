#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include "ui_Game.h"
#include "DrawingWidget.h"
#include <QTimer>
#include <algorithm>

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
	void OnFillButtonClicked();
	void OnSendButtonClicked();
	void OnUndoButtonClicked();
	void UpdateRoomInformation();
	void OnPlayerQuit();
	void ChangeBrushSize();
	void OnTimeEnd();
	void OnLeaveButtonClicked();

private:
	void DisplayPlayer(const std::string& username, int index, const std::string& score);
	void DisplayPlayerCount(int count);
	void HidePlayers();
	void ShowDrawingUI();
	void HideDrawingUI();
	void closeEvent(QCloseEvent* event) override;
	void StartTimer();
	void CheckGameEnded();
	void GamePlayers();
	void CheckRoundNumber();
	void UpdateTimeLeft();
	void UpdateChat();
	void UpdateDrawingPlayerAndWord();
	void UpdateDrawingImage();
	void SendDrawing(const QByteArray& drawingData);
	void ReturnDrawing(std::string& drawingData);


private:
	Ui::GameClass m_ui;
	std::string m_username;
	std::string m_roomID;
	int m_playerIndex;
	bool m_isOwner;
	bool m_isDrawing;
	bool m_guessedWord;
	int m_currentBrushSizeIndex;
	
private:
	std::unique_ptr<DrawingWidget> m_drawingArea;
	std::unique_ptr<QTimer> m_updateTimer;
	std::unique_ptr<QTimer> m_roundTimer;
};

#endif // GAME_H
