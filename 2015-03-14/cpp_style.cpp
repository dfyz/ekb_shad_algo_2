#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

static const size_t ALPHABET_SIZE = std::numeric_limits<char>::max();
static const size_t NO_TRANSITION = std::numeric_limits<size_t>::max();

struct TVertex {
	TVertex()
		: Transitions(ALPHABET_SIZE, NO_TRANSITION)
	{
	}

	std::vector<size_t> Transitions;
};

class TAutomaton {
public:
	TAutomaton(const std::vector<TVertex>& vertices)
		: Vertices(vertices)
		, Start(0)
	{
	}

	bool AcceptsString(const std::string& str) {
		size_t current = Start;
		for (char ch: str) {
			size_t next = Vertices[current].Transitions[ch];
			if (next == NO_TRANSITION) {
				return false;
			}
			current = next;
		}
		return current == Start;
	}

private:
	const std::vector<TVertex>& Vertices;
	const size_t Start;
};

int main() {
	std::vector<TVertex> vertices;

	size_t from;
	size_t to;
	char ch;
	while (std::cin >> from >> to >> ch) {
		size_t newVertexCount = std::max(from, to) + 1;

		if (newVertexCount > vertices.size()) {
			vertices.resize(newVertexCount);
		}

		vertices[from].Transitions[ch] = to;
	}

	TAutomaton automaton(vertices);

	std::cout << automaton.AcceptsString("AABB") << std::endl;
	std::cout << automaton.AcceptsString("") << std::endl;
	std::cout << automaton.AcceptsString("ABABBABA") << std::endl;

	std::cout << automaton.AcceptsString("AAABB") << std::endl;
	std::cout << automaton.AcceptsString("ABBBB") << std::endl;
	std::cout << automaton.AcceptsString("ABABABA") << std::endl;
	std::cout << automaton.AcceptsString("B") << std::endl;
}
