#include <initializer_list>
#include <iostream>

void PrintSpecialCode(char ch) {
	std::cout << static_cast<int>(ch) << ": [" << ch << "]" << std::endl;
}

int main() {
	for (auto ch: { '\a', '\b', '\t', '\n', '\v', '\r' }) {
		PrintSpecialCode(ch);
	}
}