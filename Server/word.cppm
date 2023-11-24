export module word;

import <string>;

namespace skribbl
{
	export class Word
	{
	public:
		Word() = default;
		Word(int id, const std::string& word);

		int getId() const noexcept;
		std::string getWord() const noexcept;

		void setId(int id);
		void setWord(const std::string& word);

	private:
		int m_id;
		std::string m_word;
	};
}