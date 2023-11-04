export module wordgenerator;

import <string>;
import <vector>;
namespace skribbl {
	export class WordGenerator {
	public:
		static std::string generateWords()
		{
			return "";
		}
		std::vector<std::string> lexicalFamily(const std::string& word)
		{
			return{};
		}
	private:
		std::vector<std::string> m_usedWords;
	};
}