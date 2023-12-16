#include <filesystem>
#include <iostream>
#include <memory>

#include <sqlite_orm/sqlite_orm.h>
#include "database.h"
#include "routing.h"


int main()
{
	Database storage;

	if (!storage.Initialize())
	{
		std::cout << "Failed to initialize database" << std::endl;
		return -1;
	}

	Routing router;
	router.Run(storage);

	return 0;
}