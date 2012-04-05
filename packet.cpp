#include <net/ethernet.h>
#include <string>
#include <iostream>

#include "common.h"
#include "packet.h"

using std::string;
using std::cout;
using std::endl;

#define IP_HL(ip)   (((ip)->ip_hl) & 0x0f)

using std::hex;

Packet::Packet(in_addr ip_from, in_addr ip_to, int port_from, int port_to)
{
    this->ip_from = ip_from;
    this->ip_to = ip_to;
    this->port_from = port_from;
    this->port_to = port_to;
}

// we want quickly print out packet
ostream &operator<<(ostream &stream, const Packet &packet) {
    BOOST_LOG_TRIVIAL(trace) << "Begin - Nids::operator<<";
    in_addr addr = packet.get_ip_from();
    stream <<  (addr.s_addr & 0xFF) << '.';
    stream << ((addr.s_addr >> 8) & 0xFF) << '.';
    stream << ((addr.s_addr >> 16) & 0xFF) << '.';
    stream << ((addr.s_addr >> 24) & 0xFF);

    stream << ' ' << ntohs(packet.get_port_from());

    addr = packet.get_ip_to();
    stream << ' ' << (addr.s_addr & 0xFF) << '.';
    stream << ((addr.s_addr >> 8) & 0xFF) << '.';
    stream << ((addr.s_addr >> 16) & 0xFF) << '.';
    stream << ((addr.s_addr >> 24) & 0xFF);

    stream << ' ' << ntohs(packet.get_port_to()) << std::endl;

    BOOST_LOG_TRIVIAL(trace) << "End - Nids::operator<<";
    return stream;
}

Packet *Packet::create_packet(const u_char *raw_packet)
{
    BOOST_LOG_TRIVIAL(trace) << "Begin - Nids::create_packet";
    // we are taking into account only TCP/IP packets
    const ethhdr *ether = reinterpret_cast<const ethhdr *>(raw_packet);
    if (ntohs(ether->h_proto) == ETH_P_IP) {
        const ip *ip_packet = reinterpret_cast<const ip *>(raw_packet + sizeof(*ether));
        if (ip_packet->ip_p == IPPROTO_TCP) {
            const int ip_header_size = IP_HL(ip_packet) * 4;
            BOOST_LOG_TRIVIAL(info) << "ip header size: " << ip_header_size;
            const tcphdr *tcp_header = reinterpret_cast<const tcphdr *>(raw_packet + sizeof(*ether) + ip_header_size);
            Packet *p = new Packet(ip_packet->ip_src, ip_packet->ip_dst, ntohs(tcp_header->source), ntohs(tcp_header->dest));
            sequence32 &route = p->get_route();

            route.push_back(TCP);

            // push IP source address
            route.push_back(ip_packet->ip_src.s_addr & 0xFF);
            route.push_back((ip_packet->ip_src.s_addr >> 8) & 0xFF);
            route.push_back((ip_packet->ip_src.s_addr >> 16) & 0xFF);
            route.push_back(ip_packet->ip_src.s_addr >> 24);

            // push TCP source port
            route.push_back(ntohs(tcp_header->source));

            // push IP dest address
            route.push_back(ip_packet->ip_dst.s_addr & 0xFF);
            route.push_back((ip_packet->ip_dst.s_addr >> 8) & 0xFF);
            route.push_back((ip_packet->ip_dst.s_addr >> 16) & 0xFF);
            route.push_back(ip_packet->ip_dst.s_addr >> 24);

            // push TCP dest port
            route.push_back(ntohs(tcp_header->dest));

            const int tcp_header_size = tcp_header->doff;
            BOOST_LOG_TRIVIAL(info) << "tcp header size: " << tcp_header_size;
            p->payload_size = ntohs(ip_packet->ip_len) - ip_header_size - tcp_header_size;
            BOOST_LOG_TRIVIAL(info) << "payload size: " << p->payload_size;
            p->payload_offset = static_cast<int>(reinterpret_cast<const u_char *>(tcp_header) - raw_packet) + tcp_header_size;
            p->raw_packet = raw_packet;

            BOOST_LOG_TRIVIAL(trace) << "End - Nids::create_packet";
            return p;
        }
    }

    BOOST_LOG_TRIVIAL(trace) << "End - Nids::create_packet";
    return NULL;
}

Packet *Packet::create_fake_packet()
{
    BOOST_LOG_TRIVIAL(trace) << "Begin - Nids::create_fake_packet";
    // we are taking into account only TCP/IP packets
    in_addr iaddr;
    iaddr.s_addr = 0;
    Packet *p = new Packet(iaddr, iaddr, 80, 80);
    sequence32 &route = p->get_route();

    // push protocol
    route.push_back(TCP);

    // push IP source address
    route.push_back(192);
    route.push_back(168);
    route.push_back(0);
    route.push_back(101);

    // push TCP source port
    route.push_back(80);

    // push IP dest address
    route.push_back(192);
    route.push_back(168);
    route.push_back(0);
    route.push_back(101);

    // push TCP dest port
    route.push_back(80);

    const u_char *payload = (const u_char *)"1234lenta.ru12312313";

    p->raw_packet = payload;
    p->payload_size = 20;
    p->payload_offset = 0;

    BOOST_LOG_TRIVIAL(trace) << "End - Nids::create_fake_packet";
    return p;
}

in_addr Packet::get_ip_from() const
{
    return ip_from;
}

in_addr Packet::get_ip_to() const
{
    return ip_to;
}

int Packet::get_port_from() const
{
    return port_from;
}

int Packet::get_port_to() const
{
    return port_to;
}

void Packet::set_ip_from(in_addr ip)
{
    ip_from = ip;
}

void Packet::set_ip_to(in_addr ip)
{
    ip_to = ip;
}

void Packet::set_port_from(int port)
{
    port_from = port;
}

void Packet::set_port_to(int port)
{
    port_to = port;
}

sequence32 &Packet::get_route()
{
    return route;
}

const sequence32 &Packet::get_route() const
{
    return route;
}

int Packet::get_payload_offset() const
{
    return payload_offset;
}

int Packet::get_payload_size() const
{
    return payload_size;
}

const u_char *Packet::get_raw_packet() const
{
    return raw_packet;
}

vector<u_char> &Packet::save_payload()
{
    payload.assign(raw_packet + payload_offset, raw_packet + payload_offset + payload_size);
    raw_packet = NULL;
    return payload;
}

const vector<u_char> &Packet::get_payload() const
{
    return payload;
}
