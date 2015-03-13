#include <iostream>
#include <string>
#include <cstddef>

typedef ptrdiff_t TPos;

TPos MatchNaively(const std::string& needle, const std::string& haystack) {
	for (size_t i = 0; i + needle.length() <= haystack.length(); i++) {
		size_t matched = 0;
		while (matched < needle.length()) {
			if (needle[matched] != haystack[i + matched]) {
				break;
			}
			matched++;
		}
		if (matched == needle.length()) {
			return i;
		}
	}
	return -1;
}

int main() {
	std::string needle;
	std::getline(std::cin, needle);
	std::string haystack;
	std::getline(std::cin, haystack);

	std::cout << "naive matching result = " << MatchNaively(needle, haystack) << std::endl;
}