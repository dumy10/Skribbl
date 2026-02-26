#pragma once
#include <crow.h>
#include <memory>
#include "database.h"
#include "GameManager.h"
#include "PlayerManager.h"
#include "RouteManager.h"

class Routing
{
public:
	Routing() = delete;
	Routing(Database& storage);
	~Routing() = default;

	void Run();

private:
	Routing(const Routing&) = delete;
	Routing& operator=(const Routing&) = delete;

private:
	Database m_storage;
	crow::SimpleApp m_app;
	
	std::unique_ptr<GameManager> m_gameManager;
	std::unique_ptr<PlayerManager> m_playerManager;
	std::unique_ptr<RouteManager> m_routeManager;
};
