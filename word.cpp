module word;

using namespace skribbl;

Word::Word(int id, const std::string& word) :
	m_id{ id },
	m_word{ word }
{
	// Empty
}

int Word::getId() const noexcept
{
	return m_id;

}
std::string Word::getWord() const noexcept
{
	return m_word;
}

void Word::setId(int id)
{
	m_id = id;
}

void Word::setWord(const std::string& word)
{
	m_word = word;
}