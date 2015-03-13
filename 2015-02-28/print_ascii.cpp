#include <iostream>

int main() {
	using TCharType = unsigned char;

	static const TCharType minChar = 0;
	static const TCharType maxChar = 255;

	bool firstChar = true;
	for (int i = minChar; i <= maxChar; ++i) {
		auto ch = static_cast<TCharType>(i);
		if (i % 32 == 0) {
			if (!firstChar) {
				std::cout << std::endl;
			}
			std::cout << "[" << i << "] ";
			firstChar = false;
		}
		std::cout << ch;
	}
}