#include "Game.h"
#include "Menu.h"
#include "utils.h"

#include <cpr/cpr.h>
#include <crow.h>
#include <QScrollBar>
#include <QBuffer>

Game::Game(const std::string& username, int playerIndex, bool isOwner, const std::string& m_roomID, QWidget* parent)
	: QMainWindow(parent), m_username(username), m_isOwner(isOwner), m_playerIndex(playerIndex), m_roomID(m_roomID)
{
	m_drawingArea = std::make_unique<DrawingWidget>(this);
	m_ui.setupUi(this);

	HidePlayers();
	DisplayPlayer(m_username, m_playerIndex, "0");

	m_updateTimer = std::make_unique<QTimer>(this);
	m_roundTimer = std::make_unique<QTimer>(this);

	m_guessedWord = false;
	m_currentBrushSizeIndex = 0;
	StartTimer();

	if (m_isOwner)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(700));
		m_roundTimer->start(61500);
	}

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
	connect(m_ui.Bucket, &QPushButton::clicked, this, &Game::OnFillButtonClicked);
	connect(m_ui.BrushSize, &QPushButton::clicked, this, &Game::ChangeBrushSize);
	connect(m_ui.Undo, &QPushButton::clicked, this, &Game::OnUndoButtonClicked);
	connect(m_ui.SendMesageButton, &QPushButton::clicked, this, &Game::OnSendButtonClicked);
	connect(m_ui.LeaveGame, &QPushButton::clicked, this, &Game::OnLeaveButtonClicked);
	connect(this, SIGNAL(PlayerQuit()), this, SLOT(OnPlayerQuit()));
	connect(m_updateTimer.get(), SIGNAL(timeout()), this, SLOT(UpdateRoomInformation()));
	connect(m_roundTimer.get(), SIGNAL(timeout()), this, SLOT(OnTimeEnd()));
}

Game::~Game()
{
	m_updateTimer->stop();
	m_roundTimer->stop();
	auto clearImageRequest = cpr::Post(
		cpr::Url{ Server::GetUrl() + "/clearImage" },
		cpr::Payload{ {"roomID", m_roomID} }
	);
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
	{
		drawingArea->SetPenColor(Qt::green);
		drawingArea->SetCurrentFillColor(Qt::green);
	}
}

void Game::SetPenColorRed()
{
	DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
	if (drawingArea)
	{
		drawingArea->SetPenColor(Qt::red);
		drawingArea->SetCurrentFillColor(Qt::red);
	}
}

void Game::SetPenColorBlue()
{
	DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
	if (drawingArea)
	{
		drawingArea->SetPenColor(Qt::blue);
		drawingArea->SetCurrentFillColor(Qt::blue);
	}
}

void Game::SetPenColorOrange()
{
	DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
	if (drawingArea)
	{
		drawingArea->SetPenColor(QColor(255, 165, 0)); // RGB for orange
		drawingArea->SetCurrentFillColor(QColor(255, 165, 0));
	}
}

void Game::SetPenColorPurple()
{
	DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
	if (drawingArea)
	{
		drawingArea->SetPenColor(QColor(128, 0, 128)); // RGB for purple
		drawingArea->SetCurrentFillColor(QColor(128, 0, 128));
	}
}

void Game::SetPenColorBrown()
{
	DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
	if (drawingArea)
	{
		drawingArea->SetPenColor(QColor(165, 42, 42)); // RGB for brown
		drawingArea->SetCurrentFillColor(QColor(165, 42, 42));
	}
}

void Game::SetPenColorBlack()
{
	DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
	if (drawingArea)
	{
		drawingArea->SetPenColor(QColor(0, 0, 0)); // RGB for black
		drawingArea->SetCurrentFillColor(QColor(0, 0, 0));
	}
}

void Game::SetPenColorWhite()
{
	DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
	if (drawingArea)
	{
		drawingArea->SetPenColor(QColor(255, 255, 255)); // RGB for white
		drawingArea->SetCurrentFillColor(QColor(255, 255, 255));
	}
}

void Game::SetPenColorYellow()
{
	DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
	if (drawingArea)
	{
		drawingArea->SetPenColor(QColor(255, 255, 0)); // RGB for yellow
		drawingArea->SetCurrentFillColor(QColor(255, 255, 0));
	}
}

