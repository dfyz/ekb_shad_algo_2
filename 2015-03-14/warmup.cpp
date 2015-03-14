#include <iostream>

struct T {
	T()
		: X(new int(42))
	{
		std::cout << "Created" << std::endl;
	}

	T(const T& other)
		: X(new int(*other.X))
	{
		std::cout << "Copy created" << std::endl;
	}

	T& operator=(const T& other) {
		std::cout << "=" << std::endl;
		delete X;
		X = new int(*other.X);
		return *this;
	}

	~T() {
		std::cout << "Destroyed" << std::endl;
		delete X;
	}

	int* X;
};

int main() {
	{
		T t;
		T t2 = t;
		t2 = t;
	}

	std::cout << "====" << std::endl;

	T* ptrT = new T();
	delete ptrT;

	std::cout << "====" << std::endl;

	T* multiT = new T[5];
	delete[] multiT;
}