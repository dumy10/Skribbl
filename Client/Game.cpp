#include "Game.h"
#include "Settings.h"

Game::Game(QWidget* parent)
    : QMainWindow(parent) {
    m_ui.setupUi(this);

    connect(m_ui.Clear, &QPushButton::clicked, this, &Game::clearDrawingArea);
    connect(m_ui.Verde, &QPushButton::clicked, this, &Game::setPenColorGreen);
    connect(m_ui.Rosu, &QPushButton::clicked, this, &Game::setPenColorRed);
    connect(m_ui.Albastru, &QPushButton::clicked, this, &Game::setPenColorBlue);
    connect(m_ui.Orange, &QPushButton::clicked, this, &Game::setPenColorOrange);
    connect(m_ui.Brown, &QPushButton::clicked, this, &Game::setPenColorBrown);
    connect(m_ui.Purple, &QPushButton::clicked, this, &Game::setPenColorPurple);
    connect(m_ui.White, &QPushButton::clicked, this, &Game::setPenColorWhite);
    connect(m_ui.Black, &QPushButton::clicked, this, &Game::setPenColorBlack);
    connect(m_ui.Grey, &QPushButton::clicked, this, &Game::setPenColorGrey);
    connect(m_ui.Yellow, &QPushButton::clicked, this, &Game::setPenColorYellow);
    connect(m_ui.Pink, &QPushButton::clicked, this, &Game::setPenColorPink);
    connect(m_ui.Turquoise, &QPushButton::clicked, this, &Game::setPenColorTurquoise);
    connect(m_ui.SettingsButton, &QPushButton::clicked, this, &Game::openSettings);
    connect(m_ui.SendMesageButton, &QPushButton::clicked, this, &Game::sendMessage);
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
void Game::setPenColorOrange() 
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(QColor(255, 165, 0)); // RGB for orange
}

void Game::setPenColorPurple() 
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(QColor(128, 0, 128)); // RGB for purple
}

void Game::setPenColorBrown() 
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(QColor(165, 42, 42)); // RGB for brown
}

void Game::setPenColorBlack()
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(QColor(0, 0, 0)); // RGB for black
}

void Game::setPenColorWhite()
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(QColor(255, 255, 255)); // RGB for white
}

void Game::setPenColorYellow()
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(QColor(255, 255, 0)); // RGB for yellow
}

void Game::setPenColorGrey()
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(QColor(128, 128, 128)); // RGB for grey
}

void Game::setPenColorTurquoise()
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(QColor(64, 224, 208)); // RGB for turquoise
}

void Game::setPenColorPink()
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(QColor(255, 192, 203)); // RGB for pink
}

void Game::openSettings()
{
    Settings* settings = new Settings(this);
    settings->setWindowOpacity(0.5);
    settings->show();
}

void Game::sendMessage()
{
    QString message = m_ui.textEdit->toPlainText();
    m_ui.textEdit->clear();
    
    //crow::SimpleApp app;
    
    //crow::json::wvalue requestJson;
    //requestJson["message"] = message.toStdString();
    //std::ostringstream os;
    //os << crow::json::dump(requestJson);

    //crow::SimplePostRequest req("/sendString", os.str());
    //auto response = app.handle(req);

    //my project doesn't work for now and i can't test it , so i will leave it with // for now
}
