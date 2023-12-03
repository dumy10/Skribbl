export module hashing;
import <string>;
import <iostream>;

export class __declspec(dllexport) Hashing
{
private:
	std::string m_hashedString;
public:
	Hashing();
	Hashing(const Hashing&) = delete;
	Hashing(std::string inputString);
	~Hashing();
	std::string hashString(std::string inputString);
};

Hashing::Hashing()
{
	m_hashedString = std::string();
}

Hashing::Hashing(std::string inputString)
{
	m_hashedString = inputString;
}

Hashing::~Hashing()
{
	//empty
}

std::string Hashing::hashString(std::string inputString)
{

	//returns a hashed string equivalent to the input string+next 3 characters of the alphabet or next 3 numbers;
	std::string hashedString;
	for (char& c : inputString) {
		if (isalpha(c)) {
			char base = (isupper(c)) ? 'A' : 'a';
			c = base + (c - base + 3) % 26;
			hashedString += c;
		}
		else if (isdigit(c)) {
			c = '0' + (c - '0' + 3) % 10;
			hashedString += c;
		}
	}
	return hashedString;

}
