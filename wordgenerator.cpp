module wordgenerator;

using namespace skribbl;


std::string WordGenerator::generateWord()
{
	/*
	Needs to take a random word from the words.txt file and return that word
	use random_shuffle
	*/

	return std::string();
}

const std::vector<std::string>& WordGenerator::getUsedWords() const noexcept
{
	return this->m_usedWords;
}