#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include "ui_Game.h"
#include "DrawingWidget.h"

class Game : public QMainWindow {
    Q_OBJECT

public:
    explicit Game(const std::string& username, int playerIndex, bool isOwner = false, QWidget* parent = nullptr);
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

    void OnSendButtonClicked();

private:
    void DisplayPlayer(const std::string& username, int index);

private:
    Ui::GameClass m_ui;
    DrawingWidget* m_drawingArea;
    std::string m_username;
    bool m_isOwner;
    int m_playerIndex;
};

#endif // GAME_H
