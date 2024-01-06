module round;

using namespace skribbl;


Round::Round(uint8_t roundNumber, const std::vector<Player>& players,int playerDrawing, std::set<std::string> words) :
	m_players{ players },
	m_playerDrawing{ playerDrawing },
	m_words{ words },
	m_roundNumber{ roundNumber }
{
	// Empty 
}

void Round::StartRound()
{
	
	auto word = m_words.begin();
	std::string censoredWord;
	for (int i = 0; i < word->size(); i++)
		censoredWord += "_";

	for (auto& player : m_players)
	{
		if (player.GetId() == m_playerDrawing)
			player.SetDrawing(true);
		else
			player.SetDrawing(false);
	}
	
	// timer + guess function + others



	EndRound();
}

void Round::EndRound()
{
	/*
	End round:
		- Display word
	*/

}

void Round::SetWords(const std::set<std::string>& words)
{
	this->m_words = words;
}

const bool Round::GuessWord(const std::string& word) const
{
	return m_words.find(word) != m_words.end();
}

const int Round::GetRound() const noexcept
{
	return m_roundNumber;
}

const std::set<std::string> Round::GetWords() const noexcept
{
	return m_words;
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


