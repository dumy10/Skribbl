#include "Game.h"
#include "Settings.h"
#include <QScrollBar>


/*
TODO:
- connect to server
- send message to server 
- check if the message is the same as the word
- if it is, send a message to the server that the player won and display a message in the chat that the player won
- send the chat to the server and display it to the other players
- display the chat from the server
- display the word to guess from the server to the player that has to draw
- display the word length to the other players
- display the number of players in the room
- update room information (players, timer, word, image that is being drawn)
- display the image that is being drawn to the other players (the image is updated every 0.5 seconds)
- display the timer to all players (the timer is updated every 0.5 seconds)
- add the player score in the UI 
*/

Game::Game(const std::string& username, bool isOwner, QWidget* parent)
    : QMainWindow(parent) , m_username(username) , m_isOwner(isOwner)
{
    m_drawingArea = new DrawingWidget(this);
    m_ui.setupUi(this);
    m_ui.chat->setPlainText("Welcome to skribbl!");
    m_ui.player1_3->setText(QString::fromUtf8(username.data(), int(username.size())));

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
    connect(m_ui.SendMesageButton, &QPushButton::clicked, this, &Game::onSendButtonClicked);
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

void Game::onSendButtonClicked()
{
    QString text = m_ui.textEdit->toPlainText();
    if (text == "" || text.size() < 3)
        return;

    if (text == "word")
    {
        m_ui.chat->setPlainText(m_ui.chat->toPlainText() + "\n" + QString::fromUtf8(m_username.data(), int(m_username.size())) + " guessed the word");
        m_ui.textEdit->clear();
        m_ui.textEdit->setReadOnly(true);
    }
    else
    {
		m_ui.chat->setPlainText(m_ui.chat->toPlainText() + "\n" + QString::fromUtf8(m_username.data(), int(m_username.size())) + ": " + text);
        m_ui.chat->ensureCursorVisible();
		m_ui.textEdit->clear();
	}
    m_ui.chat->verticalScrollBar()->setValue(m_ui.chat->verticalScrollBar()->maximum());

}
