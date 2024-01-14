#include "Hashing.h"

void Hasher::HashPassword(const char* pass)
{
	std::string password{ pass };
	char* hashedPassword = std::_Const_cast(pass);
	char hashedChar;
	int i = 0;
	for (const char& c : password) {
		if (isalpha(c)) {
			char base = (isupper(c)) ? 'A' : 'a';
			hashedChar = base + (c - base + 3) % 26;
			hashedPassword[i++] = hashedChar;
		}
		else if (isdigit(c)) {
			hashedChar = '0' + (c - '0' + 3) % 10;
			hashedPassword[i++] = hashedChar;
		}
		else {
			hashedPassword[i++] = c;
		}
	}
}
