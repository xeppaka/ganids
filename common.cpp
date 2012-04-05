#include <iomanip>
#include "common.h"

using std::hex;

//std::ostream& print_sequence_val32(std::ostream &stream, const sequence_val32 &val) {
//    int cur_node_type =  val & NODE_TYPE_PATTERN;

//    switch (cur_node_type) {
//    case NODE_NORMAL:
//        stream << static_cast<int>(val & NODE_VALUE_PATTERN);
//        break;
//    case NODE_ANY:
//        stream << '*';
//        break;
//    case NODE_RANGE:
//        int from = (val & NODE_VALUE_PATTERN) >> 8;
//        int to = val & NODE_VALUE_PATTERN & 0x00FF;

//        stream << from << '-' << to;
//        break;
//    }

//    return stream;
//}

std::ostream& operator<<(std::ostream &stream, const sequence32 &sequence) {
    for (sequence32::const_iterator it = sequence.begin(); it != sequence.end(); ++it) {
        stream << hex << *it;
        if (it != sequence.end() - 1)
            stream << ' ';
    }

    return stream;
}
