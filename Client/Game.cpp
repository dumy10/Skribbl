#include "Game.h"
#include "Settings.h"
#include <QScrollBar>
#include "utils.h"

#include <cpr/cpr.h>
#include <crow.h>


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

Game::Game(const std::string& username, int playerIndex, bool isOwner, QWidget* parent)
    : QMainWindow(parent), m_username(username), m_isOwner(isOwner), m_playerIndex(playerIndex)
{
    m_drawingArea = new DrawingWidget(this);
    m_ui.setupUi(this);
    m_ui.chat->setPlainText("Welcome to skribbl!");
    m_ui.player1_3->hide();
    m_ui.player2_3->hide();
    m_ui.player3_3->hide();
    m_ui.player4_3->hide();

    DisplayPlayer(m_username, m_playerIndex);


    connect(m_ui.Clear, &QPushButton::clicked, this, &Game::ClearDrawingArea);
    connect(m_ui.Verde, &QPushButton::clicked, this, &Game::SetPenColorGreen);
    connect(m_ui.Rosu, &QPushButton::clicked, this, &Game::SetPenColorRed);
    connect(m_ui.Albastru, &QPushButton::clicked, this, &Game::SetPenColorBlue);
    connect(m_ui.Orange, &QPushButton::clicked, this, &Game::SetPenColorOrange);
    connect(m_ui.Brown, &QPushButton::clicked, this, &Game::SetPenColorBrown);
    connect(m_ui.Purple, &QPushButton::clicked, this, &Game::SetPenColorPurple);
    connect(m_ui.White, &QPushButton::clicked, this, &Game::SetPenColorWhite);
    connect(m_ui.Black, &QPushButton::clicked, this, &Game::SetPenColorBlack);
    connect(m_ui.Grey, &QPushButton::clicked, this, &Game::SetPenColorGrey);
    connect(m_ui.Yellow, &QPushButton::clicked, this, &Game::SetPenColorYellow);
    connect(m_ui.Pink, &QPushButton::clicked, this, &Game::SetPenColorPink);
    connect(m_ui.Turquoise, &QPushButton::clicked, this, &Game::SetPenColorTurquoise);
    connect(m_ui.SettingsButton, &QPushButton::clicked, this, &Game::OpenSettings);
    connect(m_ui.SendMesageButton, &QPushButton::clicked, this, &Game::OnSendButtonClicked);
    connect(m_ui.fillButton, &QPushButton::clicked, this, &Game::onFillButtonClicked);

}

Game::~Game()
{
    // Destructorul clasei Game
}

void Game::ClearDrawingArea()
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->ClearDrawing();
}

void Game::SetPenColorGreen()
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(Qt::green);
}

void Game::SetPenColorRed()
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(Qt::red);
}

void Game::SetPenColorBlue()
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(Qt::blue);
}

void Game::SetPenColorOrange()
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(QColor(255, 165, 0)); // RGB for orange
}

void Game::SetPenColorPurple()
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(QColor(128, 0, 128)); // RGB for purple
}

void Game::SetPenColorBrown()
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(QColor(165, 42, 42)); // RGB for brown
}

void Game::SetPenColorBlack()
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(QColor(0, 0, 0)); // RGB for black
}

void Game::SetPenColorWhite()
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(QColor(255, 255, 255)); // RGB for white
}

void Game::SetPenColorYellow()
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(QColor(255, 255, 0)); // RGB for yellow
}

void Game::SetPenColorGrey()
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(QColor(128, 128, 128)); // RGB for grey
}

void Game::SetPenColorTurquoise()
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(QColor(64, 224, 208)); // RGB for turquoise
}

void Game::SetPenColorPink()
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea)
        drawingArea->SetPenColor(QColor(255, 192, 203)); // RGB for pink
}

void Game::onFillButtonClicked() 
{
    DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
    if (drawingArea) 
        drawingArea->setFillMode(true); 
}

void Game::OpenSettings()
{
    Settings* settings = new Settings(this);
    settings->setWindowOpacity(0.5);
    settings->show();
}

void Game::OnSendButtonClicked()
{
    QString text = m_ui.textEdit->toPlainText();
    if (text == "" || text.size() < 3)
        return;

    /*
    TODO:
    needs to send the text to the server
    check if the text is the same as the word
    if it is, send a message to the server that the player won and display a message in the chat that the player won
    send the chat to the server and display it to the other players
    display the chat from the server
    */

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

void Game::DisplayPlayer(const std::string& username, int index)
{
    switch (index)
    {
    case 1:
        m_ui.player1_3->show();
        m_ui.player1_3->setText(QString::fromUtf8(username.data(), int(username.size())));
        break;
    case 2:
        m_ui.player2_3->show();
        m_ui.player2_3->setText(QString::fromUtf8(username.data(), int(username.size())));
        break;
    case 3:
        m_ui.player3_3->show();
        m_ui.player3_3->setText(QString::fromUtf8(username.data(), int(username.size())));
        break;
    case 4:
        m_ui.player4_3->show();
        m_ui.player4_3->setText(QString::fromUtf8(username.data(), int(username.size())));
        break;
    default:
        break;
    }
}
