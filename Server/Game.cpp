module game;

using namespace skribbl;

Game::Game(const int id, const Player& player, const std::string& gameCode, const size_t maxPlayers, const size_t currentPlayers)
	: m_id{ id },
	m_gameCode{ gameCode },
	m_maxPlayers{ maxPlayers },
	m_round{ gameCode, {} , maxPlayers }
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
	if (m_players.size() == 1) {
		return false;
	}

	return m_round.GetGuessedPlayerNames().size() == m_players.size() - 1;
}

const bool Game::IsLastRound() const noexcept
{
	return m_round.GetRoundNumber() == GetMaxNumberOfRoundsAllowed() 
		&& m_players[m_players.size() - 1].GetName() == m_round.GetDrawingPlayer();
}

const bool Game::ShouldEndGame() const noexcept
{
	return GetAllPlayersGuessedWord() && IsLastRound();
}

const size_t Game::GetCurrentPlayerCount() const noexcept
{
	return m_players.size();
}

const size_t Game::GetNumberOfMaxPlayers() const noexcept
{
	return m_maxPlayers;
}

const std::string Game::SerializePlayers() const noexcept
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

const std::string Game::SerializePlayerNames() const noexcept
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

const std::string Game::SerializeGameChat() const noexcept
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

size_t Game::CalculateAverageGuessTime(int currentDrawingPlayerIndex) const noexcept
{
	size_t averageTime{ 0 };
	size_t timesSize{ m_round.GetTimes().size() };

	for (size_t index = 0; index < timesSize; index++) {
		if (index != currentDrawingPlayerIndex) {
			averageTime += (static_cast<size_t>(60) - m_round.GetTimes().at(index));
		}
	}

	if (m_players.size() != 1) {
		averageTime /= m_players.size() - 1;
	}

	return averageTime;
}

int Game::CalculateDrawerPoints(size_t averageTime, const std::string& currentDrawingPlayer) noexcept
{
	int points{ -50 };

	if (averageTime == ((m_players.size() - 1) * 60)) {
		std::ranges::for_each(m_players, [&](Player& player) {
			if (player.GetName() != currentDrawingPlayer)
				player.SetPoints(player.GetPoints() + points);
			});
		points = -100;
	} else {
		points = static_cast<int>(((60 - averageTime) * 100) / 60);
	}

	return points;
}

void Game::UpdateDrawerScore(const std::string& drawerName, int points) noexcept
{
	auto drawerIt = std::find_if(
		m_players.begin(),
		m_players.end(),
		[&](const Player& player) {
			return player.GetName() == drawerName;
		});

	if (drawerIt != m_players.end()) {
		drawerIt->AddPoints(points);
	}
}

bool Game::ShouldEndGame(const std::string& currentDrawingPlayer) const noexcept
{
	return m_round.GetRoundNumber() == GetMaxNumberOfRoundsAllowed()
		&& m_players[m_players.size() - 1].GetName() == currentDrawingPlayer;
}

bool Game::NextRound() noexcept
{
	const std::string currentDrawingPlayer{ m_round.GetDrawingPlayer() };
	const int currentDrawingPlayerIndex{ GetPlayerIndex(currentDrawingPlayer) };

	size_t averageTime{ CalculateAverageGuessTime(currentDrawingPlayerIndex) };
	int points{ CalculateDrawerPoints(averageTime, currentDrawingPlayer) };
	UpdateDrawerScore(currentDrawingPlayer, points);

	if (ShouldEndGame(currentDrawingPlayer)) {
		m_gameStatus = GameStatus::FINISHED;
		m_round.SetImageData("");
		m_round.ClearAllPlayersGuessed();
		return true;
	}

	// Calculate next player index
	const size_t nextPlayerIndex = (static_cast<size_t>(currentDrawingPlayerIndex) + 1) % m_players.size();
	const std::string nextPlayer = m_players.at(nextPlayerIndex).GetName();
	
	// Only increment round number when we cycle back to the first player
	const uint8_t nextRoundNumber = (nextPlayerIndex == 0) 
		? m_round.GetRoundNumber() + 1 
		: m_round.GetRoundNumber();

	if (!m_round.StartRound(nextPlayer, nextRoundNumber)) {
		return false;
	}

	return true;
}

bool Game::IsCorrectGuess(const std::string& message) const noexcept
{
	std::string messageLower{ message };
	std::string currentWord{ m_round.GetCurrentWord() };
	std::transform(messageLower.begin(), messageLower.end(), messageLower.begin(), ::tolower);
	std::transform(currentWord.begin(), currentWord.end(), currentWord.begin(), ::tolower);

	return messageLower == currentWord;
}

void Game::ProcessCorrectGuess(const std::string& username, int timeLeft, int score) noexcept
{
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
	}
}

bool Game::AddChatLineMessage(const std::string& message, const std::string& username, bool& guessed) noexcept
{
	guessed = false;

	if (IsCorrectGuess(message)) {
		const int timeLeft = m_round.GetTimeLeft();
		int score{ m_round.CalculatePoints(timeLeft) };

		ProcessCorrectGuess(username, timeLeft, score);
		guessed = true;

		m_chatLineMessages.emplace_back(username + " guessed the word!");
	} else {
		m_chatLineMessages.emplace_back(username + ": " + message);
	}

	return true;
}

void Game::AddPlayer(const Player& player) noexcept
{
	m_players.push_back(player);
	m_currentPlayers++;
}

void Game::RemovePlayer(const Player& player) noexcept
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

void Game::SetId(int id) noexcept
{
	m_id = id;
}

void Game::SetPlayers(const std::vector<Player>& players) noexcept
{
	m_players = players;
}

void Game::DeserializePlayers(const std::string& serializedPlayers) noexcept
{
	m_players.clear();
	std::istringstream ss(serializedPlayers);
	std::string serializedPlayer;
	while (std::getline(ss, serializedPlayer, ';')) {
		m_players.emplace_back(Player{}.Deserialize(serializedPlayer));
	}
}

void Game::DeserializeGameChat(const std::string& serializedChatLines) noexcept
{
	m_chatLineMessages.clear();
	std::istringstream ss(serializedChatLines);
	std::string chatLine;
	while (std::getline(ss, chatLine, ';')) {
		m_chatLineMessages.emplace_back(chatLine);
	}
}
