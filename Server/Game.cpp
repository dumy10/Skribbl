module game;

using namespace skribbl;

Game::Game(int id, const Player& player, const std::string& gameCode, size_t maxPlayers, size_t currentPlayers)
	: m_id{ id },
	m_gameCode{ gameCode },
	m_maxPlayers{ maxPlayers }
{
	m_players.reserve(maxPlayers);
	m_players.emplace_back(player);
	m_gameStatus = GameStatus::WAITING;
	m_chatLineMessages.reserve(100); // Reserve space for 100 chat messages to avoid multiple allocations
	m_chatLineMessages.emplace_back("Welcome to skribbl!\n");
}

Game::Game(Game&& other) 
	: m_id{ other.m_id },
	m_gameCode{ std::move(other.m_gameCode) },
	m_maxPlayers{ other.m_maxPlayers },
	m_currentPlayers{ other.m_currentPlayers },
	m_players{ std::move(other.m_players) },
	m_chatLineMessages{ std::move(other.m_chatLineMessages) },
	m_gameStatus{ other.m_gameStatus },
	m_round{ std::move(other.m_round) }
{
}

Game& skribbl::Game::operator=(Game&& other)
{
	if (this != &other) {
		m_id = other.m_id;
		m_gameCode = std::move(other.m_gameCode);
		m_maxPlayers = other.m_maxPlayers;
		m_currentPlayers = other.m_currentPlayers;
		m_players = std::move(other.m_players);
		m_chatLineMessages = std::move(other.m_chatLineMessages);
		m_gameStatus = other.m_gameStatus;
		m_round = std::move(other.m_round);
	}

	return *this;
}

const uint8_t Game::GetRoundNumber() const noexcept
{
	return m_round.GetRoundNumber();
}

const int Game::GetId() const noexcept
{
	return m_id;
}

const int Game::GetPlayerIndex(const std::string& username) const noexcept
{
	for (uint8_t i = 0; i < m_players.size(); i++) {
		if (m_players[i].GetName() == username) {
			return i;
		}
	}

	return -1;
}

const int Game::GetPlayerScore(const std::string& username) const noexcept
{
	if (auto it = std::find_if(
		m_players.begin(),
		m_players.end(),
		[&](const Player& player) {
			return player.GetName() == username;
		});
		it != m_players.end()) {
		return it->GetPoints();
	}

	return 0;
}

const int Game::GetMaxNumberOfRoundsAllowed() const noexcept
{
	return kMaxNumberOfRounds;
}

const int Game::GetGameStatusAsInt() const noexcept
{
	return static_cast<int>(m_gameStatus);
}

const bool Game::GetAllPlayersGuessedWord() const noexcept
{
	return m_round.GetGuessedPlayerNames().size() == m_players.size() - 1;
}

const size_t Game::GetCurrentPlayerCount() const noexcept
{
	return m_players.size();
}

const size_t Game::GetNumberOfMaxPlayers() const noexcept
{
	return m_maxPlayers;
}

const std::string Game::SerializePlayers() const
{
	std::string serializedPlayers;
	for (const auto& player : m_players) {
		serializedPlayers += player.Serialize() + ";";
	}

	if (!serializedPlayers.empty()) {
		serializedPlayers.pop_back(); // Remove the trailing semicolon
	}

	return serializedPlayers;
}

const std::string skribbl::Game::SerializePlayerNames() const
{
	std::string serializedPlayerNames;
	for (const auto& player : m_players) {
		serializedPlayerNames += player.GetName() + ",";
	}

	if (!serializedPlayerNames.empty()) {
		serializedPlayerNames.pop_back(); // Remove the trailing comma
	}

	return serializedPlayerNames;
}

const std::string Game::SerializeGameChat() const
{
	std::string serializedChat;
	for (const auto& chatLine : m_chatLineMessages) {
		serializedChat += chatLine + ";";
	}

	if (!serializedChat.empty()) {
		serializedChat.pop_back(); // Remove the trailing semicolon
	}

	return serializedChat;
}

const std::string Game::GetGameCode() const noexcept
{
	return m_gameCode;
}

const std::string Game::GetDrawingPlayer() const noexcept
{
	return m_round.GetDrawingPlayer();
}

const GameStatus Game::GetGameStatus() const noexcept
{
	return m_gameStatus;
}

const std::vector<Player> Game::GetPlayers() const noexcept
{
	return m_players;
}

const std::vector<std::string> Game::GetChatLines() const noexcept
{
	return m_chatLineMessages;
}

bool Game::StartGame(const std::set<std::string>& words) noexcept
{
	std::ranges::for_each(m_players, [&](Player& player) { player.SetPoints(0); });

	m_round.SetWords(words);

	if (!m_round.StartRound(m_players.at(0).GetName(), 1)) {
		return false;
	}

	m_gameStatus = GameStatus::INPROGRESS;

	return true;
}

bool Game::EndGame() noexcept
{
	m_gameStatus = GameStatus::FINISHED;
	return m_round.StopRound();
}

