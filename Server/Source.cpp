#include <filesystem>
#include <iostream>
#include <memory>

#include <sqlite_orm/sqlite_orm.h>
#include "database.h"
#include "routing.h"

import game;
import word;
import player;
import <vector>;


using namespace skribbl;
namespace sql = sqlite_orm;


int main()
{
	Database storage;
	if (!storage.Initialize())
	{
		std::cout << "Failed to initialize database" << std::endl;
		return -1;

	}

	Routing router;
	router.run(storage);

	return 0;
}