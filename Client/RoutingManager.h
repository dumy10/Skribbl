#pragma once
#include <string>
#include <string_view>
#include <cpr/cpr.h>

class RoutingManager
{
public:
	// Server health
	static cpr::Response CheckServerRunning(std::string_view url);

	// User/Authentication endpoints
	static cpr::Response CheckUsername(std::string_view username);
	static cpr::Response AddUser(std::string_view username, std::string_view password, std::string_view email);
	static cpr::Response LoginUser(std::string_view username, std::string_view password);

	// Room management
	static cpr::Response GetNewRoomID();
	static cpr::Response CreateRoom(std::string_view roomID, std::string_view username, std::string_view maxPlayers, std::string_view currentPlayers);
	static cpr::Response CheckRoomID(std::string_view roomID);
	static cpr::Response JoinRoom(std::string_view roomID, std::string_view username);
	static cpr::Response LeaveRoom(std::string_view roomID, std::string_view username);
	static cpr::Response GetRoomPlayers(std::string_view roomID);
	static cpr::Response GetNumberOfPlayers(std::string_view roomID);
	static cpr::Response GetCurrentNumberOfPlayers(std::string_view roomID);

	// Game management
	static cpr::Response StartGame(std::string_view roomID);
	static cpr::Response CheckGameStarted(std::string_view roomID);
	static cpr::Response CheckGameEnded(std::string_view roomID);
	static cpr::Response EndGame(std::string_view roomID);
	static cpr::Response NextRound(std::string_view roomID);

	// Game state
	static cpr::Response GetDrawingPlayer(std::string_view roomID);
	static cpr::Response GetCurrentWord(std::string_view roomID);
	static cpr::Response GetRoundNumber(std::string_view roomID);
	static cpr::Response GetTimeLeft(std::string_view roomID);
	static cpr::Response GetPlayerScore(std::string_view roomID, std::string_view username);
	static cpr::Response CheckAllPlayersGuessed(std::string_view roomID);

	// Chat
	static cpr::Response AddChat(std::string_view roomID, std::string_view username, std::string_view text);
	static cpr::Response GetChat(std::string_view roomID);

	// Drawing
	static cpr::Response SendDrawingImage(std::string_view roomID, std::string_view imageData);
	static cpr::Response GetDrawingImage(std::string_view roomID);
	static cpr::Response ClearDrawingImage(std::string_view roomID);
};