bool Game::NextRound() noexcept
{
	const std::string currentDrawingPlayer{ m_round.GetDrawingPlayer() };

	size_t averageTime{ 0 };
	size_t timesSize{ m_round.GetTimes().size() };

	const int currentDrawingPlayerIndex{ GetPlayerIndex(currentDrawingPlayer) };
	int points{ -50 };

	for (size_t index = 0; index < timesSize; index++) {
		if (index != currentDrawingPlayerIndex) {
			averageTime = averageTime + (60 - m_round.GetTimes().at(index));
		}
	}

	if (m_players.size() != 1) {
		averageTime /= m_players.size() - 1;
	}

	if (averageTime == ((m_players.size() - 1) * 60)) {
		std::ranges::for_each(m_players, [&](Player& player) {
			if (player.GetName() != currentDrawingPlayer)
				player.SetPoints(player.GetPoints() + points);
			});
		points = -100;
	}
	else {
		points = static_cast<int>(((60 - averageTime) * 100) / 60);
	}

	auto drawerIt = std::find_if(
		m_players.begin(),
		m_players.end(),
		[&](const Player& player) {
			return player.GetName() == currentDrawingPlayer;
		});

	if (drawerIt != m_players.end()) {
		drawerIt->AddPoints(points);
	}

	if (m_round.GetRoundNumber() == GetMaxNumberOfRoundsAllowed()
		&& m_players[m_players.size() - 1].GetName() == currentDrawingPlayer) {
		m_gameStatus = GameStatus::FINISHED;
		m_round.SetImageData("");

		return true;
	}

	if (!m_round.StartRound(m_players.at((currentDrawingPlayerIndex + 1) % m_players.size()).GetName(), m_round.GetRoundNumber() + 1)) {
		return false;
	}

	return true;
}

bool Game::AddChatLineMessage(const std::string& message, const std::string& username, bool& guessed)
{
	guessed = false;
	std::string chatLine{ username };
	std::string currentWord{ m_round.GetCurrentWord() };
	std::transform(chatLine.begin(), chatLine.end(), chatLine.begin(), ::tolower);
	std::transform(currentWord.begin(), currentWord.end(), currentWord.begin(), ::tolower);

	const int timeLeft = m_round.GetTimeLeft();
	int score{ m_round.CalculatePoints(timeLeft) };

	if (message == currentWord) {
		auto playerIt = std::find_if(
			m_players.begin(),
			m_players.end(),
			[&](const Player& player) {
				return player.GetName() == username;
			});

		if (playerIt != m_players.end()) {
			int index{ GetPlayerIndex(username) };
			m_round.PlayerGuessedWord(username, index, timeLeft);
			playerIt->AddPoints(score);
			guessed = true;
		}

		m_chatLineMessages.emplace_back(std::move(username) + " guessed the word!");
	}
	else {
		m_chatLineMessages.emplace_back(std::move(username) + ": " + std::move(message));
	}

	return true;
}

void Game::AddPlayer(const Player& player)
{
	m_players.push_back(player);
}

void Game::RemovePlayer(const Player& player)
{
	auto playerIt = std::find(m_players.begin(), m_players.end(), player);
	if (playerIt != m_players.end()) {
		m_players.erase(playerIt);
		m_currentPlayers--;
	}
}

void Game::SetGameCode(const std::string& gameCode) noexcept
{
	m_gameCode = gameCode;
}

void Game::SetGameStatus(GameStatus status) noexcept
{
	m_gameStatus = status;
}

void Game::SetGameStatusFromInt(int status) noexcept
{
	m_gameStatus = static_cast<GameStatus>(status);
}

void Game::SetMaxPlayers(size_t maxPlayers) noexcept
{
	m_maxPlayers = maxPlayers;
}

void Game::SetCurrentPlayers(size_t currentPlayers) noexcept
{
	m_currentPlayers = currentPlayers;
}

void Game::SetId(int id)
{
	m_id = id;
}

void Game::SetPlayers(const std::vector<Player>& players)
{
	m_players = players;
}

void Game::DeserializePlayers(const std::string& serializedPlayers)
{
	m_players.clear();
	std::istringstream ss(serializedPlayers);
	std::string serializedPlayer;
	while (std::getline(ss, serializedPlayer, ';')) {
		m_players.emplace_back(Player{}.Deserialize(serializedPlayer));
	}
}

void Game::DeserializeGameChat(const std::string& serializedChatLines)
{
	m_chatLineMessages.clear();
	std::istringstream ss(serializedChatLines);
	std::string chatLine;
	while (std::getline(ss, chatLine, ';')) {
		m_chatLineMessages.emplace_back(chatLine);
	}
}

bool skribbl::Game::operator==(const Game& other) const noexcept
{
	if (this == &other) {
		return true;
	}

	if (m_id == other.m_id) {
		return true;
	}

	if (m_gameCode == other.m_gameCode) {
		return true;
	}

	return false;
}
