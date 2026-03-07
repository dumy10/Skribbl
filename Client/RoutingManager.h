#pragma once
#include <string>
#include <cpr/cpr.h>

class RoutingManager
{
public:
	// Server health
	static cpr::Response CheckServerRunning(const std::string& url);

	// User/Authentication endpoints
	static cpr::Response CheckUsername(const std::string& username);
	static cpr::Response AddUser(const std::string& username, const std::string& password, const std::string& salt, const std::string& email);
	static cpr::Response LoginUser(const std::string& username, const std::string& password);
	static cpr::Response LogoutUser(const std::string& username);

	// Room management
	static cpr::Response GetNewRoomID();
	static cpr::Response CreateRoom(const std::string& roomID, const std::string& username, const std::string& maxPlayers, const std::string& currentPlayers);
	static cpr::Response CheckRoomID(const std::string& roomID);
	static cpr::Response JoinRoom(const std::string& roomID, const std::string& username);
	static cpr::Response LeaveRoom(const std::string& roomID, const std::string& username);
	static cpr::Response GetRoomPlayers(const std::string& roomID);
	static cpr::Response GetNumberOfPlayers(const std::string& roomID);
	static cpr::Response GetCurrentNumberOfPlayers(const std::string& roomID);

	// Game management
	static cpr::Response StartGame(const std::string& roomID);
	static cpr::Response CheckGameStarted(const std::string& roomID);
	static cpr::Response CheckGameEnded(const std::string& roomID);
	static cpr::Response EndGame(const std::string& roomID);
	static cpr::Response NextRound(const std::string& roomID);

	// Game state
	static cpr::Response GetDrawingPlayer(const std::string& roomID);
	static cpr::Response GetCurrentWord(const std::string& roomID);
	static cpr::Response GetRoundNumber(const std::string& roomID);
	static cpr::Response GetTimeLeft(const std::string& roomID);
	static cpr::Response GetPlayerScore(const std::string& roomID, const std::string& username);
	static cpr::Response CheckAllPlayersGuessed(const std::string& roomID);

	// Chat
	static cpr::Response AddChat(const std::string& roomID, const std::string& username, const std::string& text);
	static cpr::Response GetChat(const std::string& roomID);

	// Drawing
	static cpr::Response SendDrawingImage(const std::string& roomID, const std::string& imageData);
	static cpr::Response GetDrawingImage(const std::string& roomID);
	static cpr::Response ClearDrawingImage(const std::string& roomID);
};
