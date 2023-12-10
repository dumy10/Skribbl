#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include "ui_Game.h"
#include "DrawingWidget.h"

class Game : public QMainWindow {
    Q_OBJECT

public:
    explicit Game(QWidget* parent = nullptr);
    ~Game();

public slots:
    void clearDrawingArea();
    void setPenColorGreen();
    void setPenColorRed();
    void setPenColorBlue();
    void setPenColorOrange();
    void setPenColorBrown();
    void setPenColorPurple();
    void openSettings();

private:
    Ui::GameClass m_ui;
    DrawingWidget* m_drawingArea;
};

#endif // GAME_H
