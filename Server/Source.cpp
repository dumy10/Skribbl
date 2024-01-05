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

	try 
	{
		Routing router;
		router.Run(storage);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	

	return 0;
}