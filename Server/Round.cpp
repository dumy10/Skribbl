module round;

using namespace skribbl;


Round::Round(int id, const std::string& gameId)
	:
	m_id{ id },
	m_gameId{ gameId },
	m_drawingPlayerName{ "" },
	m_currentWord{ "" },
	m_roundNumber{ 1 },
	m_timeLeft{ 60 }
{
	// Empty
}

Round::Round(int id, const std::string& gameId, const std::string& drawingPlayerName, const std::string& currentWord, const std::set<std::string>& words, uint8_t roundNumber)
	:
	m_id{ id },
	m_gameId{ gameId },
	m_drawingPlayerName{ drawingPlayerName },
	m_currentWord{ currentWord },
	m_words{ words },
	m_roundNumber{ roundNumber },
	m_timeLeft{ 60 }
{
	// Empty
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

void Round::SetTimeLeft(int timeLeft)
{
	m_timeLeft = timeLeft;
}

int Round::GetTimeLeft() const noexcept
{
	return m_timeLeft;
}

void Round::StartTimer()
{
	while (m_timeLeft > 0)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		m_timeLeft--;
	}
}
