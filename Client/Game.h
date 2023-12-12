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
    void clearDrawingArea();
    void setPenColorGreen();
    void setPenColorRed();
    void setPenColorBlue();
    void setPenColorOrange();
    void setPenColorBrown();
    void setPenColorPurple();
    void setPenColorBlack();
    void setPenColorWhite();
    void setPenColorYellow();
    void setPenColorGrey();
    void setPenColorTurquoise();
    void setPenColorPink();
    void openSettings();
    void onSendButtonClicked();

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
