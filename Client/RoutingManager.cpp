#include "RoutingManager.h"
#include "utils.h"

// Server health
cpr::Response RoutingManager::CheckServerRunning(std::string_view url)
{
	return cpr::Get(cpr::Url{ std::string(url) + "/" });
}

// User/Authentication endpoints
cpr::Response RoutingManager::CheckUsername(std::string_view username)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/checkUsername" },
		cpr::Payload{ {"username", std::string(username)} }
	);
}

cpr::Response RoutingManager::AddUser(std::string_view username, std::string_view password, std::string_view email)
{
	return cpr::Post(
		cpr::Url{ Server::GetUrl() + "/addUser" },
		cpr::Payload{ {"username", std::string(username)}, {"password", std::string(password)}, {"email", std::string(email)} }
	);
}

cpr::Response RoutingManager::LoginUser(std::string_view username, std::string_view password)
{
	return cpr::Post(
		cpr::Url{ Server::GetUrl() + "/loginUser" },
		cpr::Payload{ {"username", std::string(username)}, {"password", std::string(password)} }
	);
}

// Room management
cpr::Response RoutingManager::GetNewRoomID()
{
	return cpr::Get(cpr::Url{ Server::GetUrl() + "/roomID" });
}

cpr::Response RoutingManager::CreateRoom(std::string_view roomID, std::string_view username, std::string_view maxPlayers, std::string_view currentPlayers)
{
	return cpr::Post(
		cpr::Url{ Server::GetUrl() + "/createRoom" },
		cpr::Payload{
			{"roomID", std::string(roomID)},
			{"username", std::string(username)},
			{"maxPlayers", std::string(maxPlayers)},
			{"currentPlayers", std::string(currentPlayers)}
		}
	);
}

cpr::Response RoutingManager::CheckRoomID(std::string_view roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/checkRoomID" },
		cpr::Payload{ {"roomID", std::string(roomID)} }
	);
}

cpr::Response RoutingManager::JoinRoom(std::string_view roomID, std::string_view username)
{
	return cpr::Post(
		cpr::Url{ Server::GetUrl() + "/joinRoom" },
		cpr::Payload{ {"roomID", std::string(roomID)}, {"username", std::string(username)} }
	);
}

cpr::Response RoutingManager::LeaveRoom(std::string_view roomID, std::string_view username)
{
	return cpr::Post(
		cpr::Url{ Server::GetUrl() + "/leaveRoom" },
		cpr::Payload{ {"roomID", std::string(roomID)}, {"username", std::string(username)} }
	);
}

cpr::Response RoutingManager::GetRoomPlayers(std::string_view roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/roomPlayers" },
		cpr::Payload{ {"roomID", std::string(roomID)} }
	);
}

cpr::Response RoutingManager::GetNumberOfPlayers(std::string_view roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/numberOfPlayers" },
		cpr::Payload{ {"roomID", std::string(roomID)} }
	);
}

cpr::Response RoutingManager::GetCurrentNumberOfPlayers(std::string_view roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/currentNumberOfPlayers" },
		cpr::Payload{ {"roomID", std::string(roomID)} }
	);
}

// Game management
cpr::Response RoutingManager::StartGame(std::string_view roomID)
{
	return cpr::Post(
		cpr::Url{ Server::GetUrl() + "/startGame" },
		cpr::Payload{ {"roomID", std::string(roomID)} }
	);
}

cpr::Response RoutingManager::CheckGameStarted(std::string_view roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/gameStarted" },
		cpr::Payload{ {"roomID", std::string(roomID)} }
	);
}

cpr::Response RoutingManager::CheckGameEnded(std::string_view roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/gameEnded" },
		cpr::Payload{ {"roomID", std::string(roomID)} }
	);
}

cpr::Response RoutingManager::EndGame(std::string_view roomID)
{
	return cpr::Post(
		cpr::Url{ Server::GetUrl() + "/endGame" },
		cpr::Payload{ {"roomID", std::string(roomID)} }
	);
}

cpr::Response RoutingManager::NextRound(std::string_view roomID)
{
	return cpr::Post(
		cpr::Url{ Server::GetUrl() + "/nextRound" },
		cpr::Payload{ {"roomID", std::string(roomID)} }
	);
}

// Game state
cpr::Response RoutingManager::GetDrawingPlayer(std::string_view roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/drawingPlayer" },
		cpr::Payload{ {"roomID", std::string(roomID)} }
	);
}

cpr::Response RoutingManager::GetCurrentWord(std::string_view roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/currentWord" },
		cpr::Payload{ {"roomID", std::string(roomID)} }
	);
}

cpr::Response RoutingManager::GetRoundNumber(std::string_view roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/roundNumber" },
		cpr::Payload{ {"roomID", std::string(roomID)} }
	);
}

cpr::Response RoutingManager::GetTimeLeft(std::string_view roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/timeLeft" },
		cpr::Payload{ {"roomID", std::string(roomID)} }
	);
}

cpr::Response RoutingManager::GetPlayerScore(std::string_view roomID, std::string_view username)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/playerScore" },
		cpr::Payload{ {"roomID", std::string(roomID)}, {"username", std::string(username)} }
	);
}

cpr::Response RoutingManager::CheckAllPlayersGuessed(std::string_view roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/allPlayersGuessed" },
		cpr::Payload{ {"roomID", std::string(roomID)} }
	);
}

// Chat
cpr::Response RoutingManager::AddChat(std::string_view roomID, std::string_view username, std::string_view text)
{
	return cpr::Post(
		cpr::Url{ Server::GetUrl() + "/addChat" },
		cpr::Payload{ {"roomID", std::string(roomID)}, {"username", std::string(username)}, {"text", std::string(text)} }
	);
}

cpr::Response RoutingManager::GetChat(std::string_view roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/getChat" },
		cpr::Payload{ {"roomID", std::string(roomID)} }
	);
}

// Drawing
cpr::Response RoutingManager::SendDrawingImage(std::string_view roomID, std::string_view imageData)
{
	return cpr::Post(
		cpr::Url{ Server::GetUrl() + "/drawingImage" },
		cpr::Parameters{ {"roomID", std::string(roomID)} },
		cpr::Body{ std::string(imageData) }
	);
}

cpr::Response RoutingManager::GetDrawingImage(std::string_view roomID)
{
	return cpr::Get(
		cpr::Url{ Server::GetUrl() + "/drawingImage" },
		cpr::Payload{ {"roomID", std::string(roomID)} }
	);
}

cpr::Response RoutingManager::ClearDrawingImage(std::string_view roomID)
{
	return cpr::Post(
		cpr::Url{ Server::GetUrl() + "/clearImage" },
		cpr::Payload{ {"roomID", std::string(roomID)} }
	);
}
