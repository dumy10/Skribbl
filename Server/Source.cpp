#include "routing.h"

int main()
{
	Database storage{};

	try
	{
		if (!storage.Initialize())
		{
			std::cerr << "Failed to initialize database" << std::endl;
			return -1;
		}

		try
		{
			Routing router{ storage };
			router.Run();
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