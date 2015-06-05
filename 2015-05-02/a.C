#include <iostream>
#include <limits>
#include <queue>
#include <vector>

typedef unsigned short TSize;
typedef unsigned char TABC;

static_assert(sizeof(TSize) + sizeof(TABC) <= sizeof(size_t), "");
static_assert(sizeof(TSize) * 2 <= sizeof(size_t), "");

struct TAutomata {
  TAutomata(TSize n, TABC l)
    : nstates(n),
      nsymbols(l),
      transitions(n*l, std::numeric_limits<TSize>::max()),
      terminals(nstates, false) {}

  TSize& transition_at(TSize state, TABC symbol) {
    return transitions.at(symbol*nstates + state);
  }

  const TSize& transition_at(TSize state, TABC symbol) const {
    return transitions.at(symbol*nstates + state);
  }

  const TSize nstates;
  const TABC  nsymbols;
  std::vector<TSize> transitions;
  std::vector<bool>  terminals;
};

TAutomata read_automata(std::istream& in) {
  TSize n, k;
  size_t l;
  in >> n >> k >> l;
  TAutomata rv(n, l);

  for (TSize i = 0; i != k; ++i) {
    TSize terminal;
    in >> terminal;
    rv.terminals[terminal] = true;
  }

  for (size_t i = 0; i != rv.transitions.size(); ++i) {
    TSize state, target;
    char raw_symbol;
    in >> state >> raw_symbol >> target;
    rv.transition_at(state, raw_symbol - 'a') = target;
  }

  return rv;
}

bool are_equivalent(const TAutomata& lhs, const TAutomata& rhs) {
  if (lhs.nsymbols != rhs.nsymbols)
    return false;

  std::vector<bool> seen(lhs.nstates * rhs.nstates, false);
  typedef std::pair<TSize, TSize> TStatePair;
  auto was_seen = [&seen, &lhs](const TStatePair& states) {
    return seen.at(states.first + lhs.nstates * states.second);
  };

  std::queue<TStatePair> states_queue;
  states_queue.push(std::make_pair(0, 0));

  while (!states_queue.empty()) {
    TStatePair states = states_queue.front(); states_queue.pop();
    if (was_seen(states)) {
      continue;
    }
    was_seen(states) = true;

    if (lhs.terminals[states.first] != rhs.terminals[states.second])
      return false;

    for (TABC symbol = 0; symbol != lhs.nsymbols; ++symbol) {
      states_queue.push(std::make_pair(
          lhs.transition_at(states.first, symbol),
          rhs.transition_at(states.second, symbol)));
    }
  }

  return true;
}

int main(void) {
  const TAutomata lhs = read_automata(std::cin);
  const TAutomata rhs = read_automata(std::cin);

  std::cout << (are_equivalent(rhs, lhs) ? "" : "NOT ") << "EQUIVALENT"
            << std::endl;
}
