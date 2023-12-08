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
        drawingArea->ClearDrawing();
}

void Lobby::setPenColorGreen() {
    m_drawingArea->SetPenColor(Qt::green);
}

void Lobby::setPenColorRed() {
    m_drawingArea->SetPenColor(Qt::red);
}

void Lobby::setPenColorBlue() {
    m_drawingArea->SetPenColor(Qt::blue);
}

void Lobby::openSettings()
{
    Settings* settings = new Settings(this);
    settings->setWindowOpacity(0.5);
    settings->show();
}
