#ifndef COMMON_H
#define COMMON_H

#include <boost/log/core.hpp>
#include <boost/log/utility/init/to_file.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/filters.hpp>
#include <ostream>
#include <vector>

namespace logging = boost::log;
namespace flt = boost::log::filters;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;

typedef unsigned char sequence_val8;
typedef unsigned short sequence_val16;
typedef unsigned int sequence_val32;
typedef std::vector<sequence_val32> sequence32;
typedef std::vector<sequence_val8> sequence8;

std::ostream& print_sequence_val32(std::ostream &stream, const sequence_val32 &val);
std::ostream& operator<<(std::ostream &stream, const sequence32 &sequence);

#define NODE_NORMAL 0x00000000
#define NODE_ANY 0x00010000
#define NODE_RANGE 0x00020000

#define NODE_TYPE_PATTERN 0x00FF0000
#define NODE_VALUE_PATTERN 0x0000FFFF

#endif // COMMON_H