void Game::SetPenColorGrey()
{
	DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
	if (drawingArea)
	{
		drawingArea->SetPenColor(QColor(128, 128, 128)); // RGB for grey
		drawingArea->SetCurrentFillColor(QColor(128, 128, 128));
	}
}

void Game::SetPenColorTurquoise()
{
	DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
	if (drawingArea)
	{
		drawingArea->SetPenColor(QColor(64, 224, 208)); // RGB for turquoise
		drawingArea->SetCurrentFillColor(QColor(64, 224, 208));
	}
}

void Game::SetPenColorPink()
{
	DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
	if (drawingArea)
	{
		drawingArea->SetPenColor(QColor(255, 192, 203)); // RGB for pink
		drawingArea->SetCurrentFillColor(QColor(255, 192, 203));
	}
}

void Game::OnSendButtonClicked()
{
	QString text = m_ui.textEdit->toPlainText();
	if (text == "" || text.size() < 3)
		return;

	auto request = cpr::Post(
		cpr::Url{ Server::GetUrl() + "/addChat" },
		cpr::Payload{ {"roomID", m_roomID}, {"username", m_username}, {"text", text.toUtf8().constData()} }
	);

	if (request.status_code != 200)
		return;

	m_ui.chat->ensureCursorVisible();
	m_ui.textEdit->clear();

	if (request.text == "TRUE")
		m_guessedWord = true;
}

void Game::UpdateRoomInformation()
{
	CheckGameEnded();
	GamePlayers();
	CheckRoundNumber();
	UpdateTimeLeft();
	UpdateChat();
	UpdateDrawingPlayerAndWord();
	UpdateDrawingImage();
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
	ClearDrawingArea();
	this->close();
}

void Game::ShowDrawingUI()
{
	m_ui.Clear->show();
	m_ui.Verde->show();
	m_ui.Rosu->show();
	m_ui.Albastru->show();
	m_ui.Orange->show();
	m_ui.Brown->show();
	m_ui.Purple->show();
	m_ui.White->show();
	m_ui.Black->show();
	m_ui.Grey->show();
	m_ui.Yellow->show();
	m_ui.Pink->show();
	m_ui.Turquoise->show();
	m_ui.SettingsButton->show();
	m_ui.Bucket->show();
	m_ui.BrushSize->show();
	m_ui.Undo->show();
}

