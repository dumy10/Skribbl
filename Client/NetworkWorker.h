#pragma once

#include <QObject>
#include <QThread>
#include <QString>
#include <QImage>
#include <cpr/cpr.h>
#include <string>
#include <memory>
#include <unordered_map>

// Struct to hold all room information data in a single batch
struct RoomUpdateData {
	std::string drawingPlayer;
	std::string currentWord;
	std::string roundNumber;
	std::string timeLeft;
	std::string chat;
	std::string drawingImage;
	std::string players;
	std::unordered_map<std::string, std::string> playerScores;
	bool gameEnded = false;
	bool allPlayersGuessed = false;
	int currentPlayerCount = 0;
};

// Register the type so it can be used in signals/slots across threads
Q_DECLARE_METATYPE(RoomUpdateData)
Q_DECLARE_METATYPE(QImage)

class NetworkWorker : public QObject 
{
	Q_OBJECT

public:
	explicit NetworkWorker(const std::string& roomID, const std::string& username);
	~NetworkWorker() = default;

public slots:
	// Main update slot - batches all room information requests
	void FetchRoomUpdate();
	
	// Individual request slots for user actions
	void SendNetworkMessage(const QString& message);
	void SendDrawingData(const QImage& image) const noexcept;
	void LeaveRoom();
	void StartNextRound();
	void EndGameSession();

signals:
	// Signals for room update results
	void RoomUpdateReceived(const RoomUpdateData& data);
	
	// Signals for individual requests
	void MessageSent(bool success, bool correctGuess);
	void RoomLeft(bool success);
	void NextRoundStarted(bool success);
	void GameEnded(bool success);

private:
	std::string m_roomID;
	std::string m_username;
};
