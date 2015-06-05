#include <unordered_set>
#include <vector>
#include <fstream>
#include <memory>
#include <limits>
#include <iostream>

using std::vector;
using std::unordered_set;

typedef size_t state_id_t;
typedef unordered_set<state_id_t> state_set_t;
typedef vector<vector<state_id_t>> transitions_t;
typedef vector<vector<bool>> visited_matrix_t;

static const size_t NO_TRANSITION = std::numeric_limits<size_t>::max();

class TAutomaton {
    public:
        static TAutomaton read(std::istream &in);

        bool is_terminal(state_id_t state) const;
        size_t alphabet_size() const;
        size_t nstates() const;
        state_id_t transit(state_id_t state, char symbol) const;

        TAutomaton(TAutomaton&& tmp);

        TAutomaton& operator=(TAutomaton&& tmp) {
            this->_nstates = tmp._nstates;
            this->_alphabet_size = tmp._alphabet_size;
            this->_terminals = std::move(tmp._terminals);
            this->_transitions = std::move(tmp._transitions);

            return *this;
        }

        ~TAutomaton();

    private:
        size_t _nstates;
        size_t _alphabet_size;
        state_set_t _terminals;
        transitions_t _transitions;

        TAutomaton(
                size_t nstates,
                size_t alphabet_size,
                state_set_t terminals,
                transitions_t transitions);

        TAutomaton(const TAutomaton& other) = delete;
        TAutomaton& operator=(const TAutomaton& other) = delete;
};

class TAutomatonUtility {
    public:
        static bool are_equivalent(
                const TAutomaton& automaton_a, const TAutomaton& automaton_b);
};

int main() {
    auto automaton_a = TAutomaton::read(std::cin);
    auto automaton_b = TAutomaton::read(std::cin);

    if (TAutomatonUtility::are_equivalent(automaton_a, automaton_b)) {
        std::cout << "EQUIVALENT" << std::endl;
    } else {
        std::cout << "NOT EQUIVALENT" << std::endl;
    }

    exit(EXIT_SUCCESS);
}

TAutomaton TAutomaton::read(std::istream &in) {
    size_t nstates, nterminals, alphabet_size;
    in >> nstates >> nterminals >> alphabet_size;
    state_set_t terminals(nterminals);
    state_id_t tmp;

    for (size_t i = 0; i < nterminals; ++i) {
        in >> tmp;
        terminals.insert(tmp);
    }

    transitions_t transitions(
            nstates, vector<state_id_t>(alphabet_size, NO_TRANSITION));

    for (auto vec: transitions) {
        vec.resize(alphabet_size);
        for (size_t i = 0; i < vec.size(); ++i) {
            vec[i] = NO_TRANSITION;
        }
    }

    for (size_t i = 0; i < nstates; ++i) {
        for (size_t j = 0; j < alphabet_size; ++j) {
            state_id_t from, to;
            char symbol;
            in >> from >> symbol >> to;
            transitions[from][symbol - 'a'] = to;
        }
    }

    return TAutomaton(
            nstates,
            alphabet_size,
            std::move(terminals),
            std::move(transitions));
}

TAutomaton::TAutomaton(
        size_t nstates,
        size_t alphabet_size,
        state_set_t terminals,
        transitions_t transitions) :
    _nstates(nstates),
    _alphabet_size(alphabet_size),
    _terminals(terminals),
    _transitions(transitions) {
    }

TAutomaton::TAutomaton(TAutomaton&& tmp)
    : _nstates(tmp._nstates),
    _alphabet_size(tmp._alphabet_size),
    _terminals(tmp._terminals),
    _transitions(tmp._transitions) {
}

TAutomaton::~TAutomaton() {
}

bool TAutomaton::is_terminal(state_id_t state) const {
    return _terminals.find(state) != _terminals.end();
}

size_t TAutomaton::alphabet_size() const {
    return _alphabet_size;
}

size_t TAutomaton::nstates() const {
    return _nstates;
}

state_id_t TAutomaton::transit(state_id_t state, char symbol) const {
    return _transitions[state][symbol];
}

static bool are_states_equivalent(
        const TAutomaton& automaton_a,
        const TAutomaton& automaton_b,
        state_id_t start_a, state_id_t state_b,
        visited_matrix_t &be);

bool TAutomatonUtility::are_equivalent(
        const TAutomaton& automaton_a, const TAutomaton& automaton_b) {
    state_id_t begin_state_a = 0;
    state_id_t begin_state_b = 0;
    size_t nstates_first = automaton_a.nstates();
    size_t nstates_second = automaton_b.nstates();

    vector<vector<bool>> visited_matrix(nstates_first);

    for (size_t i = 0; i < nstates_first; ++i) {
        visited_matrix[i].resize(nstates_second);
        for (size_t j = 0; j < nstates_second; ++j) {
            visited_matrix[i][j] = false;
        }
    }
    return are_states_equivalent(
            automaton_a, automaton_b, begin_state_a, begin_state_b,
            visited_matrix);
}

static bool are_states_equivalent(
        const TAutomaton& automaton_a,
        const TAutomaton& automaton_b,
        state_id_t state_a, state_id_t state_b,
        visited_matrix_t &visited_matrix) {
    if (visited_matrix[state_a][state_b]) {
        return true;
    }

    if (automaton_a.is_terminal(state_a) !=
            automaton_b.is_terminal(state_b)) {
        return false;
    }

    visited_matrix[state_a][state_b] = true;

    size_t alphabet_size = automaton_a.alphabet_size();

    for (size_t i = 0; i < alphabet_size; ++i) {
        state_id_t state_a_im = automaton_a.transit(state_a, i);
        state_id_t state_b_im = automaton_b.transit(state_b, i);

        if (!are_states_equivalent(
                automaton_a, automaton_b, state_a_im, state_b_im,
                visited_matrix)) {
            return false;
        }
    }

    return true;
}

