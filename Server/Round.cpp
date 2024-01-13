module round;

using namespace skribbl;


Round::Round(int id, const std::string& gameId, size_t maxPlayers)
	:
	m_id{ id },
	m_gameId{ gameId },
	m_drawingPlayerName{ "" },
	m_currentWord{ "" },
	m_roundNumber{ 1 },
	m_timeLeft{ 60 }
{
	m_imageData = std::string();
	m_times.resize(maxPlayers);
	std::ranges::for_each(m_times, [](auto& time) { time = 0; });
}

void Round::SetId(int id)
{
	m_id = id;
}

void Round::SetGameId(std::string gameId)
{
	m_gameId = gameId;
}

void Round::SetDrawingPlayer(const std::string& drawingPlayerName)
{
	m_drawingPlayerName = drawingPlayerName;
}

void Round::SetCurrentWord(const std::string& currentWord)
{
	m_currentWord = currentWord;
}

void Round::SetWords(const std::set<std::string>& words)
{
	m_words = words;
}

void Round::SetRoundNumber(uint8_t roundNumber)
{
	m_roundNumber = roundNumber;
}

void Round::DeserializeTimes(const std::string& serializedTimes)
{
	std::stringstream ss{ serializedTimes };
	std::string point;
	while (std::getline(ss, point, ','))
		m_times.push_back(std::stoi(point));
}

void Round::SetTimes(const std::vector<int>& times)
{
	m_times = times;
}

void Round::SetImageData(const std::string& imageData)
{
	m_imageData = imageData;
}

int Round::GetId() const noexcept
{
	return m_id;
}

std::string Round::GetGameId() const noexcept
{
	return m_gameId;
}

std::string Round::GetDrawingPlayer() const noexcept
{
	return m_drawingPlayerName;
}

std::string Round::GetCurrentWord() const noexcept
{
	return m_currentWord;
}

uint8_t Round::GetRoundNumber() const noexcept
{
	return m_roundNumber;
}

std::string Round::SerializeWords() const noexcept
{
	std::string serializedWords;
	for (const auto& word : m_words)
		serializedWords += word + ",";

	if (!serializedWords.empty())
		serializedWords.pop_back();

	return serializedWords;
}

void Round::DeserializeWords(const std::string& serializedWords)
{
	std::stringstream ss{ serializedWords };
	std::string word;
	while (std::getline(ss, word, ','))
		m_words.insert(word);
}

std::set<std::string> skribbl::Round::GetWords() const noexcept
{
	return m_words;
}

std::string Round::SerializeTimes() const noexcept
{
	std::string serializedTimes;
	for (const auto& time : m_times)
		serializedTimes += std::to_string(time) + ",";
	if (!serializedTimes.empty())
		serializedTimes.pop_back();
	return serializedTimes;
}

std::vector<int> Round::GetTimes() const noexcept
{
	return m_times;
}

void Round::UpdateTimes(int index, int value) noexcept
{
	m_times[index] = value;
}

std::string Round::GetImageData() const noexcept
{
	return m_imageData;
}

void Round::SetTimeLeft(int timeLeft)
{
	m_timeLeft = timeLeft;
}

int Round::GetTimeLeft() const noexcept
{
	return m_timeLeft;
}

