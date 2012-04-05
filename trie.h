#ifndef TRIE_H
#define TRIE_H

#include <vector>

#include "common.h"
#include "trie_node.h"

using std::vector;

class Trie
{
public:
    Trie();
    ~Trie();

    void insert(const sequence32& sequence, int value);
    bool search(const sequence32& sequence, vector<int> &result);
    void clear();

private:
    Trie_node *start;
    void clear_(Trie_node *node, int depth = 0);
};

#endif // TRIE_H
