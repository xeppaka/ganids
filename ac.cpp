#include <algorithm>

#include "ac.h"

AC::AC()
{
    transition_table.resize(1);
    transition_table.back().resize(STATE_SIZE, FAIL);
    out.resize(1);
}

void AC::insert(const sequence8 &sequence, int value)
{
    cout << UCHAR_MAX << endl;
    enter(sequence, value);
}

void AC::search(const sequence8 &sequence, vector<int> &result)
{
    BOOST_LOG_TRIVIAL(trace) << "Begin - AC::search";

    int s = 0;
    for (sequence8::const_iterator it = sequence.begin(); it != sequence.end(); ++it) {
        s = transition_table[s][*it];
        if (!out[s].empty())
            result.insert(result.end(), out[s].begin(), out[s].end());
    }

    BOOST_LOG_TRIVIAL(info) << "found " << out.size() << " mathes";
    BOOST_LOG_TRIVIAL(trace) << "End - AC::search";
}

void AC::search(const u_char *sequence, int length, vector<int> &result)
{
    BOOST_LOG_TRIVIAL(trace) << "Begin - AC::search";

    int s = 0;
    for (const u_char *p = sequence; p < sequence + length; ++p) {
        s = transition_table[s][*p];
        if (!out[s].empty())
            result.insert(result.end(), out[s].begin(), out[s].end());
    }

    BOOST_LOG_TRIVIAL(info) << "found " << out.size() << " mathes";
    BOOST_LOG_TRIVIAL(trace) << "End - AC::search";

}

int AC::next(int state, sequence_val8 value)
{
    return transition_table[state][value];
}

void AC::finish()
{
    BOOST_LOG_TRIVIAL(trace) << "Begin - AC::finish";

    for (int c = 0; c < STATE_SIZE; ++c)
        if (transition_table[0][c] == FAIL)
            transition_table[0][c] = 0;
    compute_fail_transitions();

    BOOST_LOG_TRIVIAL(trace) << "End - AC::finish";
}

int AC::count_states()
{
    return transition_table.size();
}

int AC::get_transition_value(int state, int c)
{
    return transition_table[state][c];
}

const vector<int> &AC::get_transition_row(int state)
{
    return transition_table[state];
}

const vector<int> &AC::get_out(int state)
{
    return out[state];
}

void AC::enter(const sequence8 &sequence, int value)
{
    BOOST_LOG_TRIVIAL(trace) << "Begin - AC::enter";

    int state = 0;
    int j = 0;
    while (next(state, sequence[j]) != FAIL && j < sequence.size()) {
        state = transition_table[state][sequence[j]];
        ++j;
    }

    int newstate;
    for (int i = j; i < sequence.size(); ++i) {
        newstate = transition_table.size();

        transition_table.resize(transition_table.size() + 1);
        out.resize(out.size() + 1);

        transition_table.back().resize(STATE_SIZE, FAIL);
        transition_table[state][sequence[i]] = newstate;
        state = newstate;
    }

    out[state].push_back(value);

    BOOST_LOG_TRIVIAL(trace) << "End - AC::enter";
}

void AC::compute_fail_transitions()
{
    BOOST_LOG_TRIVIAL(trace) << "Begin - AC::compute_fail_transitions";

    vector<int> fail_function(transition_table.size(), 0);
    queue<int> q;

    for (int c = 0; c < STATE_SIZE; ++c) {
        int transition_to = transition_table[0][c];
        if (transition_to != 0)
            q.push(transition_to);
        fail_function[transition_to] = 0;
    }

    while (!q.empty()) {
        int cur_state = q.front();
        q.pop();

        for (int c = 0; c < STATE_SIZE; ++c) {
            int transition_to = transition_table[cur_state][c];
            if (transition_to == FAIL)
                continue;

            q.push(transition_to);
            int state = fail_function[cur_state];
            while (transition_table[state][c] == FAIL)
                state = fail_function[state];
            fail_function[transition_to] = transition_table[state][c];
            out[transition_to].insert(out[transition_to].end(), out[fail_function[transition_to]].begin(), out[fail_function[transition_to]].end());
        }
    }

    for (int i = 0; i < transition_table.size(); ++i)
        for (int j = 0; j < STATE_SIZE; ++j)
            if (transition_table[i][j] == FAIL)
                transition_table[i][j] = fail_function[i];

    BOOST_LOG_TRIVIAL(trace) << "End - AC::compute_fail_transitions";
}
