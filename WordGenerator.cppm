export module wordgenerator;

import <string>;
import <vector>;
namespace modern {
	export class WordGenerator {
	public:
		std::string generateWords()
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