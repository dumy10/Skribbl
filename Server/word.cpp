module word;

using namespace skribbl;

Word::Word(int id, const std::string& word) :
	m_id{ id },
	m_word{ word }
{
	// Empty
}

const int Word::GetId() const noexcept
{
	return m_id;
}

const std::string Word::GetWord() const noexcept
{
	return m_word;
}

const std::string Word::Serialize() const noexcept
{
	return std::to_string(this->m_id) + "," + this->m_word;
}

void Word::SetId(int id)
{
	m_id = id;
}

void Word::SetWord(const std::string& word)
{
	m_word = word;
}