#include <filesystem>
#include <iostream>
#include <memory>

#include <sqlite_orm/sqlite_orm.h>
#include "database.h"
#include "routing.h"

int main()
{
	Database storage;

	try
	{
		if (!storage.Initialize())
		{
			std::cerr << "Failed to initialize database" << std::endl;
			return -1;
		}

		try
		{
			Routing router;
			router.Run(storage);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return -1;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}