#ifndef LOBBY_H
#define LOBBY_H

#include <QMainWindow>
#include "ui_Lobby.h"
#include "DrawingWidget.h"

class Lobby : public QMainWindow {
    Q_OBJECT

public:
    explicit Lobby(QWidget* parent = nullptr);
    ~Lobby();

public slots:
    void clearDrawingArea();
    void setPenColorGreen();
    void setPenColorRed();
    void setPenColorBlue();
    void openSettings();

private:
    Ui::LobbyClass m_ui;
    DrawingWidget* m_drawingArea;
};

#endif // LOBBY_H
