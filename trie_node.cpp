#include "trie_node.h"

Trie_node::Trie_node()
{
    any_next_ = NULL;
}

vector<int>& Trie_node::get_values()
{
    return values;
}

void Trie_node::append_values(vector<int> &values)
{
    this->values.insert(this->values.end(), values.begin(), values.end());
}

void Trie_node::append_value(int value)
{
    values.push_back(value);
}

void Trie_node::set_values(vector<int> &values)
{
    this->values = values;
}

Trie_node *Trie_node::next(sequence_val32 val)
{
    if (next_.count(val) > 0)
        return next_[val];
    else
        return NULL;
}

void Trie_node::set_next(sequence_val32 val, Trie_node *next)
{
    next_[val] = next;
}

void Trie_node::set_next_any(Trie_node *next)
{
    any_next_ = next;
}

Trie_node *Trie_node::get_next_any()
{
    return any_next_;
}

trie_transition_t &Trie_node::get_transitions()
{
    return next_;
}

trie_range_t &Trie_node::get_ranges()
{
    return range_next_;
}

trie_range_value_t &Trie_node::get_range_values()
{
    return range_next_value_;
}

