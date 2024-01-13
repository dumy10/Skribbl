#include "routing.h"
import player;
import game;
using namespace skribbl;


void Routing::Run(Database& storage)
{
	// Just a test route
	CROW_ROUTE(m_app, "/")([]() {
		return "Hello, Skribbl World!"; });

	// Gets a random word from the database
	CROW_ROUTE(m_app, "/randomWord")([&]() {
		return crow::response{ storage.GetRandomWord() };
		});
	
	// Checks if the username exists in the database
	CROW_ROUTE(m_app, "/checkUsername")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string username = x["username"];

		if (username == "")
			return crow::response{ 404, "Username not found." };

		std::transform(username.begin(), username.end(), username.begin(), ::tolower);

		if (storage.CheckUsername(username))
			return crow::response{ "true" };

		return crow::response{ 409, "Username does not exist!" };
			});

	// Adds a user to the database
	CROW_ROUTE(m_app, "/addUser")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string username = x["username"];
		std::string password = x["password"];
		std::string email = x["email"];

		if (username == "" || password == "" || email == "")
			return crow::response{ 404, "Username, password or email not found." };
		std::transform(username.begin(), username.end(), username.begin(), ::tolower);
		if (storage.AddUser(username, password, email))
			return crow::response{ "true" };

		return crow::response{ "false" };
			});

	// Checks if the username and password match in the database
	CROW_ROUTE(m_app, "/loginUser")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string username = x["username"];
		std::string password = x["password"];

		if (username == "" || password == "")
			return crow::response{ 404, "Username or password not found." };
		std::transform(username.begin(), username.end(), username.begin(), ::tolower);
		if (!storage.CheckUsername(username))
			return crow::response{ 409, "Username does not exist!" };

		if (!storage.CheckPassword(username, password))
			return crow::response{ 409, "Wrong password!" };

		return crow::response{ 200 };

			});

	// Gets a random roomID from the database
	CROW_ROUTE(m_app, "/roomID")
		.methods("GET"_method, "POST"_method)([&]() {
		std::string roomID = storage.GetRandomID();

		while (storage.CheckRoomID(roomID) && storage.GetGame(roomID).GetGameStatusAsInt() != 3)
			roomID = storage.GetRandomID();

		return crow::response{ roomID };
			});

	// Creates a game in the database
	CROW_ROUTE(m_app, "/createRoom")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["gameCode"];
		std::string username = x["username"];
		int maxPlayers = std::stoi(x["maxPlayers"]);
		int currentPlayers = std::stoi(x["currentPlayers"]);

		std::transform(username.begin(), username.end(), username.begin(), ::tolower);
		Player player = storage.GetPlayer(username);

		if (!storage.AddGame(player, roomID, maxPlayers))
			return crow::response{ 409, "Error creating the game." };
		return crow::response{ 200 };
			});

	// Checks if the roomID exists in the database
	CROW_ROUTE(m_app, "/checkRoomID")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];

		if (!storage.CheckRoomID(roomID))
			return crow::response{ 409, "Game not found" };

		if (storage.GetGame(roomID).GetGameStatusAsInt() != 1)
			return crow::response{ 409, "Game already started" };

		if (storage.GetGame(roomID).GetCurrentPlayers() >= storage.GetGame(roomID).GetMaxPlayers())
			return crow::response{ 409, "Game is full" };

		std::string currentPlayers = std::to_string(storage.GetGame(roomID).GetCurrentPlayers());
		return crow::response{ 200 , currentPlayers };
			});

	// Adds a player to the game in the database
	CROW_ROUTE(m_app, "/joinRoom")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];
		std::string username = x["username"];

		std::transform(username.begin(), username.end(), username.begin(), ::tolower);
		Player player = storage.GetPlayer(username);
		int currentPlayers = std::stoi(x["currentPlayers"]);

		if (!storage.AddPlayerToGame(player, roomID, currentPlayers))
			return crow::response{ 409, "Error joining the game." };

		return crow::response{ 200 };
			});

	// Gets the players from the game in the database
	CROW_ROUTE(m_app, "/roomPlayers")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];

		std::string players = storage.GetGame(roomID).SerializePlayers();

		return crow::response{ players };
			});

	// Gets the max number of players from the game in the database
	CROW_ROUTE(m_app, "/numberOfPlayers")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];

		std::string numberOfPlayers = std::to_string(storage.GetGame(roomID).GetMaxPlayers());

		return crow::response{ numberOfPlayers };
			});

	// Gets the current number of players from the game in the database
	CROW_ROUTE(m_app, "/currentNumberOfPlayers")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];

		std::string currentNumberOfPlayers = std::to_string(storage.GetGame(roomID).GetCurrentPlayers());

		return crow::response{ currentNumberOfPlayers };
			});

	// Removes a player from the game in the database
	CROW_ROUTE(m_app, "/leaveRoom")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];
		std::string username = x["username"];

		std::transform(username.begin(), username.end(), username.begin(), ::tolower);
		Player player = storage.GetPlayer(username);

		if (!storage.RemovePlayerFromGame(player, roomID))
			return crow::response{ 409, "Error leaving the game." };
		return crow::response{ 200 };
			});

	// Checks if the game has started in the database
	CROW_ROUTE(m_app, "/gameStarted")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];

		if (storage.GetGame(roomID).GetGameStatusAsInt() == 2)
			return crow::response{ 200 };

		return crow::response{ 409 };
			});

	// Starts the game based on the roomID in the database
	CROW_ROUTE(m_app, "/startGame")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];

		std::vector<Player> players = storage.GetGame(roomID).GetPlayers();

		// Set the players score to 0 at the start of the game
		for (auto& player : players)
			storage.SetPlayerScore(player.GetName(), 0);

		Game currentGame = storage.GetGame(roomID);
		Round currentRound = storage.GetRound(roomID);
		int noOfWords = currentGame.GetPlayers().size() * currentGame.GetNoOfRounds(); // Calculate the number of words based on the number of players and the number of rounds

		std::set<std::string> words;
		while (words.size() < noOfWords) // Gets n different words from the database
			words.insert(storage.GetRandomWord());

		// Sets the first round
		currentRound.SetCurrentWord(*words.begin());
		words.erase(words.begin());
		currentRound.SetWords(words);
		currentRound.SetRoundNumber(1);
		currentRound.SetDrawingPlayer(currentGame.GetPlayers()[0].GetName());
		storage.Update(currentRound);

		// Set the game status to 2 (in progress)
		if (!storage.SetGameStatus(roomID, 2))
			return crow::response{ 409, "Error starting the game." };

		return crow::response{ 200 };
			});

	// Gets the player score from the database based on the username
	CROW_ROUTE(m_app, "/playerScore")
		.methods("GET"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string username = x["username"];
		std::string roomID = x["roomID"];

		std::transform(username.begin(), username.end(), username.begin(), ::tolower);
		std::string score = std::to_string(storage.GetPlayerScore(username));

		return crow::response{ score };
			});

	// Adds the text that the player wrote to the current chat in the database
	CROW_ROUTE(m_app, "/addChat")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];
		std::string username = x["username"];
		std::string text = x["text"];

		std::string currentChat = storage.GetGame(roomID).GetChat();
		std::string currentWord = storage.GetRound(roomID).GetCurrentWord();
		std::transform(text.begin(), text.end(), text.begin(), ::tolower);
		std::transform(currentWord.begin(), currentWord.end(), currentWord.begin(), ::tolower);
		
		int timeLeft = storage.GetRound(roomID).GetTimeLeft();
		int score{ 0 };

		if (timeLeft >= 30)
			score = 100;
		else if (timeLeft > 0 && timeLeft < 30)
			score = ((60 - timeLeft) * 100) / 30;

		std::transform(username.begin(), username.end(), username.begin(), ::tolower);

		Player player = storage.GetPlayer(username);
		if (text == currentWord)
		{
			// Add the timelft in an array in order to calculate the average time
			Round currentRound = storage.GetRound(roomID);
			int index = storage.GetGame(roomID).GetPlayerIndex(username);
			
			if(index == -1)
				return crow::response{ 409, "Error adding the chat." };

			currentRound.UpdateTimes(index, timeLeft);
			storage.Update(currentRound);

			// Add the points to the player
			player.AddPoints(score);
			storage.Update(player);

			currentChat += username + " guessed the word!\n";
			if (!storage.SetGameChat(roomID, currentChat))
				return crow::response{ 409, "Error adding the chat." };
			return crow::response{ 200 , "TRUE" };
		}
		else
		{
			currentChat += username + ": " + text + "\n";

			if (!storage.SetGameChat(roomID, currentChat))
				return crow::response{ 409, "Error adding the chat." };

			return crow::response{ 200 };
		}
			});

	// Gets the current chat from the database
	CROW_ROUTE(m_app, "/getChat")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];
		std::string chat = storage.GetGame(roomID).GetChat();

		return crow::response{ chat };
			});

	// Gets the current drawing player name from the database
	CROW_ROUTE(m_app, "/drawingPlayer")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];
		std::string drawer = storage.GetRound(roomID).GetDrawingPlayer();

		return crow::response{ drawer };
			});

	// Gets the current round number from the database
	CROW_ROUTE(m_app, "/roundNumber")
		.methods("GET"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];
		std::string roundNumber = std::to_string(storage.GetRound(roomID).GetRoundNumber()) + "/" + std::to_string(storage.GetGame(roomID).GetNoOfRounds());

		return crow::response{ roundNumber };
			});

	// Gets the current word that needs to be drawn from the database
	CROW_ROUTE(m_app, "/currentWord")
		.methods("GET"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];
		std::string currentWord = storage.GetRound(roomID).GetCurrentWord();

		return crow::response{ currentWord };
			});

	// Updates the time left from the owner of the game and gets the time left from the database
	CROW_ROUTE(m_app, "/timeLeft")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];

		if (req.method == crow::HTTPMethod::POST)
		{
			std::string timeLeft = x["timer"];
			Round currentRound = storage.GetRound(roomID);
			currentRound.SetTimeLeft(std::stoi(timeLeft));
			storage.Update(currentRound);
			return crow::response{ 200 };
		}
		else if (req.method == crow::HTTPMethod::GET)
		{
			std::string timeLeft = std::to_string(storage.GetRound(roomID).GetTimeLeft());
			return crow::response{ timeLeft };
		}
		else
			return crow::response{ 404 };

			});

	// Sets the next round in the database
	CROW_ROUTE(m_app, "/nextRound")
		.methods("POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];

		Round currentRound = storage.GetRound(roomID);
		std::set<std::string> words = currentRound.GetWords();
		std::vector<Player> players = storage.GetGame(roomID).GetPlayers();
		std::string currentDrawingPlayer = currentRound.GetDrawingPlayer();

		// Calculate the average time on the current round in order to calculate the points to the drawer
		int averageTime{ 0 };
		int points{ 0 };
		int currentDrawingPlayerIndex = storage.GetGame(roomID).GetPlayerIndex(currentDrawingPlayer);

		for (uint8_t index = 0; index < currentRound.GetTimes().size(); index++)
			if (index != currentDrawingPlayerIndex)
				averageTime = averageTime + (60 - currentRound.GetTimes()[index]);

		averageTime /= players.size() - 1;

		if (averageTime == ((players.size() - 1) * 60))
			points = -100;
		else
			points = ((60 - averageTime) * 100) / 60;

		Player drawer = storage.GetPlayer(currentDrawingPlayer);

		drawer.AddPoints(points); // Add the points to the drawer
		storage.Update(drawer);

		// If it is the last round and the last player is the drawer, the timer ended so the game ends
		if (currentRound.GetRoundNumber() == storage.GetGame(roomID).GetNoOfRounds() && players[players.size() - 1].GetName() == currentDrawingPlayer)
		{
			storage.SetGameStatus(roomID, 3);
			return crow::response{ 200 };
		}

		// Set the average times to 0 for the next round
		for(auto& time : currentRound.GetTimes())
			time = 0;

		// Update the round
		for (uint8_t index = 0; index < players.size(); index++)
		{
			if (players[index].GetName() == currentDrawingPlayer)
			{

				if (index == players.size() - 1)
				{
					uint8_t roundNumber = currentRound.GetRoundNumber();
					roundNumber++;
					currentRound.SetRoundNumber(roundNumber);
					currentRound.SetDrawingPlayer(players[0].GetName());
					currentRound.SetCurrentWord(*words.begin());
					words.erase(words.begin());
					currentRound.SetWords(words);
					storage.Update(currentRound);
					return crow::response{ 200 };
				}
				else if (index < players.size() - 1)
				{
					currentRound.SetDrawingPlayer(players[index + 1].GetName());
					currentRound.SetCurrentWord(*words.begin());
					words.erase(words.begin());
					currentRound.SetWords(words);
					storage.Update(currentRound);
					return crow::response{ 200 };
				}
			}
		}

		return crow::response{ 409 };
			});

	// Checks if the game has ended in the database
	CROW_ROUTE(m_app, "/gameEnded")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];

		if (storage.GetGame(roomID).GetGameStatusAsInt() == 3)
			return crow::response{ 200 };

		return crow::response{ 409 };
			});

	CROW_ROUTE(m_app, "/startTurn/SendDrawing")
		.methods("POST"_method)
		([this](const crow::request& req) {
		auto jsonData = crow::json::load(req.body);
		if (!jsonData)
			return crow::response(400);

		m_gameHandlers.SetDrawing(jsonData["DrawingData"].s());
		return crow::response(200, "OK");
			});

	CROW_ROUTE(m_app, "/startTurn/Return_DrawingData")
		.methods("POST"_method)
		([this]() {
		auto drawingData = m_gameHandlers.GetDrawing();
		return crow::json::wvalue{ {"DrawingData", drawingData} };
			});



	m_app.bindaddr("127.0.0.1").port(18080).multithreaded().run();
}
