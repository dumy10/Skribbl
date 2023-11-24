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
	std::vector<Word> words = storage.GetWords();
	for (auto& word : words)
	{
		std::cout << word.getId();
		std::cout << word.getWord() << std::endl;
	}
	std::cout << storage.GetRandomWord();

	Routing router;
	router.run();

	return 0;
}