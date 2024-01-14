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
		const std::string& username = x["username"];

		if (username == "")
			return crow::response{ 404, "Username not found." };

		if (storage.CheckUsername(std::move(username)))
			return crow::response{ "true" };

		return crow::response{ 409, "Username does not exist!" };
			});

	// Adds a user to the database
	CROW_ROUTE(m_app, "/addUser")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		const std::string& username = x["username"];
		const std::string& password = x["password"];
		const std::string& email = x["email"];

		if (username == "" || password == "" || email == "")
			return crow::response{ 404, "Username, password or email not found." };

		if (storage.AddUser(std::move(username), std::move(password), std::move(email)))
			return crow::response{ "true" };

		return crow::response{ "false" };
			});

	// Checks if the username and password match in the database
	CROW_ROUTE(m_app, "/loginUser")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		const std::string& username = x["username"];
		const std::string& password = x["password"];

		std::string hashedPass = password;
		Hasher::HashPassword(hashedPass.c_str());
		std::string hashedPassStr{ hashedPass };

		if (username == "" || password == "")
			return crow::response{ 404, "Username or password not found." };

		if (!storage.CheckUsername(std::move(username)))
			return crow::response{ 409, "Username does not exist!" };

		if (!storage.CheckPassword(std::move(username), std::move(hashedPassStr)))
			return crow::response{ 409, "Wrong password!" };

		return crow::response{ 200 };

			});

	// Gets a random roomID from the database
	CROW_ROUTE(m_app, "/roomID")
		.methods("GET"_method, "POST"_method)([&]() {
		std::string roomID = storage.GetRandomID();

		while (storage.CheckRoomID(std::move(roomID)) && storage.GetGame(std::move(roomID)).GetGameStatusAsInt() != 3)
			roomID = storage.GetRandomID();

		return crow::response{ roomID };
			});

	// Creates a game in the database
	CROW_ROUTE(m_app, "/createRoom")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		const std::string& roomID = x["gameCode"];
		const std::string& username = x["username"];
		int maxPlayers = std::stoi(x["maxPlayers"]);
		int currentPlayers = std::stoi(x["currentPlayers"]);

		Player player = storage.GetPlayer(std::move(username));

		if (!storage.AddGame(std::move(player), std::move(roomID), maxPlayers))
			return crow::response{ 409, "Error creating the game." };
		return crow::response{ 200 };
			});

	// Checks if the roomID exists in the database
	CROW_ROUTE(m_app, "/checkRoomID")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		const std::string& roomID = x["roomID"];

		if (!storage.CheckRoomID(std::move(roomID)))
			return crow::response{ 409, "Game not found" };

		if (storage.GetGame(std::move(roomID)).GetGameStatusAsInt() != 1)
			return crow::response{ 409, "Game already started" };

		if (storage.GetGame(std::move(roomID)).GetCurrentPlayers() >= storage.GetGame(std::move(roomID)).GetMaxPlayers())
			return crow::response{ 409, "Game is full" };

		const std::string& currentPlayers = std::to_string(storage.GetGame(std::move(roomID)).GetCurrentPlayers());
		return crow::response{ 200 , currentPlayers };
			});

	// Adds a player to the game in the database
	CROW_ROUTE(m_app, "/joinRoom")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		const std::string& roomID = x["roomID"];
		const std::string& username = x["username"];

		Player player = storage.GetPlayer(std::move(username));
		int currentPlayers = std::stoi(x["currentPlayers"]);

		if (!storage.AddPlayerToGame(std::move(player), std::move(roomID), currentPlayers))
			return crow::response{ 409, "Error joining the game." };

		return crow::response{ 200 };
			});

	// Gets the players from the game in the database
	CROW_ROUTE(m_app, "/roomPlayers")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		const std::string& roomID = x["roomID"];

		return crow::response{ storage.GetGame(std::move(roomID)).SerializePlayers() };
			});

	// Gets the max number of players from the game in the database
	CROW_ROUTE(m_app, "/numberOfPlayers")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		const std::string& roomID = x["roomID"];

		return crow::response{ std::to_string(storage.GetGame(std::move(roomID)).GetMaxPlayers()) };
			});

	// Gets the current number of players from the game in the database
	CROW_ROUTE(m_app, "/currentNumberOfPlayers")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		const std::string& roomID = x["roomID"];

		return crow::response{ std::to_string(storage.GetGame(std::move(roomID)).GetCurrentPlayers()) };
			});

	// Removes a player from the game in the database
	CROW_ROUTE(m_app, "/leaveRoom")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		const std::string& roomID = x["roomID"];
		const std::string& username = x["username"];

		Player player = storage.GetPlayer(std::move(username));

		if (!storage.RemovePlayerFromGame(std::move(player), std::move(roomID)))
			return crow::response{ 409, "Error leaving the game." };
		return crow::response{ 200 };
			});

	// Checks if the game has started in the database
	CROW_ROUTE(m_app, "/gameStarted")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		const std::string& roomID = x["roomID"];

		if (storage.GetGame(std::move(roomID)).GetGameStatusAsInt() == 2)
			return crow::response{ 200 };

		return crow::response{ 409 };
			});

	// Starts the game based on the roomID in the database
	CROW_ROUTE(m_app, "/startGame")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		const std::string& roomID = x["roomID"];

		std::vector<Player> players = storage.GetGame(std::move(roomID)).GetPlayers();

		// Set the players score to 0 at the start of the game
		std::ranges::for_each(players, [&](Player& player) { storage.SetPlayerScore(player.GetName(), 0); });

		Game currentGame = storage.GetGame(std::move(roomID));
		Round currentRound = storage.GetRound(std::move(roomID));
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
		storage.Update(std::move(currentRound));

		// Set the game status to 2 (in progress)
		if (!storage.SetGameStatus(std::move(roomID), 2))
			return crow::response{ 409, "Error starting the game." };

		return crow::response{ 200 };
			});

	// Gets the player score from the database based on the username
	CROW_ROUTE(m_app, "/playerScore")
		.methods("GET"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		const std::string& username = x["username"];

		return crow::response{ std::to_string(storage.GetPlayerScore(std::move(username))) };
			});

	// Adds the text that the player wrote to the current chat in the database
	CROW_ROUTE(m_app, "/addChat")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		const std::string& roomID = x["roomID"];
		const std::string& username = x["username"];
		std::string text = x["text"];

		// if %20 is found in the text, replace it with a space
		if (text.find("%20") != std::string::npos)
			text.replace(text.find("%20"), 3, " ");

		std::string currentChat = storage.GetGame(std::move(roomID)).GetChat();
		std::string currentWord = storage.GetRound(std::move(roomID)).GetCurrentWord();
		std::transform(text.begin(), text.end(), text.begin(), ::tolower);
		std::transform(currentWord.begin(), currentWord.end(), currentWord.begin(), ::tolower);

		int timeLeft = storage.GetRound(std::move(roomID)).GetTimeLeft();
		int score{ 0 };

		if (timeLeft >= 30)
			score = 100;
		else if (timeLeft > 0 && timeLeft < 30)
			score = (((60 - timeLeft) * 100) / 60);

		Player player = storage.GetPlayer(std::move(username));
		if (text == currentWord)
		{
			// Add the timelft in an array in order to calculate the average time
			Round currentRound = storage.GetRound(roomID);
			int index = storage.GetGame(std::move(roomID)).GetPlayerIndex(std::move(username));

			if (index == -1)
				return crow::response{ 409, "Error adding the chat." };

			currentRound.UpdateTimes(index, timeLeft);
			storage.Update(std::move(currentRound));

			// Add the points to the player
			player.AddPoints(score);
			storage.Update(std::move(player));

			currentChat += std::move(username) + " guessed the word!\n";
			if (!storage.SetGameChat(std::move(roomID), std::move(currentChat)))
				return crow::response{ 409, "Error adding the chat." };
			return crow::response{ 200 , "TRUE" };
		}
		else
		{
			currentChat += std::move(username) + ": " + std::move(text) + "\n";

			if (!storage.SetGameChat(std::move(roomID), std::move(currentChat)))
				return crow::response{ 409, "Error adding the chat." };

			return crow::response{ 200 };
		}
			});

	// Gets the current chat from the database
	CROW_ROUTE(m_app, "/getChat")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		const std::string& roomID = x["roomID"];

		return crow::response{ storage.GetGame(std::move(roomID)).GetChat() };
			});

	// Gets the current drawing player name from the database
	CROW_ROUTE(m_app, "/drawingPlayer")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		const std::string& roomID = x["roomID"];

		return crow::response{ storage.GetRound(std::move(roomID)).GetDrawingPlayer() };
			});

	// Gets the current round number from the database
	CROW_ROUTE(m_app, "/roundNumber")
		.methods("GET"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		std::string roomID = x["roomID"];

		return crow::response{ std::to_string(storage.GetRound(std::move(roomID)).GetRoundNumber()) + "/" + std::to_string(storage.GetGame(std::move(roomID)).GetNoOfRounds()) };
			});

	// Gets the current word that needs to be drawn from the database
	CROW_ROUTE(m_app, "/currentWord")
		.methods("GET"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		const std::string& roomID = x["roomID"];

		return crow::response{ storage.GetRound(std::move(roomID)).GetCurrentWord() };
			});

	// Updates the time left from the owner of the game and gets the time left from the database
	CROW_ROUTE(m_app, "/timeLeft")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		const std::string& roomID = x["roomID"];

		if (req.method == crow::HTTPMethod::POST)
		{
			const std::string& timeLeft = x["timer"];
			Round currentRound = storage.GetRound(std::move(roomID));
			currentRound.SetTimeLeft(std::stoi(std::move(timeLeft)));
			storage.Update(std::move(currentRound));
			return crow::response{ 200 };
		}
		else if (req.method == crow::HTTPMethod::GET)
		{
			return crow::response{ std::to_string(storage.GetRound(std::move(roomID)).GetTimeLeft()) };
		}
		else
			return crow::response{ 404 };

			});

	// Sets the next round in the database
	CROW_ROUTE(m_app, "/nextRound")
		.methods("POST"_method)([&](const crow::request& req) {
		auto x = parseUrlArgs(req.body);
		const std::string& roomID = x["roomID"];

		Round currentRound = storage.GetRound(std::move(roomID));
		std::set<std::string> words = currentRound.GetWords();
		std::vector<Player> players = storage.GetGame(std::move(roomID)).GetPlayers();
		std::string currentDrawingPlayer = currentRound.GetDrawingPlayer();

		// Calculate the average time on the current round in order to calculate the points to the drawer
		int averageTime{ 0 };
		int points{ -50 };
		int currentDrawingPlayerIndex = storage.GetGame(std::move(roomID)).GetPlayerIndex(std::move(currentDrawingPlayer));

		for (uint8_t index = 0; index < currentRound.GetTimes().size(); index++)
			if (index != currentDrawingPlayerIndex)
				averageTime = averageTime + (60 - currentRound.GetTimes()[index]);

		if (players.size() != 1)
			averageTime /= players.size() - 1;

		std::vector<int> times = currentRound.GetTimes();

		if (averageTime == ((players.size() - 1) * 60))
		{
			std::ranges::for_each(players, [&](const Player& player) {
				if (player.GetName() != currentDrawingPlayer)
					storage.SetPlayerScore(std::move(player.GetName()), storage.GetPlayerScore(std::move(player.GetName())) + points);
				});
			points = -100;
		}
		else
			points = ((60 - averageTime) * 100) / 60;

		Player drawer = storage.GetPlayer(std::move(currentDrawingPlayer));

		drawer.AddPoints(points); // Add the points to the drawer
		storage.Update(std::move(drawer));

		// If it is the last round and the last player is the drawer, the timer ended so the game ends
		if (currentRound.GetRoundNumber() == storage.GetGame(std::move(roomID)).GetNoOfRounds() && players[players.size() - 1].GetName() == currentDrawingPlayer)
		{
			storage.SetGameStatus(std::move(roomID), 3);
			currentRound.SetImageData("");
			return crow::response{ 200 };
		}

		// Set the average times to 0 for the next round
		std::ranges::for_each(times, [](int& time) { time = 0; });

		currentRound.SetTimes(std::move(times));
		currentRound.SetImageData("");
		// Update the round
		for (size_t index = 0; index < players.size(); index++)
		{
			if (players[index].GetName() == currentDrawingPlayer)
			{
				if (index == players.size() - 1)
				{
					currentRound.SetRoundNumber(currentRound.GetRoundNumber() + 1);
					currentRound.SetDrawingPlayer(std::move(players[0].GetName()));
					currentRound.SetCurrentWord(std::move(*words.begin()));
					words.erase(words.begin());
					currentRound.SetWords(std::move(words));
					storage.Update(std::move(currentRound));
					return crow::response{ 200 };
				}
				else if (index < players.size() - 1)
				{
					currentRound.SetDrawingPlayer(std::move(players[index + 1].GetName()));
					currentRound.SetCurrentWord(std::move(*words.begin()));
					words.erase(words.begin());
					currentRound.SetWords(std::move(words));
					storage.Update(std::move(currentRound));
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
		const std::string& roomID = x["roomID"];

		if (storage.GetGame(std::move(roomID)).GetGameStatusAsInt() == 3)
			return crow::response{ 200 };

		return crow::response{ 409 };
			});

	CROW_ROUTE(m_app, "/drawingImage")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {

		if (req.method == crow::HTTPMethod::POST)
		{
			const std::string& roomID = req.url_params.get("roomID");
			const std::string& imageData = req.body;
			Round currentRound = storage.GetRound(std::move(roomID));
			currentRound.SetImageData(std::move(imageData));
			storage.Update(std::move(currentRound));
			return crow::response{ 200 };
		}
		else if (req.method == crow::HTTPMethod::GET)
		{
			auto x = parseUrlArgs(req.body);
			const std::string& roomID = x["roomID"];
			return crow::response{ storage.GetRound(std::move(roomID)).GetImageData() };
		}
		else
			return crow::response{ 404 };

			});

	CROW_ROUTE(m_app, "/clearImage")
		.methods("GET"_method, "POST"_method)([&](const crow::request& req) {
			auto x = parseUrlArgs(req.body);
			const std::string& roomID = x["roomID"];
			Round currentRound = storage.GetRound(std::move(roomID));
			currentRound.SetImageData("");
			storage.Update(std::move(currentRound));
			return crow::response{ 200 };
			});

	m_app.bindaddr("127.0.0.1").port(18080).multithreaded().run();
}
