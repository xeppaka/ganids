#ifndef RULE_SPIRIT_H
#define RULE_SPIRIT_H

#include "common.h"
#include "vector"

using std::vector;

enum Action {
    ALERT = 0
};

enum TransportProtocol {
    TCP = 0
};

enum RuleOptionType {
    CONTENT = 0,
    PCRE,
    OFFSET,
    LENGTH
};

struct RuleOption {
    RuleOptionType type;
    int value;
    sequence8 sequence_value;
};

typedef vector<RuleOption> rule_options;

struct RuleSpirit {
    Action action;
    sequence32 route;
    rule_options options;
};

#endif // RULE_SPIRIT_H