void Game::HideDrawingUI()
{
	m_ui.Clear->hide();
	m_ui.Verde->hide();
	m_ui.Rosu->hide();
	m_ui.Albastru->hide();
	m_ui.Orange->hide();
	m_ui.Brown->hide();
	m_ui.Purple->hide();
	m_ui.White->hide();
	m_ui.Black->hide();
	m_ui.Grey->hide();
	m_ui.Yellow->hide();
	m_ui.Pink->hide();
	m_ui.Turquoise->hide();
	m_ui.SettingsButton->hide();
	m_ui.Bucket->hide();
	m_ui.BrushSize->hide();
	m_ui.Undo->hide();
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

void Game::CheckGameEnded()
{
	auto gameEndedRequest = cpr::Get(
		cpr::Url{ Server::GetUrl() + "/gameEnded" },
		cpr::Payload{ {"roomID", m_roomID} }
	);

	if (gameEndedRequest.status_code == 200)
	{

		m_updateTimer->stop();
		m_roundTimer->stop();
		int time = 10;

		QTimer* timer = new QTimer(this);

		connect(timer, &QTimer::timeout, [=]() mutable {
			m_ui.timer->display(time);
			time--;

			m_ui.textEdit->setReadOnly(true);
			m_ui.drawingArea->setEnabled(false);
			m_ui.drawerLabel->setText("Game has ended");
			m_ui.wordLabel->setText("Going back to the menu in 10 seconds");

			if (time < 0)
			{
				timer->stop();
				timer->deleteLater();

				Menu* menu = new Menu(std::move(m_username));
				menu->show();

				ClearDrawingArea();
				this->close();
				this->deleteLater();
			}

			});

		timer->start(1000);

		return;
	}
}

void Game::GamePlayers()
{
	auto req = cpr::Get(
		cpr::Url{ Server::GetUrl() + "/roomPlayers" },
		cpr::Payload{ {"roomID", m_roomID} }
	);

	if (req.status_code != 200)
		return;

	std::vector<std::string> players = split(req.text, ",");

	if (players.empty())
		return;

	DisplayPlayerCount(players.size());

	for (int i = 0; i < players.size(); i++)
	{
		auto request = cpr::Get(
			cpr::Url{ Server::GetUrl() + "/playerScore" },
			cpr::Payload{ {"roomID", m_roomID}, {"username", players[i]} }
		);

		if (request.status_code != 200)
			return;

		DisplayPlayer(players[i], i + 1, request.text);
	}
}

void Game::CheckRoundNumber()
{
	auto roundNumberRequest = cpr::Get(
		cpr::Url{ Server::GetUrl() + "/roundNumber" },
		cpr::Payload{ {"roomID", m_roomID} }
	);

	if (roundNumberRequest.status_code != 200)
		return;

	QString roundNumber = "Round: " + QString::fromUtf8(roundNumberRequest.text.data(), int(roundNumberRequest.text.size()));
	m_ui.roundLabel->setText(roundNumber);
}

void Game::UpdateTimeLeft()
{
	if (m_isOwner)
	{
		int timeLeft = m_roundTimer->remainingTime() / 1000;
		m_ui.timer->display(timeLeft);
		if (timeLeft == 0)
		{
			m_guessedWord = false;
			ClearDrawingArea();
		}
		auto timeLeftRequest = cpr::Post(
			cpr::Url{ Server::GetUrl() + "/timeLeft" },
			cpr::Payload{ {"roomID", m_roomID}, {"timer", std::to_string(timeLeft) } }
		);
		if (timeLeftRequest.status_code != 200)
			return;
	}
	else
	{
		auto timeLeftRequest = cpr::Get(
			cpr::Url{ Server::GetUrl() + "/timeLeft" },
			cpr::Payload{ {"roomID", m_roomID} }
		);

		if (timeLeftRequest.status_code != 200)
			return;

		m_ui.timer->display(QString::fromUtf8(timeLeftRequest.text.data(), int(timeLeftRequest.text.size())));
		if (std::stoi(timeLeftRequest.text) == 0)
		{
			m_guessedWord = false;
			ClearDrawingArea();
		}
	}
}

void Game::UpdateChat()
{
	auto chatRequest = cpr::Get(
		cpr::Url{ Server::GetUrl() + "/getChat" },
		cpr::Payload{ {"roomID", m_roomID} }
	);

	if (chatRequest.status_code != 200)
		return;
	/*
	%20 = " "
	%0A = "\n"
	%0a = "\n"
	*/

	QString chat = QString::fromUtf8(chatRequest.text.data(), int(chatRequest.text.size()));
	chat.replace("%20", " ");
	chat.replace("%0A", "\n");
	chat.replace("%0a", "\n");

	m_ui.chat->setPlainText(chat);
	m_ui.chat->verticalScrollBar()->setValue(m_ui.chat->verticalScrollBar()->maximum());
}

void Game::UpdateDrawingPlayerAndWord()
{

	// get the drawing player name from the server
	auto drawingPlayerRequest = cpr::Get(
		cpr::Url{ Server::GetUrl() + "/drawingPlayer" },
		cpr::Payload{ {"roomID", m_roomID} }
	);

	if (drawingPlayerRequest.status_code != 200)
		return;

	m_ui.drawerLabel->setText(QString::fromUtf8(drawingPlayerRequest.text.data(), int(drawingPlayerRequest.text.size())) + " is currently drawing");

	if (m_username == drawingPlayerRequest.text)
		m_isDrawing = true;
	else
		m_isDrawing = false;

	auto wordRequest = cpr::Get(
		cpr::Url{ Server::GetUrl() + "/currentWord" },
		cpr::Payload{ {"roomID", m_roomID} }
	);

	if (wordRequest.status_code != 200)
		return;

	if (m_isDrawing)
	{
		m_ui.textEdit->setReadOnly(true); //disable player from sending messages to the chat
		m_ui.drawingArea->setEnabled(true); //allow player to draw
		ShowDrawingUI();
		m_ui.wordLabel->setText(QString::fromUtf8(wordRequest.text.data(), int(wordRequest.text.size())));
	}
	else
	{
		m_ui.textEdit->setReadOnly(false); //enable player to send messages to the chat
		m_ui.drawingArea->setEnabled(false); //disable player from drawing
		HideDrawingUI();
		if (!m_guessedWord)
		{
			std::string currentWordString = wordRequest.text;
			QString currentWord = "";
			for (uint i = 0; i < currentWordString.size(); i++)
			{
				if (currentWordString[i] == ' ')
					currentWord += " ";
				else
					currentWord += "_ ";
			}
			m_ui.wordLabel->setText(currentWord);
		}
		else
		{
			m_ui.wordLabel->setText(QString::fromUtf8(wordRequest.text.data(), int(wordRequest.text.size())));
			m_ui.textEdit->setReadOnly(true);
		}
	}
}

void Game::UpdateDrawingImage()
{
	if (m_isDrawing)
	{
		int timeLeft = m_ui.timer->intValue();
		if (timeLeft == 0)
			return;

		QImage image{ 621, 491, QImage::Format_ARGB32 };
		DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
		if (drawingArea)
			image = drawingArea->GetImage();

		if (image.isNull())
			return;

		std::string imageString = "";

		QByteArray byteArray;
		QBuffer buffer(&byteArray);
		buffer.open(QIODevice::WriteOnly);
		image.save(&buffer, "PNG");

		SendDrawing(byteArray);
	}
	else
	{
		int timeLeft = m_ui.timer->intValue();
		if (timeLeft == 0)
		{
			ClearDrawingArea();
			return;
		}

		std::string imageString;
		ReturnDrawing(imageString);

		if (imageString.empty())
			return;

		QByteArray byteArray = QByteArray::fromBase64(imageString.c_str());

		QImage m_receivedImage{ 621, 491, QImage::Format_ARGB32 };

		m_receivedImage.loadFromData(byteArray, "PNG");

		DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
		if (drawingArea)
			drawingArea->SetImage(m_receivedImage);
		update();
	}
}

void Game::closeEvent(QCloseEvent* event)
{
	emit PlayerQuit();

	QMainWindow::closeEvent(event);
}

void Game::OnFillButtonClicked()
{
	DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
	if (drawingArea)
		drawingArea->ToggleFillMode();
}

void Game::OnUndoButtonClicked()
{
	DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
	if (drawingArea)
		drawingArea->Undo();
}

void Game::ChangeBrushSize()
{
	DrawingWidget* drawingArea = qobject_cast<DrawingWidget*>(m_ui.drawingArea);
	if (drawingArea)
	{
		std::vector<int> brushSizes = { 3, 6, 9 };
		if (m_currentBrushSizeIndex == 2)
			m_currentBrushSizeIndex = 0;
		else
			m_currentBrushSizeIndex++;
		int newBrushSize = brushSizes[m_currentBrushSizeIndex];
		drawingArea->SetPenWidth(newBrushSize);
	}
}

void Game::OnTimeEnd()
{
	if (!m_isOwner)
		return;

	auto request = cpr::Post(
		cpr::Url{ Server::GetUrl() + "/nextRound" },
		cpr::Payload{ {"roomID", m_roomID} }
	);

	if (request.status_code != 200)
		OnTimeEnd();
	ClearDrawingArea();
}

void Game::OnLeaveButtonClicked()
{
	auto request = cpr::Get(
		cpr::Url{ Server::GetUrl() + "/gameEnded" },
		cpr::Payload{ {"roomID", m_roomID} }
	);

	if (request.status_code == 200)
	{
		Menu* menu = new Menu(std::move(m_username));
		menu->show();
		this->close();
		this->deleteLater();
		return;

	}

	// send request to server to remove the player from the room (game)
	auto req = cpr::Post(
		cpr::Url{ Server::GetUrl() + "/leaveRoom" },
		cpr::Payload{
			{"roomID", m_roomID},
			{"username", m_username}
		}
	);

	Menu* menu = new Menu(std::move(m_username));
	menu->show();
	ClearDrawingArea();
	this->close();
	this->deleteLater();
}

void Game::SendDrawing(const QByteArray& drawingData)
{
	auto request = cpr::Post(
		cpr::Url{ Server::GetUrl() + "/drawingImage" },
		cpr::Parameters{ {"roomID", m_roomID} },
		cpr::Body{ drawingData.toBase64() }
	);

	if (request.status_code != 200)
		SendDrawing(drawingData);
}

void Game::ReturnDrawing(std::string& drawingData)
{
	auto request = cpr::Get(
		cpr::Url{ Server::GetUrl() + "/drawingImage" },
		cpr::Payload{ {"roomID", m_roomID} }
	);

	if (request.status_code != 200)
		return ReturnDrawing(drawingData);

	drawingData = request.text;
}