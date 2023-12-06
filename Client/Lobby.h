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

    //there is a QPushButton in Lobby.ui called SettingsButton that should open the Settings.ui window
    void openSettings();

private:
    Ui::LobbyClass m_ui;
    DrawingWidget* drawingArea;
};

#endif // LOBBY_H
