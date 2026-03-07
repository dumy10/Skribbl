#include "RouteManager.h"

RouteManager::RouteManager(crow::SimpleApp& app, Database& storage, GameManager& gameManager, PlayerManager& playerManager)
	: m_app{ app }
	, m_storage{ storage }
	, m_gameManager{ gameManager }
	, m_playerManager{ playerManager }
{
}

crow::response RouteManager::HandleRoot()
{
	return crow::response{ "Hello, Skribbl World!" };
}

crow::response RouteManager::HandleRandomWord()
{
	return crow::response{ 200, m_storage.GetRandomWord() };
}

void RouteManager::RegisterRoutes()
{
	// Test route
	CROW_ROUTE(m_app, "/")([this]() { return HandleRoot(); });

	// Word management
	CROW_ROUTE(m_app, "/randomWord")([this]() { return HandleRandomWord(); });

	// User authentication routes
	CROW_ROUTE(m_app, "/checkUsername").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return m_playerManager.HandleCheckUsername(req); });
	CROW_ROUTE(m_app, "/addUser").methods(crow::HTTPMethod::Post)([this](const crow::request& req) { return m_playerManager.HandleAddUser(req); });
	CROW_ROUTE(m_app, "/loginUser").methods(crow::HTTPMethod::Post)([this](const crow::request& req) { return m_playerManager.HandleLoginUser(req); });
	CROW_ROUTE(m_app, "/logoutUser").methods(crow::HTTPMethod::Post)([this](const crow::request& req) { return m_playerManager.HandleLogoutUser(req); });

	// Room management routes
	CROW_ROUTE(m_app, "/roomID").methods(crow::HTTPMethod::Get)([this]() { return m_gameManager.HandleGetRoomID(); });
	CROW_ROUTE(m_app, "/createRoom").methods(crow::HTTPMethod::Post)([this](const crow::request& req) { return m_gameManager.HandleCreateRoom(req, m_playerManager); });
	CROW_ROUTE(m_app, "/checkRoomID").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return m_gameManager.HandleCheckRoomID(req); });
	CROW_ROUTE(m_app, "/joinRoom").methods(crow::HTTPMethod::Post)([this](const crow::request& req) { return m_gameManager.HandleJoinRoom(req, m_playerManager); });
	CROW_ROUTE(m_app, "/leaveRoom").methods(crow::HTTPMethod::Post)([this](const crow::request& req) { return m_gameManager.HandleLeaveRoom(req, m_playerManager); });
	CROW_ROUTE(m_app, "/roomPlayers").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return m_gameManager.HandleGetRoomPlayers(req); });
	CROW_ROUTE(m_app, "/numberOfPlayers").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return m_gameManager.HandleGetNumberOfPlayers(req); });
	CROW_ROUTE(m_app, "/currentNumberOfPlayers").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return m_gameManager.HandleGetCurrentNumberOfPlayers(req); });

	// Game state routes
	CROW_ROUTE(m_app, "/gameStarted").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return m_gameManager.HandleGameStarted(req); });
	CROW_ROUTE(m_app, "/startGame").methods(crow::HTTPMethod::Post)([this](const crow::request& req) { return m_gameManager.HandleStartGame(req); });
	CROW_ROUTE(m_app, "/gameEnded").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return m_gameManager.HandleGameEnded(req); });
	CROW_ROUTE(m_app, "/endGame").methods(crow::HTTPMethod::Post)([this](const crow::request& req) { return m_gameManager.HandleEndGame(req); });

	// Game play routes
	CROW_ROUTE(m_app, "/playerScore").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return m_gameManager.HandleGetPlayerScore(req); });
	CROW_ROUTE(m_app, "/drawingPlayer").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return m_gameManager.HandleGetDrawingPlayer(req); });
	CROW_ROUTE(m_app, "/roundNumber").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return m_gameManager.HandleGetRoundNumber(req); });
	CROW_ROUTE(m_app, "/currentWord").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return m_gameManager.HandleGetCurrentWord(req); });
	CROW_ROUTE(m_app, "/timeLeft").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return m_gameManager.HandleGetTimeLeft(req); });
	CROW_ROUTE(m_app, "/nextRound").methods(crow::HTTPMethod::Post)([this](const crow::request& req) { return m_gameManager.HandleNextRound(req); });
	CROW_ROUTE(m_app, "/allPlayersGuessed").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return m_gameManager.HandleAllPlayersGuessed(req); });

	// Chat routes
	CROW_ROUTE(m_app, "/addChat").methods(crow::HTTPMethod::Post)([this](const crow::request& req) { return m_gameManager.HandleAddChat(req); });
	CROW_ROUTE(m_app, "/getChat").methods(crow::HTTPMethod::Get)([this](const crow::request& req) { return m_gameManager.HandleGetChat(req); });

	// Drawing routes
	CROW_ROUTE(m_app, "/drawingImage").methods(crow::HTTPMethod::Get, crow::HTTPMethod::Post)([this](const crow::request& req) { return m_gameManager.HandleDrawingImage(req); });
	CROW_ROUTE(m_app, "/clearImage").methods(crow::HTTPMethod::Post)([this](const crow::request& req) { return m_gameManager.HandleClearImage(req); });
}
