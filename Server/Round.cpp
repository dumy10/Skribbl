module round;

using namespace skribbl;


Round::Round(const Turn& turn, const std::string& currentWord, uint8_t roundNumber, const std::vector<Player>& players) :
	m_turn{ turn },
	m_word{ currentWord },
	m_roundNumber{ roundNumber },
	m_players{ players }
{
	// Empty
}

void Round::StartRound()
{
	/*
	Start round:
		- Display welcome message
		- Display word to guess censored
		- Display leaderboard
		- Display who's turn is
		- Display timer
	*/
	for (const auto& player : m_players)
	{
		//turn.SetPlayerTurn(player, m_word);
		//turn.StartTimer();
		//wait for the turn to end
	}
	EndRound();
}

void Round::EndRound()
{
	/*
	End round:
		- Display word
	*/

}

void Round::SetWord(const std::string& word)
{
	this->m_word = word;
}

const bool Round::GuessWord(const std::string& word) const
{
	return word == m_word;
}

const int Round::GetRound() const noexcept
{
	return m_roundNumber;
}

const std::string skribbl::Round::GetWord() const noexcept
{
	//return turn.GetCurrentWord();
	return "";
}

void Round::ModifyRound()
{
	/*
	Next round:
		- Clear board
		- Set new word
		- Set new turn
		- Set new leaderboard
	*/

	m_roundNumber++;
}

void Round::SetTurn(const Player& player, const std::string& word)
{
	this->m_turn.SetPlayerTurn(player, word);
}

std::string skribbl::Round::GetDrawer()
{
	return m_turn.GetCurrentPlayer().GetName();
}

