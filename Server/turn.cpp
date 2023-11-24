module turn;

using namespace skribbl;

Turn::Turn(const Player& player, const std::string& word):
	m_player{ player },
	m_word {word},
	m_drawTime(std::chrono::seconds(60))
{
	// Empty
}

void Turn::setPlayerTurn(const Player& player, const std::string& word)
{
	this->m_player = player;
	this->m_word = word;
}

void Turn::startTimer() noexcept
{
	m_startTime = std::chrono::steady_clock::now();
}

const Player& Turn::getCurrentPlayer() const noexcept
{
	return this->m_player;
}

const std::string_view Turn::getCurrentWord() const noexcept
{
	return this->m_word;
}

const bool Turn::guessWord(const std::string& word) const
{
	return this->m_word == word;
}