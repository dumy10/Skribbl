#pragma once
#include <crow.h>
#include "database.h"
#include "GameManager.h"
#include "PlayerManager.h"

class RouteManager
{
public:
	RouteManager(crow::SimpleApp& app, Database& storage, GameManager& gameManager, PlayerManager& playerManager);

	void RegisterRoutes();

private:
	// Simple route handlers (no manager needed)
	crow::response HandleRoot();
	crow::response HandleRandomWord();

private:
	crow::SimpleApp& m_app;
	Database& m_storage;
	GameManager& m_gameManager;
	PlayerManager& m_playerManager;
};
