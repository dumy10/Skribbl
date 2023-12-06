#include "Lobby.h"
#include "Settings.h"

Lobby::Lobby(QWidget* parent)
    : QMainWindow(parent) {
    m_ui.setupUi(this);

    connect(m_ui.Clear, &QPushButton::clicked, this, &Lobby::clearDrawingArea);
    connect(m_ui.Verde, &QPushButton::clicked, this, &Lobby::setPenColorGreen);
    connect(m_ui.Rosu, &QPushButton::clicked, this, &Lobby::setPenColorRed);
    connect(m_ui.Albastru, &QPushButton::clicked, this, &Lobby::setPenColorBlue);
    connect(m_ui.SettingsButton, &QPushButton::clicked, this, &Lobby::openSettings);
}

Lobby::~Lobby() 
{
    // Destructorul clasei Lobby
}

void Lobby::clearDrawingArea() 
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea) 
        drawingArea->clearDrawing();
}

void Lobby::setPenColorGreen() {
    drawingArea->setPenColor(Qt::green);
}

void Lobby::setPenColorRed() {
    drawingArea->setPenColor(Qt::red);
}

void Lobby::setPenColorBlue() {
    drawingArea->setPenColor(Qt::blue);
}

void Lobby::openSettings()
{
    Settings* settings = new Settings(this);
    settings->setWindowOpacity(0.5);
    settings->show();
}
