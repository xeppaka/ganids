#ifndef REGEX_H
#define REGEX_H

#include <vector>
#include <climits>

#include "re2/re2.h"
#include "re2/regexp.h"
#include "re2/prog.h"

#include "common.h"

using std::vector;

#define STATE_SIZE (UCHAR_MAX + 1)
#define STATE_DEAD -1
#define STATE_WHOLE_STR -2

class Regex
{
public:
    Regex();

    int compile(const sequence8 &regex);
    int count_states();
    int get_transition_value(int dfa_offset, int state, int c);
    const vector<int> get_transition_state(int dfa_offset, int state);
    const vector<int> get_transition_state(int row);
    bool is_match(int dfa_offset, int state);
    bool is_match(int row);
    const vector<int> get_dfa_offsets() const;
    bool search(const sequence8 &data, int dfa_offset);
    bool search(const u_char *data, int length, int dfa_offset);
    void dump();

private:
    vector< vector<int> > transition_table;
    vector<int> dfa_offsets;
    int states_total;
    int states_cur_dfa;
    map<void *, int> states_map;
    const re2::uint8 *bytemap;

    int analyze_state(void *state);
};

#endif // REGEX_H
