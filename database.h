#pragma once
#include <string>
#include <vector>

#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>
namespace sql = sqlite_orm;

import utils;
import player;

using namespace skribbl;

/*
Todo:
A table with the words should be created that should be initiallized with the words from the file

*/


inline auto CreateStorage(const std::string& filename)
{
	return sql::make_storage(
		filename,
		sql::make_table(
			"Players",
			sql::make_column("id", &Player::setId, &Player::getId, sql::primary_key().autoincrement()),
			sql::make_column("name", &Player::setName, &Player::getName),
			sql::make_column("password", &Player::setPassword, &Player::getPassword),
			sql::make_column("email", &Player::setEmail, &Player::getEmail),
			sql::make_column("score", &Player::setPoints, &Player::getPoints)
			)
	);
}

using Storage = decltype(CreateStorage(""));