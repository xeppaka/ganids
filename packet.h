#ifndef PACKET_H
#define PACKET_H

#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <ostream>
#include <vector>
#include "common.h"
#include "rule_spirit.h"

using std::ostream;
using std::vector;

class Packet
{
public:
    Packet(in_addr ip_from, in_addr ip_to, int port_from, int port_to);

    // create packet from raw data
    static Packet* create_packet(const u_char *raw_packet);

    // for test purposes
    static Packet* create_fake_packet();

    // getters and setters for packet components
    in_addr get_ip_from() const;
    in_addr get_ip_to() const;
    int get_port_from() const;
    int get_port_to() const;
    void set_ip_from(in_addr ip);
    void set_ip_to(in_addr ip);
    void set_port_from(int port);
    void set_port_to(int port);
    sequence32 &get_route();
    const sequence32 &get_route() const;
    int get_payload_offset() const;
    int get_payload_size() const;
    const u_char *get_raw_packet() const;

    // payload from raw_packet is saved into internal vector
    // it means that packet will be analyzed by CPU
    vector<u_char> &save_payload();
    const vector<u_char> &get_payload() const;

private:
    in_addr ip_from;
    in_addr ip_to;
    int port_from;
    int port_to;

    int payload_size;
    int payload_offset;

    // info about route for analyzing in the trie
    sequence32 route;
    const u_char *raw_packet;
    vector<u_char> payload;
};

ostream &operator<<(ostream &stream, const Packet &packet);

#endif // PACKET_H
