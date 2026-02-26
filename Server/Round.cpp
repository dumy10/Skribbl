module round;

using namespace skribbl;

Round::Round(const std::string& gameId, const std::set<std::string>& words, const size_t maxPlayers)
	:
	m_gameId{ gameId },
	m_drawingPlayerName{ "" },
	m_currentWord{ "" },
	m_imageData{ "" },
	m_roundNumber{ 1 },
	m_words{ words }
{
	m_times.resize(maxPlayers);
	std::ranges::for_each(m_times, [](auto& time) { time = 0; });
}

Round::Round(Round&& other) noexcept
	:
	m_roundNumber{ other.m_roundNumber },
	m_gameId{ std::move(other.m_gameId) },
	m_drawingPlayerName{ std::move(other.m_drawingPlayerName) },
	m_currentWord{ std::move(other.m_currentWord) },
	m_imageData{ std::move(other.m_imageData) },
	m_times{ std::move(other.m_times) },
	m_guessedPlayerNames{ std::move(other.m_guessedPlayerNames) },
	m_words{ std::move(other.m_words) },
	m_timer{ std::move(other.m_timer) }
{
}

Round& Round::operator=(Round&& other) noexcept
{
	if (this != &other) {
		m_roundNumber = other.m_roundNumber;
		m_gameId = std::move(other.m_gameId);
		m_drawingPlayerName = std::move(other.m_drawingPlayerName);
		m_currentWord = std::move(other.m_currentWord);
		m_imageData = std::move(other.m_imageData);
		m_times = std::move(other.m_times);
		m_guessedPlayerNames = std::move(other.m_guessedPlayerNames);
		m_words = std::move(other.m_words);
		m_timer = std::move(other.m_timer);
	}

	return *this;
}

bool Round::StartRound(const std::string& drawingPlayerName, const uint8_t roundNumber) noexcept
{
	SetCurrentWord(*m_words.begin());
	SetRoundNumber(roundNumber);
	SetDrawingPlayer(drawingPlayerName);
	m_words.erase(m_words.begin());

	std::ranges::for_each(m_times, [](int& time) { time = 0; });
	SetImageData("");
	m_guessedPlayerNames.clear();

	m_timer.StopTicking();
	m_timer.ResetTimer();
	m_timer.StartTicking();

	return true;
}

bool Round::StopRound() noexcept
{
	SetImageData("");
	m_timer.StopTicking();

	return true;
}

void Round::SetRoundNumber(uint8_t roundNumber) noexcept
{
	m_roundNumber = roundNumber;
}

void Round::SetGameId(const std::string& gameId) noexcept
{
	m_gameId = gameId;
}

void Round::SetDrawingPlayer(const std::string& drawingPlayerName) noexcept
{
	m_drawingPlayerName = drawingPlayerName;
}

void Round::SetCurrentWord(const std::string& currentWord) noexcept
{
	m_currentWord = currentWord;
}

void Round::SetImageData(const std::string& imageData) noexcept
{
	m_imageData = imageData;
}

void Round::SetWords(const std::set<std::string>& words) noexcept
{
	m_words = words;
}

void Round::PlayerGuessedWord(const std::string& playerName, const int index, const int timeLeft) noexcept
{
	m_guessedPlayerNames.emplace_back(std::move(playerName));
	UpdateTimes(index, timeLeft);
}

void Round::ClearAllPlayersGuessed() noexcept
{
	m_guessedPlayerNames.clear();
}

void Round::SetTimes(const std::vector<int>& times) noexcept
{
	m_times = times;
}

void Round::UpdateTimes(const int index, const int value) noexcept
{
	if (index < 0 || index >= m_times.size()) {
		return;
	}

	m_times[index] = value;
}

const int Round::GetTimeLeft() const noexcept
{
	return m_timer.GetTimeLeft();
}

const int Round::CalculatePoints(const int time) const noexcept
{
	if (time >= 30) {
		return 100;
	}

	return ((60 - time) * 100) / 60;
}

const uint8_t Round::GetRoundNumber() const noexcept
{
	return m_roundNumber;
}

const std::string Round::GetGameId() const noexcept
{
	return m_gameId;
}

const std::string Round::GetDrawingPlayer() const noexcept
{
	return m_drawingPlayerName;
}

const std::string Round::GetCurrentWord() const noexcept
{
	return m_currentWord;
}

const std::string Round::GetImageData() const noexcept
{
	return m_imageData;
}

const std::vector<int> Round::GetTimes() const noexcept
{
	return m_times;
}

const std::vector<std::string> Round::GetGuessedPlayerNames() const noexcept
{
	return m_guessedPlayerNames;
}

const std::set<std::string> skribbl::Round::GetWords() const noexcept
{
	return m_words;
}
