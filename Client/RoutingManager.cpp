#include "RoutingManager.h"
#include "utils.h"

// Server health
cpr::Response RoutingManager::CheckServerRunning(const std::string& url)
{
	return cpr::Get(cpr::Url{ url + "/" });
}

// User/Authentication endpoints
cpr::Response RoutingManager::CheckUsername(const std::string& username)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/checkUsername" },
		cpr::Payload{ {"username", username} }
	);
}

cpr::Response RoutingManager::AddUser(const std::string& username, const std::string& password, const std::string& email)
{
	return cpr::Post(
		cpr::Url{ Server::GetUrl() + "/addUser" },
		cpr::Payload{ {"username", username}, {"password", password}, {"email", email} }
	);
}

cpr::Response RoutingManager::LoginUser(const std::string& username, const std::string& password)
{
	return cpr::Post(
		cpr::Url{ Server::GetUrl() + "/loginUser" },
		cpr::Payload{ {"username", username}, {"password", password} }
	);
}

// Room management
cpr::Response RoutingManager::GetNewRoomID()
{
	return cpr::Get(cpr::Url{ Server::GetUrl() + "/roomID" });
}

cpr::Response RoutingManager::CreateRoom(const std::string& roomID, const std::string& username, const std::string& maxPlayers, const std::string& currentPlayers)
{
	return cpr::Post(
		cpr::Url{ Server::GetUrl() + "/createRoom" },
		cpr::Payload{
			{"roomID", roomID},
			{"username", username},
			{"maxPlayers", maxPlayers},
			{"currentPlayers", currentPlayers}
		}
	);
}

cpr::Response RoutingManager::CheckRoomID(const std::string& roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/checkRoomID" },
		cpr::Payload{ {"roomID", roomID} }
	);
}

cpr::Response RoutingManager::JoinRoom(const std::string& roomID, const std::string& username)
{
	return cpr::Post(
		cpr::Url{ Server::GetUrl() + "/joinRoom" },
		cpr::Payload{ {"roomID", roomID}, {"username", username} }
	);
}

cpr::Response RoutingManager::LeaveRoom(const std::string& roomID, const std::string& username)
{
	return cpr::Post(
		cpr::Url{ Server::GetUrl() + "/leaveRoom" },
		cpr::Payload{ {"roomID", roomID}, {"username", username} }
	);
}

cpr::Response RoutingManager::GetRoomPlayers(const std::string& roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/roomPlayers" },
		cpr::Payload{ {"roomID", roomID} }
	);
}

cpr::Response RoutingManager::GetNumberOfPlayers(const std::string& roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/numberOfPlayers" },
		cpr::Payload{ {"roomID", roomID} }
	);
}

cpr::Response RoutingManager::GetCurrentNumberOfPlayers(const std::string& roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/currentNumberOfPlayers" },
		cpr::Payload{ {"roomID", roomID} }
	);
}

// Game management
cpr::Response RoutingManager::StartGame(const std::string& roomID)
{
	return cpr::Post(
		cpr::Url{ Server::GetUrl() + "/startGame" },
		cpr::Payload{ {"roomID", roomID} }
	);
}

cpr::Response RoutingManager::CheckGameStarted(const std::string& roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/gameStarted" },
		cpr::Payload{ {"roomID", roomID} }
	);
}

cpr::Response RoutingManager::CheckGameEnded(const std::string& roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/gameEnded" },
		cpr::Payload{ {"roomID", roomID} }
	);
}

cpr::Response RoutingManager::EndGame(const std::string& roomID)
{
	return cpr::Post(
		cpr::Url{ Server::GetUrl() + "/endGame" },
		cpr::Payload{ {"roomID", roomID} }
	);
}

cpr::Response RoutingManager::NextRound(const std::string& roomID)
{
	return cpr::Post(
		cpr::Url{ Server::GetUrl() + "/nextRound" },
		cpr::Payload{ {"roomID", roomID} }
	);
}

// Game state
cpr::Response RoutingManager::GetDrawingPlayer(const std::string& roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/drawingPlayer" },
		cpr::Payload{ {"roomID", roomID} }
	);
}

cpr::Response RoutingManager::GetCurrentWord(const std::string& roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/currentWord" },
		cpr::Payload{ {"roomID", roomID} }
	);
}

cpr::Response RoutingManager::GetRoundNumber(const std::string& roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/roundNumber" },
		cpr::Payload{ {"roomID", roomID} }
	);
}

cpr::Response RoutingManager::GetTimeLeft(const std::string& roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/timeLeft" },
		cpr::Payload{ {"roomID", roomID} }
	);
}

cpr::Response RoutingManager::GetPlayerScore(const std::string& roomID, const std::string& username)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/playerScore" },
		cpr::Payload{ {"roomID", roomID}, {"username", username} }
	);
}

cpr::Response RoutingManager::CheckAllPlayersGuessed(const std::string& roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/allPlayersGuessed" },
		cpr::Payload{ {"roomID", roomID} }
	);
}

// Chat
cpr::Response RoutingManager::AddChat(const std::string& roomID, const std::string& username, const std::string& text)
{
	return cpr::Post(
		cpr::Url{ Server::GetUrl() + "/addChat" },
		cpr::Payload{ {"roomID", roomID}, {"username", username}, {"text", text} }
	);
}

cpr::Response RoutingManager::GetChat(const std::string& roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/getChat" },
		cpr::Payload{ {"roomID", roomID} }
	);
}

// Drawing
cpr::Response RoutingManager::SendDrawingImage(const std::string& roomID, const std::string& imageData)
{
	return cpr::Post(
		cpr::Url{ Server::GetUrl() + "/drawingImage" },
		cpr::Parameters{ {"roomID", roomID} },
		cpr::Body{ imageData }
	);
}

cpr::Response RoutingManager::GetDrawingImage(const std::string& roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/drawingImage" },
		cpr::Payload{ {"roomID", roomID} }
	);
}

cpr::Response RoutingManager::ClearDrawingImage(const std::string& roomID)
{
	return cpr::Post(
		cpr::Url{ Server::GetUrl() + "/clearImage" },
		cpr::Payload{ {"roomID", roomID} }
	);
}
