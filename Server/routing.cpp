#include "routing.h"
import player;

using namespace skribbl;



void Routing::run(Database& storage)
{

	CROW_ROUTE(m_app, "/")([]() {
		return "Hello, Skribbl World!"; });
	CROW_ROUTE(m_app, "/randomWord")([&]() {
		return crow::response{ storage.GetRandomWord() };
		});

	CROW_ROUTE(m_app, "/checkUsername")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string username = x["username"];

		if (username == "")
			return crow::response{ 404, "Username not found." };

		if (storage.CheckUsername(username))
			return crow::response{ "true" };

		return crow::response{ 409, "Username does not exist!" };
			});

	CROW_ROUTE(m_app, "/addUser")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string username = x["username"];
		std::string password = x["password"];
		std::string email = x["email"];

		if (storage.AddUser(username, password, email))
			return crow::response{ "true" };

		return crow::response{ "false" };
			});

	CROW_ROUTE(m_app, "/loginUser")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string username = x["username"];
		std::string password = x["password"];

		if (!storage.CheckUsername(username))
			return crow::response{ 409, "Username does not exist!" };

		if (!storage.CheckPassword(username, password))
			return crow::response{ 409, "Wrong password!" };

		return crow::response{ 200 };

			});

	CROW_ROUTE(m_app, "/roomID")
		.methods("GET"_method, "POST"_method)([&]() {
		return crow::response{ storage.GetRandomID() };
			});
	CROW_ROUTE(m_app, "/sendString")
		.methods("POST"_method)
		([](const crow::request& req) {
		auto json = crow::json::load(req.body);
		if (!json) {
			return crow::response(400);
		}

		std::string receivedString = json["message"].s();
		std::cout << "Server received: " << receivedString << std::endl;

		return crow::response("Server received: " + receivedString);
			});

	m_app.port(18080).multithreaded().run();
}
