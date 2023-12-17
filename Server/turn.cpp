module turn;

using namespace skribbl;

Turn::Turn(const Player& player, const std::string& word) :
	m_player{ player },
	m_word{ word },
	m_drawTime(std::chrono::seconds(60))
{
	// Empty
}

void Turn::SetPlayerTurn(const Player& player, const std::string& word)
{
	this->m_player = player;
	this->m_word = word;
}

void Turn::StartTimer() noexcept
{
	m_startTime = std::chrono::steady_clock::now();
}

const Player& Turn::GetCurrentPlayer() const noexcept
{
	return this->m_player;
}

const std::string Turn::GetCurrentWord() const noexcept
{
	return this->m_word;
}

const bool Turn::GuessWord(const std::string& word) const
{
	return this->m_word == word;
}