#include <common.h>
#include <iostream>
#include "rule.h"
#include "grammar.h"

using std::cout;
using std::endl;

typedef std::string::const_iterator iterator_t;
typedef RuleRouteGrammar<iterator_t> RuleRouteGrammar_t;

Rule::Rule()
{
}

bool Rule::parse_rule(const string &line)
{
    string::const_iterator begin = line.begin();
    string::const_iterator end = line.end();

    RuleSpirit rule_spirit;
    static RuleRouteGrammar_t route_grammar;
    if (!phrase_parse(begin, end, route_grammar, qi::ascii::space, rule_spirit))
        return false;

    init_from_rule_spirit(rule_spirit);

    return true;
}

void Rule::init_from_rule_spirit(const RuleSpirit &rule)
{
    action = rule.action;
    route = rule.route;

    for (vector<RuleOption>::const_iterator it = rule.options.begin(); it != rule.options.end(); ++it) {
        switch ((*it).type) {
        case CONTENT:
            content.push_back((*it).sequence_value);
            break;
        case PCRE:
            // rule can contain only 1 pcre
            if (pcre.size() <= 0)
                    pcre = (*it).sequence_value;
            break;
        case MESSAGE:
            message.assign((*it).sequence_value.begin(), (*it).sequence_value.end());
            break;
        case OFFSET:
            offset = (*it).value;
            break;
        case LENGTH:
            length = (*it).value;
            break;
        }
    }
}

Rule *Rule::create_rule(const string &rule)
{
    Rule *new_rule = new Rule();
    if (new_rule->parse_rule(rule))
        return new_rule;
    else
        delete new_rule;

    return NULL;
}

const sequence32 &Rule::get_route()
{
    return route;
}

const vector<sequence8> &Rule::get_content()
{
    return content;
}

const sequence8 &Rule::get_pcre() const
{
    return pcre;
}

ostream &operator<<(ostream &stream, Rule &rule) {
    const sequence32 &route = rule.get_route();
//    for (int i = 0; i < 4; ++i) {
//        print_sequence_val32(stream, route[i]);
//        if (i < 3)
//            stream << '.';
//    }

//    stream << ' ';
//    print_sequence_val32(stream, route[4]);
//    stream << " -> ";

//    for (int i = 5; i < 9; ++i) {
//        print_sequence_val32(stream, route[i]);
//        if (i < 8)
//            stream << '.';
//    }

//    stream << ' ';
//    print_sequence_val32(stream, route[9]);
    cout << route << endl;

    return stream;
}
