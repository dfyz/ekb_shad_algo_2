#include <initializer_list>
#include <bitset>
#include <iostream>

int main() {
	for (auto x: {
		u8"helloworld",
		u8"приветмир",
		u8"ऄअआइई",
		u8"😄🚫🚗",
	}) {
		for (size_t i = 0; x[i]; i++) {
			std::cout << std::bitset<8>(x[i]) << " ";
		}
		std::cout << std::endl;
	}
}