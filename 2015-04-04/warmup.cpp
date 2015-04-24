#include <iostream>
#include <unordered_map>
#include <memory>

std::unordered_map<void*, size_t> PtrToObjectId;
size_t NextObjectId;

struct TBigData {
	int X = 42;

	static void* operator new(size_t size) {
		auto result = ::operator new(size);
		auto newId = NextObjectId++;
		PtrToObjectId[result] = newId;
		std::cout << "Allocated memory for object #" << newId << std::endl;
		return result;
	}

	static void operator delete(void* ptr) {
		std::cout << "Deallocating memory for object #" << PtrToObjectId[ptr] << std::endl;
		::operator delete(ptr);
	}
};

struct T {
	T()
		: X(new TBigData())
	{
		std::cout << "\tConstructor" << std::endl;
	}

	T(const T& other)
		: X(new TBigData(*other.X))
	{
		std::cout << "\tCopy constructor" << std::endl;
	}

	/* T(T&& other)
		: X(other.X)
	{
		std::cout << "\tMove constructor" << std::endl;
		other.X = nullptr;
	} */

	T& operator=(const T& other) {
		delete X;
		X = new TBigData(*other.X);
		std::cout << "\tAssignment operator" << std::endl;
		return *this;
	}

	~T() {
		delete X;
		std::cout << "\tDestructor" << std::endl;
	}

	TBigData* X;
};

std::unique_ptr<T> CreateT() {
	return std::unique_ptr<T>(new T());
}

/* void UseT(std::shared_ptr<TBigData> t) {
	std::cout << "Used T: " << t.get() << std::endl;
} */

int main() {
	T* t = CreateT().get();
	std::cout << t->X << std::endl;

	/*T t;
	T t2 = t;
	t2 = t;

	T t3(CreateT());
	UseT(std::move(t3));

	T t4(t3);
	std::cout << t3.X << " " << t4.X << std::endl;

	T t5(std::move(t3));
	std::cout << t3.X << " " << t5.X << std::endl;

	{
		// C++11
		std::unique_ptr<TBigData> smartT(new TBigData());
		std::cout << smartT.get() << std::endl;
		UseT(std::move(smartT));
		std::cout << smartT.get() << std::endl;

		smartT = CreateT();
	}

	{
		std::shared_ptr<TBigData> smartT(new TBigData());
		std::cout << smartT.get() << std::endl;
		UseT(smartT);
		std::cout << smartT.get() << std::endl;

		smartT = CreateT();
	}*/

	std::cout << "===" << std::endl;
	std::cout << "ALLOCATION COUNT: " << NextObjectId << std::endl;
}