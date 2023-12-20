#include "Game.h"
#include "Settings.h"
#include "utils.h"

#include <cpr/cpr.h>
#include <crow.h>

#include <QScrollBar>

Game::Game(const std::string& username, int playerIndex, bool isOwner, const std::string& m_roomID, QWidget* parent)
	: QMainWindow(parent), m_username(username), m_isOwner(isOwner), m_playerIndex(playerIndex), m_roomID(m_roomID)
{
	m_drawingArea = std::make_shared<DrawingWidget>(this);
	m_ui.setupUi(this);
	m_ui.chat->setPlainText("Welcome to skribbl!");

	HidePlayers();
	DisplayPlayer(m_username, m_playerIndex, "0");
	m_updateTimer = std::make_unique<QTimer>(this);

	StartTimer();
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
	connect(m_ui.fillButton, &QPushButton::clicked, m_drawingArea.get(), &DrawingWidget::ToggleFillMode);
	connect(m_updateTimer.get(), SIGNAL(timeout()), this, SLOT(UpdateRoomInformation()));
	connect(this, SIGNAL(PlayerQuit()), this, SLOT(OnPlayerQuit()));
}

Game::~Game()
{
	m_updateTimer->stop();
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

void Game::OnFillButtonClicked()
{
	DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
	if (drawingArea)
		drawingArea->ToggleFillMode();
}

void Game::OpenSettings()
{
	Settings* settings = new Settings(this);
	settings->show();
	// if the settings window is closed, the settings windows should be deleted from memory so it doesn't take up space
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

/*
TODO:
- update room information ( timer, word, image that is being drawn)
- if the player is the one that has to draw, display the word to guess, otherwise display the word length
- if the player is the one that has to draw, send the image that is being drawn onto to the server every 0.2 seconds, 
otherwise pull and display the image from the server every 0.2 seconds
- update chat for all players every 0.2 seconds
- the player that is currently drawing shouldnt be able to send messages to the chat
- the players that are not drawing shouldnt be able to draw
*/
void Game::UpdateRoomInformation()
{
	auto req = cpr::Get(
		cpr::Url{ Server::GetUrl() + "/roomPlayers" },
		cpr::Payload{ {"roomID", m_roomID} }
	);
	std::vector<std::string> players = split(req.text, ",");

	if (players.empty())
		return;

	// get the drawing player name from the server


	if (m_isDrawing)
	{
		//disable player from sending messages to the chat
		m_ui.textEdit->setReadOnly(true);
		//allow player to draw
		m_ui.drawingArea->setEnabled(true);
	}
	else
	{
		//enable player to send messages to the chat
		m_ui.textEdit->setReadOnly(false);
		//disable player from drawing
		m_ui.drawingArea->setEnabled(false);
	}

	DisplayPlayerCount(players.size());

	for (int i = 0; i < players.size(); i++)
		DisplayPlayer(players[i], i + 1, "0");
}

void Game::OnPlayerQuit()
{
	auto request = cpr::Get(
		cpr::Url{ Server::GetUrl() + "/gameEnded" },
		cpr::Payload{ {"roomID", m_roomID} }
	);

	if (request.status_code == 200)
		return;

	// send request to server to remove the player from the room (game)
	auto req = cpr::Post(
		cpr::Url{ Server::GetUrl() + "/leaveRoom" },
		cpr::Payload{
			{"roomID", m_roomID},
			{"username", m_username}
		}
	);
}

void Game::DisplayPlayer(const std::string& username, int index, const std::string& score)
{
	switch (index)
	{
	case 1:
		m_ui.player1_3->show();
		m_ui.player1_3->setText(QString::fromUtf8(username.data(), int(username.size())));
		m_ui.player1_score->show();
		m_ui.player1_score->setText(QString::fromUtf8(score.data(), int(score.size())));
		break;
	case 2:
		m_ui.player2_3->show();
		m_ui.player2_3->setText(QString::fromUtf8(username.data(), int(username.size())));
		m_ui.player2_score->show();
		m_ui.player2_score->setText(QString::fromUtf8(score.data(), int(score.size())));
		break;
	case 3:
		m_ui.player3_3->show();
		m_ui.player3_3->setText(QString::fromUtf8(username.data(), int(username.size())));
		m_ui.player3_score->show();
		m_ui.player3_score->setText(QString::fromUtf8(score.data(), int(score.size())));
		break;
	case 4:
		m_ui.player4_3->show();
		m_ui.player4_3->setText(QString::fromUtf8(username.data(), int(username.size())));
		m_ui.player4_score->show();
		m_ui.player4_score->setText(QString::fromUtf8(score.data(), int(score.size())));
		break;
	default:
		break;
	}
}

void Game::DisplayPlayerCount(int count)
{
	switch (count)
	{
	case 1:
		m_ui.player1_3->show();
		m_ui.player1_score->show();
		m_ui.player2_3->hide();
		m_ui.player2_score->hide();
		m_ui.player3_3->hide();
		m_ui.player3_score->hide();
		m_ui.player4_3->hide();
		m_ui.player4_score->hide();
		break;
	case 2:
		m_ui.player1_3->show();
		m_ui.player1_score->show();
		m_ui.player2_3->show();
		m_ui.player2_score->show();
		m_ui.player3_3->hide();
		m_ui.player3_score->hide();
		m_ui.player4_3->hide();
		m_ui.player4_score->hide();
		break;
	case 3:
		m_ui.player1_3->show();
		m_ui.player1_score->show();
		m_ui.player2_3->show();
		m_ui.player2_score->show();
		m_ui.player3_3->show();
		m_ui.player3_score->show();
		m_ui.player4_3->hide();
		m_ui.player4_score->hide();
		break;
	case 4:
		m_ui.player1_3->show();
		m_ui.player1_score->show();
		m_ui.player2_3->show();
		m_ui.player2_score->show();
		m_ui.player3_3->show();
		m_ui.player3_score->show();
		m_ui.player4_3->show();
		m_ui.player4_score->show();
		break;
	default:
		break;
	}
}

void Game::HidePlayers()
{
	m_ui.player1_3->hide();
	m_ui.player2_3->hide();
	m_ui.player3_3->hide();
	m_ui.player4_3->hide();
	m_ui.player1_score->hide();
	m_ui.player2_score->hide();
	m_ui.player3_score->hide();
	m_ui.player4_score->hide();
}

void Game::StartTimer()
{
	m_updateTimer->start(200);
}

void Game::closeEvent(QCloseEvent* event)
{
	emit PlayerQuit();

	QMainWindow::closeEvent(event);
}