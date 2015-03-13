#include <cstddef>

#include <iostream>
#include <limits>
#include <vector>
#include <string>

typedef ptrdiff_t TPos;

class TCountingStringWrapper : public std::string {
public:
	char Get(size_t idx) {
		++Count;
		return (*this)[idx];
	}

	void Reset() {
		Count = 0;
	}

	size_t Count = 0;
};

TPos MatchNaively(const std::string& needle, TCountingStringWrapper& haystack) {
	for (size_t i = 0; i + needle.length() <= haystack.length(); i++) {
		size_t matched = 0;
		while (matched < needle.length()) {
			if (needle[matched] != haystack.Get(i + matched)) {
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

TPos MatchWithBMH(const std::string& needle, TCountingStringWrapper& haystack) {
	std::vector<TPos> shiftsByChar(std::numeric_limits<char>::max(), needle.length());
	for (TPos i = 0; i + 1 < needle.length(); i++) {
		shiftsByChar[needle[i]] = needle.length() - 1 - i;
	}

	TPos i = needle.length() - 1;
	while (i < haystack.length()) {
		size_t matched = 0;
		while (matched < needle.length()) {
			if (needle[needle.length() - 1 - matched] != haystack.Get(i - matched)) {
				break;
			}
			matched++;
		}
		if (matched == needle.length()) {
			return i - needle.length() + 1;
		}
		i += shiftsByChar[haystack[i]];
	}
	return -1;
}

int main() {
	std::string needle;
	std::getline(std::cin, needle);
	TCountingStringWrapper haystack;
	std::getline(std::cin, haystack);

	haystack.Reset();
	std::cout << "naive matching result = " << MatchNaively(needle, haystack) << std::endl;
	std::cout << "Count = " << haystack.Count << std::endl;

	haystack.Reset();
	std::cout << "BMH result = " << MatchWithBMH(needle, haystack) << std::endl;
	std::cout << "Count = " << haystack.Count << std::endl;
}