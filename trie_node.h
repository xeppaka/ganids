#ifndef TRIE_NODE_H
#define TRIE_NODE_H

#include <map>
#include <vector>
#include "common.h"

using std::map;
using std::vector;

class Trie_node;

struct trie_range {
    trie_range(sequence_val32 from, sequence_val32 to):
        from_(from),
        to_(to)
    {}
    int from_;
    int to_;
};

typedef vector<trie_range> trie_range_t;
typedef vector<Trie_node *> trie_range_value_t;
typedef map<sequence_val32, Trie_node *> trie_transition_t;

class Trie_node
{
public:
    Trie_node();
    vector<int>& get_values();
    void set_values(vector<int> &values);
    void append_values(vector<int> &values);
    void append_value(int value);
    Trie_node *next(sequence_val32 val);
    void set_next(sequence_val32 val, Trie_node *next);
    void set_next_any(Trie_node *next);
    Trie_node *get_next_any();

    trie_transition_t &get_transitions();
    trie_range_t &get_ranges();
    trie_range_value_t &get_range_values();

private:
    trie_transition_t next_;
    trie_range_t range_next_;
    trie_range_value_t range_next_value_;
    Trie_node *any_next_;
    vector<int> values;
};

#endif // TRIE_NODE_H
