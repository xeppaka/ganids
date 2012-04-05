#ifndef AC_H
#define AC_H

#include <string>
#include <queue>
#include <set>
#include <climits>

#include "common.h"

using namespace std;

#define FAIL -1
#define STATE_SIZE UCHAR_MAX + 1

class AC
{
public:
    AC();

    void insert(const sequence8 &sequence, int value);
    void search(const sequence8 &sequence, vector<int> &result);
    void search(const u_char *sequence, int length, vector<int> &result);
    int next(int state, sequence_val8 value);

    void finish();
    int count_states();
    int get_transition_value(int state, int c);
    const vector<int> &get_transition_row(int state);
    const vector<int> &get_out(int state);

private:
    void enter(const sequence8 &sequence, int value);
    void compute_fail_transitions();

    vector< vector<int> > transition_table;
    vector< vector<int> > out;
};

#endif // AC_H
