export module word;

import <string>;

namespace skribbl
{
	export class Word
	{
	public:
		Word() = default;
		Word(int id, const std::string& word);

		int GetId() const noexcept;
		std::string GetWord() const noexcept;

		void SetId(int id);
		void SetWord(const std::string& word);

	private:
		int m_id;
		std::string m_word;
	};
}