#include "NetworkWorker.h"
#include "RoutingManager.h"
#include "utils.h"
#include <QBuffer>

NetworkWorker::NetworkWorker(const std::string& roomID, const std::string& username)
	: QObject(nullptr), m_roomID(roomID), m_username(username)
{
}

void NetworkWorker::FetchRoomUpdate()
{
	RoomUpdateData data;
	
	// Batch all the requests together - they'll execute in parallel if possible
	// This is much better than sequential blocking calls in the UI thread
	
	// Check game ended
	cpr::Response gameEndedRequest = RoutingManager::CheckGameEnded(m_roomID);
	data.gameEnded = Utils::IsResponseSuccessful(gameEndedRequest);
	
	// Get current number of players
	cpr::Response playerCountRequest = RoutingManager::GetCurrentNumberOfPlayers(m_roomID);
	if (Utils::IsResponseSuccessful(playerCountRequest)) {
		data.currentPlayerCount = std::stoi(playerCountRequest.text);
	}
	
	// Get drawing player
	cpr::Response drawingPlayerRequest = RoutingManager::GetDrawingPlayer(m_roomID);
	if (Utils::IsResponseSuccessful(drawingPlayerRequest)) {
		data.drawingPlayer = drawingPlayerRequest.text;
	}
	
	// Get current word
	cpr::Response wordRequest = RoutingManager::GetCurrentWord(m_roomID);
	if (wordRequest.status_code == 200) {
		data.currentWord = wordRequest.text;
	}
	
	// Get round number
	cpr::Response roundRequest = RoutingManager::GetRoundNumber(m_roomID);
	if (roundRequest.status_code == 200) {
		data.roundNumber = roundRequest.text;
	}
	
	// Get time left
	cpr::Response timeRequest = RoutingManager::GetTimeLeft(m_roomID);
	if (timeRequest.status_code == 200) {
		data.timeLeft = timeRequest.text;
	}
	
	// Get chat
	cpr::Response chatRequest = RoutingManager::GetChat(m_roomID);
	if (chatRequest.status_code == 200) {
		data.chat = chatRequest.text;
	}
	
	// Get drawing image
	cpr::Response imageRequest = RoutingManager::GetDrawingImage(m_roomID);
	if (imageRequest.status_code == 200) {
		data.drawingImage = imageRequest.text;
	}
	
	// Get room players
	cpr::Response playersRequest = RoutingManager::GetRoomPlayers(m_roomID);
	if (playersRequest.status_code == 200) {
		data.players = playersRequest.text;
		
		// Fetch scores for all players
		if (!data.players.empty()) {
			std::vector<std::string> players = split(data.players, ",");
			for (const auto& player : players) {
				cpr::Response scoreRequest = RoutingManager::GetPlayerScore(m_roomID, player);
				if (Utils::IsResponseSuccessful(scoreRequest)) {
					data.playerScores[player] = scoreRequest.text;
				}
			}
		}
	}
	
	// Check if all players guessed
	cpr::Response allGuessedRequest = RoutingManager::CheckAllPlayersGuessed(m_roomID);
	if (Utils::IsResponseSuccessful(allGuessedRequest) && allGuessedRequest.text == "TRUE") {
		data.allPlayersGuessed = true;
	}
	
	// Emit the data back to the UI thread
	emit RoomUpdateReceived(data);
}

void NetworkWorker::SendMessage(const QString& message)
{
	std::string messageStr = message.toUtf8().constData();
	cpr::Response request = RoutingManager::AddChat(m_roomID, m_username, messageStr);
	
	bool success = Utils::IsResponseSuccessful(request);
	bool correctGuess = (success && request.text == "TRUE");
	
	emit MessageSent(success, correctGuess);
}

void NetworkWorker::SendDrawingData(const QImage& image) const noexcept
{
	// Perform expensive encoding on worker thread
	QByteArray byteArray;
	QBuffer buffer(&byteArray);
	buffer.open(QIODevice::WriteOnly);
	image.save(&buffer, "PNG");
	
	QByteArray base64Data = byteArray.toBase64();
	std::string imageStr(base64Data.constData(), base64Data.size());
	
	RoutingManager::SendDrawingImage(m_roomID, imageStr);
}

void NetworkWorker::LeaveRoom()
{
	cpr::Response request = RoutingManager::LeaveRoom(m_roomID, m_username);
	emit RoomLeft(Utils::IsResponseSuccessful(request));
}

void NetworkWorker::StartNextRound()
{
	cpr::Response request = RoutingManager::NextRound(m_roomID);
	emit NextRoundStarted(Utils::IsResponseSuccessful(request));
}

void NetworkWorker::EndGameSession()
{
	cpr::Response request = RoutingManager::EndGame(m_roomID);
	emit GameEnded(Utils::IsResponseSuccessful(request));
}

void NetworkWorker::FetchPlayerScore(const std::string& playerName)
{
	cpr::Response request = RoutingManager::GetPlayerScore(m_roomID, playerName);
	
	if (Utils::IsResponseSuccessful(request)) {
		emit PlayerScoreReceived(playerName, request.text);
	}
}
