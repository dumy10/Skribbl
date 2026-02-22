export module word;

import <string>;

namespace skribbl
{
	export class Word
	{
	public:
		Word() = default;
		Word(int id, const std::string& word);

		const int GetId() const noexcept;

		const std::string GetWord() const noexcept;
		const std::string Serialize() const noexcept;

		void SetId(int id);
		void SetWord(const std::string& word);

	private:
		int m_id;
		std::string m_word;
	};
}
