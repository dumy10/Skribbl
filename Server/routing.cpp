#include "routing.h"

Routing::Routing(Database& storage)
	: m_storage{ storage }
	, m_gameManager{ std::make_unique<GameManager>(m_storage) }
	, m_playerManager{ std::make_unique<PlayerManager>(m_storage) }
	, m_routeManager{ std::make_unique<RouteManager>(m_app, m_storage, *m_gameManager, *m_playerManager) }
{
}

void Routing::Run()
{
	m_routeManager->RegisterRoutes();
	m_app.bindaddr("127.0.0.1").port(18080).multithreaded().run();
}
