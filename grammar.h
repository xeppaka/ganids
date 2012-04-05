#ifndef RULE_GRAMMARS_H
#define RULE_GRAMMARS_H

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <vector>

#include "common.h"
#include "rule_spirit.h"

using std::vector;

namespace qi = boost::spirit::qi;

using qi::ascii::space_type;
using qi::lit;
using qi::uint_;
using qi::uint_parser;
using qi::char_;
using qi::no_skip;
using qi::lexeme;

BOOST_FUSION_ADAPT_STRUCT(
        RuleOption,
        (RuleOptionType, type)
        (int, value)
        (sequence8, sequence_value)
)

BOOST_FUSION_ADAPT_STRUCT(
        RuleSpirit,
        (Action, action)
        (sequence32, route)
        (rule_options, options)
)

struct DigitRange {
    DigitRange():
        from(-1),
        to(-1)
    {}
    int from;
    int to;
};

BOOST_FUSION_ADAPT_STRUCT(
        DigitRange,
        (int, from)
        (int, to)
)

struct action_ : qi::symbols<char, Action>
{
    action_()
    {
        add
                ("alert", ALERT)
                ;
    }
} action;

struct protocol_ : qi::symbols<char, TransportProtocol>
{
    protocol_()
    {
        add
                ("tcp", TCP)
                ;
    }
} protocol;

template<typename Iterator>
struct RuleRouteGrammar : qi::grammar<Iterator, RuleSpirit(), space_type>
{
    RuleRouteGrammar() : RuleRouteGrammar::base_type(rule)
    {
        using boost::phoenix::push_back;
        using boost::phoenix::insert;
        using boost::phoenix::assign;
        using boost::phoenix::begin;
        using boost::phoenix::end;
        using boost::phoenix::at_c;
        using namespace qi::labels;

        ip_digit = uint_parser<unsigned, 10, 1, 3>();
        ip_part_range = uint_parser<unsigned, 10, 1, 3>() >> '-' >> uint_parser<unsigned, 10, 1, 3>();
        ip_part = ip_digit[_val = _1] | char_('*')[_val = NODE_ANY];
        ip_address = (ip_part_range[push_back(_val, NODE_RANGE)][push_back(_val, at_c<0>(_1))][push_back(_val, at_c<1>(_1))]
                      | ip_part[push_back(_val, _1)]) >> '.' >>
                     (ip_part_range[push_back(_val, NODE_RANGE)][push_back(_val, at_c<0>(_1))][push_back(_val, at_c<1>(_1))]
                      | ip_part[push_back(_val, _1)]) >> '.' >>
                     (ip_part_range[push_back(_val, NODE_RANGE)][push_back(_val, at_c<0>(_1))][push_back(_val, at_c<1>(_1))]
                      | ip_part[push_back(_val, _1)]) >> '.' >>
                     (ip_part_range[push_back(_val, NODE_RANGE)][push_back(_val, at_c<0>(_1))][push_back(_val, at_c<1>(_1))]
                      | ip_part[push_back(_val, _1)]);

        port_range = uint_ >> '-' >> uint_;
        port = (uint_[_val = _1] | char_('*')[_val = NODE_ANY]);
        tcp_route_info = protocol[push_back(_val, _1)] >> ip_address[insert(_val, end(_val), begin(_1), end(_1))]
                                    >> (port_range[push_back(_val, NODE_RANGE)][push_back(_val, at_c<0>(_1))][push_back(_val, at_c<1>(_1))] | port[push_back(_val, _1)]) >> lit("->")
                                    >> ip_address[insert(_val, end(_val), begin(_1), end(_1))]
                                    >> (port_range[push_back(_val, NODE_RANGE)][push_back(_val, at_c<0>(_1))][push_back(_val, at_c<1>(_1))] | port[push_back(_val, _1)]);

        option_value_bin = uint_parser<unsigned, 16, 2, 2>();
        option_value_content = *((lit("\\\"")[push_back(_val, '"')] | lit("\\|")[push_back(_val, '|')] | (char_ - (lit('"') | '|'))[push_back(_val, _1)]) | (lit('|') >> *option_value_bin[push_back(_val, _1)] >> '|'));
        option_value_pcre = *(lit("\\\"")[push_back(_val, '"')] | (char_ - lit('"'))[push_back(_val, _1)]);
        option_type_content = lit("content")[_val = CONTENT];
        option_type_pcre = lit("pcre")[_val = PCRE];
        option_type_offset = lit("offset")[_val = OFFSET];
        option_type_length = lit("offset")[_val = LENGTH];
        rule_option = (option_type_pcre[at_c<0>(_val) = _1] >> ':' >> '"' >> option_value_pcre[assign(at_c<2>(_val), begin(_1), end(_1))] >> '"') |
                (option_type_content[at_c<0>(_val) = _1] >> ':' >> '"' >> option_value_content[assign(at_c<2>(_val), begin(_1), end(_1))] >> '"') |
                (option_type_offset[at_c<0>(_val) = _1] >> ':' >> '"' >> uint_[at_c<1>(_val) = _1]) |
                (option_type_length[at_c<0>(_val) = _1] >> ':' >> '"' >> uint_[at_c<1>(_val) = _1]);
        tcp_rule_option = rule_option.alias();
        tcp_options = lit('{') >> *tcp_rule_option >> '}';

        tcp_rule = action >> tcp_route_info >> tcp_options;
        rule = tcp_rule.alias();

//        ip_part.name("ip_part");
//        ip_digit.name("ip_digit");
//        ip_part_range.name("ip_part_range");
//        ip_address.name("ip_address");
//        port.name("port");
//        port_range.name("port_range");
//        tcp_route_info.name("rule");
//        option_value_bin.name("option_value_bin");
//        option_value_content.name("option_value_content");
//        option_value_pcre.name("option_value_pcre");
//        option_type_content.name("option_type_content");
//        option_type_pcre.name("option_type_pcre");
//        option_type_offset.name("option_type_offset");
//        option_type_length.name("option_type_length");
//        rule_option.name("rule_option");
//        tcp_rule_option.name("tcp_rule_option");
//        tcp_options.name("tcp_options");
//        tcp_rule.name("tcp_rule");
//        rule.name("rule");

//        debug(ip_part);
//        debug(ip_digit);
//        debug(ip_part_range);
//        debug(ip_address);
//        debug(port);
//        debug(port_range);
//        debug(tcp_route_info);
//        debug(option_value_bin);
//        debug(option_value_content);
//        debug(option_value_pcre);
//        debug(option_type_content);
//        debug(option_type_pcre);
//        debug(option_type_offset);
//        debug(option_type_length);
//        debug(rule_option);
//        debug(tcp_rule_option);
//        debug(tcp_options);
//        debug(tcp_rule);
//        debug(rule);
    }

