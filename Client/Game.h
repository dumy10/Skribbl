#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include "ui_Game.h"
#include "DrawingWidget.h"
#include <QTimer>

class Game : public QMainWindow {
	Q_OBJECT

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
	void UpdatePlayerInformation();

private:
	void DisplayPlayer(const std::string& username, int index);
	void StartTimer();

private:
	Ui::GameClass m_ui;
	DrawingWidget* m_drawingArea;
	std::string m_username;
	std::string m_roomID;
	bool m_isOwner;
	int m_playerIndex;

private:
	std::unique_ptr<QTimer> m_updateTimer;
};

#endif // GAME_H
