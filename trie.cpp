#include "trie.h"
#include "queue"

using std::queue;

Trie::Trie()
{
    start = new Trie_node();
}

Trie::~Trie()
{
    clear();
    delete start;
}

void Trie::insert(const sequence32 &sequence, int value)
{
    Trie_node *cur_node = start;

    for (sequence32::const_iterator it = sequence.begin(); it != sequence.end(); ++it) {
        int node_type = (*it) & NODE_TYPE_PATTERN;
        int node_value = 0;
        Trie_node *next = 0;
        switch (node_type) {
        case NODE_NORMAL:
            node_value = (*it) & NODE_VALUE_PATTERN;
            next = cur_node->next(node_value);
            if (next == NULL) {
                Trie_node *new_next = new Trie_node();
                cur_node->set_next(node_value, new_next);
                cur_node = new_next;
            } else
                cur_node = next;
            break;
        case NODE_ANY:
            if (cur_node->get_next_any() == NULL) {
                Trie_node *new_node = new Trie_node();
                cur_node->set_next_any(new_node);
                cur_node = new_node;
            } else
                cur_node = cur_node->get_next_any();
            break;
        case NODE_RANGE:
            {
                sequence_val32 from = *(++it);
                sequence_val32 to = *(++it);
                trie_range_t &ranges = cur_node->get_ranges();
                ranges.push_back(trie_range(from, to));
                Trie_node *new_node = new Trie_node();
                trie_range_value_t &range_values = cur_node->get_range_values();
                range_values.push_back(new_node);
                cur_node = new_node;
            }
            break;
        }
    }

    cur_node->append_value(value);
}

bool Trie::search(const sequence32 &sequence, vector<int> &result)
{
    queue<Trie_node *> q_nodes;
    q_nodes.push(start);
    queue<sequence32::const_iterator> q_iterators;
    q_iterators.push(sequence.begin());

    while (!q_nodes.empty()) {
        Trie_node *cur_node = q_nodes.front();
        q_nodes.pop();

        sequence32::const_iterator cur_seq_iter = q_iterators.front();
        q_iterators.pop();

        if (cur_seq_iter == sequence.end()) {
            vector<int>& val = cur_node->get_values();
            if (!val.empty())
                result.insert(result.end(), val.begin(), val.end());

            continue;
        }

        sequence_val32 cur_value = (*cur_seq_iter) & NODE_VALUE_PATTERN;

        // look in transition table
        Trie_node *normal_next = cur_node->next(cur_value);
        if (normal_next) {
            q_nodes.push(normal_next);
            q_iterators.push(cur_seq_iter + 1);
        }

        // look in ranges
        for (trie_range_t::const_iterator it = cur_node->get_ranges().begin(); it != cur_node->get_ranges().end(); ++it) {
            if (cur_value >= (*it).from_ && cur_value <= (*it).to_) {
                q_nodes.push(cur_node->get_range_values()[it - cur_node->get_ranges().begin()]);
                q_iterators.push(cur_seq_iter + 1);
            }
        }

        // check if "any" flag is set
        if (cur_node->get_next_any()) {
            q_nodes.push(cur_node->get_next_any());
            q_iterators.push(cur_seq_iter + 1);
        }
    }
}

void Trie::clear()
{
    if (start)
        clear_(start);
}

void Trie::clear_(Trie_node *node, int depth)
{
    const trie_transition_t &trans = node->get_transitions();
    for (trie_transition_t::const_iterator it = trans.begin(); it != trans.end(); ++it)
        clear_((*it).second, depth + 1);
    if (depth > 0)
        delete node;
}
