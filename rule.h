#ifndef RULE_H
#define RULE_H

#include <ostream>
#include <string>
#include <vector>

using std::string;
using std::ostream;
using std::vector;

#include "rule_spirit.h"
#include "trie.h"

class Rule
{
public:
    static Rule *create_rule(const string &rule);

    Action get_action();
    const sequence32 &get_route();
    const vector<sequence8> &get_content();
    const sequence8 &get_pcre() const;

private:
    Rule();
    bool parse_rule(const string &line);
    void init_from_rule_spirit(const RuleSpirit &rule);
    void print_sequence_val(sequence_val32 value, ostream &stream);

    // action that should be taken if rule is matched
    Action action;

    // message that should be inserted into database if this rule matched
    string message;

    // route info includes all information about protocol and IP adresses and ports.
    // it will be inserted in trie and matched against route info of every received packet.
    sequence32 route;
    vector<sequence8> content;
    sequence8 pcre;
    int offset;
    int length;
};

ostream &operator<<(ostream &stream, Rule &rule);

#endif // RULE_H
