export module hashing;
import <string>;

export class __declspec(dllexport) Hashing
{
private:
	std::string m_hashedString;
public:
	Hashing();
	Hashing(const Hashing&) = delete;
	Hashing(std::string inputString);
	~Hashing();
	std::string hashString(const std::string& inputString);
};

Hashing::Hashing()
{
	//constructor for hashing class
}

Hashing::Hashing(std::string inputString)
{
	m_hashedString = inputString;
}

Hashing::~Hashing()
{
	//empty
}

std::string Hashing::hashString(const std::string& inputString)
{
	//returns a hashed string equivalent to the input string+next 3 characters of the alphabet or next 3 numbers
	m_hashedString = inputString;
	for (char& c : m_hashedString) {
		if (isalpha(c))
		{
			char base = (isupper(c)) ? 'A' : 'a';
			c = base + (c - base + 3) % 26;
		}
		else if (isdigit(c))
		{
			c = '0' + (c - '0' + 3) % 10;
		}
	}
	return m_hashedString;
}