    qi::rule<Iterator, sequence_val8()> option_value_bin;
    qi::rule<Iterator, sequence8(), space_type> option_value_content;
    qi::rule<Iterator, sequence8(), space_type> option_value_pcre;

    // option types
    qi::rule<Iterator, RuleOptionType()> option_type_content;
    qi::rule<Iterator, RuleOptionType()> option_type_pcre;
    qi::rule<Iterator, RuleOptionType()> option_type_offset;
    qi::rule<Iterator, RuleOptionType()> option_type_length;

    // general option
    qi::rule<Iterator, RuleOption(), space_type> rule_option;

    // tcp option: general option + some tcp specific options
    qi::rule<Iterator, RuleOption(), space_type> tcp_rule_option;

    // tcp options: array of tcp option
    qi::rule<Iterator, vector<RuleOption>(), space_type> tcp_options;

    qi::rule<Iterator, sequence_val32()> ip_part;
    qi::rule<Iterator, sequence_val32()> ip_digit;
    qi::rule<Iterator, DigitRange()> ip_part_range;
    qi::rule<Iterator, sequence32()> ip_address;
    qi::rule<Iterator, sequence_val32()> port;
    qi::rule<Iterator, DigitRange()> port_range;

    qi::rule<Iterator, sequence32(), space_type> tcp_route_info;

    // tcp rule: protocol tcp, has specific route info + tcp options
    qi::rule<Iterator, RuleSpirit(), space_type> tcp_rule;
    qi::rule<Iterator, RuleSpirit(), space_type> rule;
};

#endif // RULE_GRAMMARS_H
