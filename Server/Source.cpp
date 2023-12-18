#include <filesystem>
#include <iostream>
#include <memory>

#include <sqlite_orm/sqlite_orm.h>
#include "database.h"
#include "routing.h"

import game;
import player;


int main()
{
	Database storage;

	if (!storage.Initialize())
	{
		std::cout << "Failed to initialize database" << std::endl;
		return -1;
	}

	std::vector<Player> players = storage.GetPlayers(); // leave this for debugging
	std::vector<Game> games = storage.GetGames(); // leave this for debugging

	Routing router;
	router.Run(storage);

	return 0;
}