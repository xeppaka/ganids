#include <iostream>
#include <iomanip>

#include "re2/dfa.cc"
#include "regex.h"

using std::cout;
using std::endl;
using std::setw;

#define MAX_MEM_REGEX 8388608

using re2::DFA;

Regex::Regex():
    states_total(0)
{ }

int Regex::compile(const sequence8 &regex)
{
    if (regex.size() <= 0)
        return -1;

    string regex_str;
    regex_str.assign(regex.begin(), regex.end());

    states_map.clear();
    RE2 re(regex_str.c_str());

    re2::Regexp *rg;
    re2::Prog *prog;
    re2::DFA *dfa;

    try {
        rg = re.Regexp();
        if (rg == NULL)
            throw 1;

        prog = rg->CompileToProg(MAX_MEM_REGEX);
        if (prog == NULL)
            throw 1;

        dfa = prog->GetDFA(re2::Prog::kFirstMatch);
        if (dfa == NULL)
            throw 1;
    } catch(int i) {
        return -1;
    }

    dfa->BuildAllStates();
    // Analyze DFA. Start state is determined on this step.
    re2::DFA::SearchParams sp("standardtext12345", "standardtext12345", NULL);
    dfa->AnalyzeSearch(&sp);

    bytemap = prog->bytemap();

    DFA::State *s = sp.start;

    states_cur_dfa = 0;
    analyze_state(s);
    dfa_offsets.push_back(states_total);
    states_total += states_cur_dfa;

    return states_total - states_cur_dfa;
}

int Regex::count_states()
{
    return transition_table.size();
}

int Regex::get_transition_value(int dfa_offset, int state, int c)
{
    return transition_table[dfa_offset + state][c];
}

const vector<int> Regex::get_transition_state(int dfa_offset, int state)
{
    return transition_table[dfa_offset + state];
}

const vector<int> Regex::get_transition_state(int row)
{
    return transition_table[row];
}

bool Regex::is_match(int dfa_offset, int state)
{
    return transition_table[dfa_offset + state][STATE_SIZE] > 0;
}

bool Regex::is_match(int row)
{
    return transition_table[row][STATE_SIZE] > 0;
}

const vector<int> Regex::get_dfa_offsets() const
{
    return dfa_offsets;
}

bool Regex::search(const sequence8 &data, int dfa_offset)
{
    int cur_state = 0;

    for (sequence8::const_iterator it = data.begin(); it != data.end(); ++it) {
        if (is_match(dfa_offset, cur_state))
            return true;

        if (cur_state == STATE_DEAD)
            return false;

        if (cur_state == STATE_WHOLE_STR)
            return true;

        cur_state = transition_table[dfa_offset + cur_state][*it];
    }

    return false;
}

bool Regex::search(const u_char *data, int length, int dfa_offset)
{
    int cur_state = 0;

    for (const u_char *it = data; it < data + length; ++it) {
        if (cur_state == STATE_DEAD)
            return false;

        if (cur_state == STATE_WHOLE_STR)
            return true;

        if (is_match(dfa_offset, cur_state))
            return true;

        cur_state = transition_table[dfa_offset + cur_state][*it];
    }

    return false;
}

void Regex::dump()
{
    for (int i = 0; i < transition_table.size(); ++i) {
        for (int j = 0; j < transition_table[i].size(); ++j) {
            cout << ' ' << setw(2) << transition_table[i][j];
        }
        cout << endl;
    }
}

int Regex::analyze_state(void *state)
{
    DFA::State *state_p = reinterpret_cast<DFA::State *>(state);
    int state_mapped;
    if (states_map.count(state) > 0) {
        state_mapped = states_map[state];
    } else {
        states_map[state] = state_mapped = states_cur_dfa;
        ++states_cur_dfa;

        transition_table.resize(transition_table.size() + 1);
        transition_table.back().resize(STATE_SIZE + 1);

        if (state_p->IsMatch())
            transition_table[state_mapped + states_total][STATE_SIZE] = 1;
        for (int c = 0; c < STATE_SIZE; ++c) {
            DFA::State *next_state = state_p->next_[bytemap[c]];

            if (next_state == reinterpret_cast<DFA::State *>(0) || next_state == reinterpret_cast<DFA::State *>(1)) {
                transition_table[state_mapped + states_total][c] = STATE_DEAD;
                continue;
            }
            if (next_state == reinterpret_cast<DFA::State *>(2)) {
                transition_table[state_mapped + states_total][c] = STATE_WHOLE_STR;
                continue;
            }

            if (next_state != state) {
                int k = analyze_state(next_state);
                transition_table[state_mapped + states_total][c] = k;
            }
            else
                transition_table[state_mapped + states_total][c] = state_mapped;
        }
    }

    return state_mapped;
}
