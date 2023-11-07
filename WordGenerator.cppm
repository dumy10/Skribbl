export module wordgenerator;

import <string>;
import <vector>;
import <fstream>;


namespace skribbl {
	export class WordGenerator {
	public:
		static std::string generateWord();
		const std::vector<std::string>& getUsedWords() const noexcept;

	private:
		std::vector<std::string> m_words;
		std::vector<std::string> m_usedWords;
	};
}