#include "Game.h"
#include "Settings.h"

Game::Game(QWidget* parent)
    : QMainWindow(parent) {
    m_ui.setupUi(this);

    connect(m_ui.Clear, &QPushButton::clicked, this, &Game::clearDrawingArea);
    connect(m_ui.Verde, &QPushButton::clicked, this, &Game::setPenColorGreen);
    connect(m_ui.Rosu, &QPushButton::clicked, this, &Game::setPenColorRed);
    connect(m_ui.Albastru, &QPushButton::clicked, this, &Game::setPenColorBlue);
    connect(m_ui.SettingsButton, &QPushButton::clicked, this, &Game::openSettings);
}

Game::~Game()
{
    // Destructorul clasei Game
}

void Game::clearDrawingArea()
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->ClearDrawing();
}

void Game::setPenColorGreen()
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(Qt::green);
}


void Game::setPenColorRed()
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(Qt::red);
}


void Game::setPenColorBlue()
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(Qt::blue);
}

void Game::openSettings()
{
    Settings* settings = new Settings(this);
    settings->setWindowOpacity(0.5);
    settings->show();
}