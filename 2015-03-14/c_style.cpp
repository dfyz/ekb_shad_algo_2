#include <algorithm>
#include <iostream>
#include <limits>
#include <string>

static const size_t ALPHABET_SIZE = std::numeric_limits<char>::max();

struct TVertex {
	TVertex()
		: Transitions(new const TVertex*[ALPHABET_SIZE])
	{
	}

	TVertex(const TVertex&) = delete;
	TVertex& operator=(const TVertex&) = delete;

	~TVertex() {
		delete[] Transitions;
	}

	const TVertex** Transitions;
};

class TAutomaton {
public:
	TAutomaton(const TVertex& start)
		: Start(start)
	{
	}

	bool AcceptsString(const std::string& str) {
		auto current = &Start;
		for (auto ch: str) {
			auto next = current->Transitions[ch];
			if (next == nullptr) {
				return false;
			}
			current = next;
		}
		return current == &Start;
	}

private:
	const TVertex& Start;
};

int main() {
	TVertex** vertices = new TVertex*[0];
	size_t vertexCount = 0;

	size_t from;
	size_t to;
	char ch;
	while (std::cin >> from >> to >> ch) {
		size_t newVertexCount = std::max(from, to) + 1;

		// Don't try this at
		if (newVertexCount > vertexCount) {
			TVertex** newVertices = new TVertex*[newVertexCount];
			for (size_t i = 0; i < vertexCount; i++) {
				newVertices[i] = vertices[i];
			}
			for (size_t i = vertexCount; i < newVertexCount; i++) {
				newVertices[i] = new TVertex();
			}

			delete[] vertices;

			vertices = newVertices;
			vertexCount = newVertexCount;
		}

		vertices[from]->Transitions[ch] = vertices[to];
	}

	TAutomaton automaton(*vertices[0]);

	std::cout << automaton.AcceptsString("AABB") << std::endl;
	std::cout << automaton.AcceptsString("") << std::endl;
	std::cout << automaton.AcceptsString("ABABBABA") << std::endl;

	std::cout << automaton.AcceptsString("AAABB") << std::endl;
	std::cout << automaton.AcceptsString("ABBBB") << std::endl;
	std::cout << automaton.AcceptsString("ABABABA") << std::endl;
	std::cout << automaton.AcceptsString("B") << std::endl;

	for (size_t i = 0; i < vertexCount; i++) {
		delete vertices[i];
	}
	delete[] vertices;
}
