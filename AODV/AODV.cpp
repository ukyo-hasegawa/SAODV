#include <iostream>
#include <unordered_map>
#include <vector>
#include "data_struct.h"
#include <random>

struct RoutingTableEntry {
    std::string destination;
    std::string nextHop;
    uint32_t sequenceNumber;
    uint8_t hopCount;
    bool active;

    RoutingTableEntry(std::string dest, std::string next, uint32_t seq, uint8_t hop)
        : destination(dest), nextHop(next), sequenceNumber(seq), hopCount(hop), active(true) {}
};

RREQ createRREQ(uint32_t dest_ip, uint32_t origin_ip, uint32_t origin_seq_num) {
    RREQ rreq;
    rreq.type = 1; // RREQ type
    rreq.flags = 0; // Flags
    rreq.hop_count = 0;
    rreq.rreq_id = generateRREQID(); // Implement a function to generate unique RREQ IDs
    rreq.dest_ip = dest_ip;
    rreq.dest_seq_num = 0; // If no known sequence, set as 0
    rreq.origin_ip = origin_ip;
    rreq.origin_seq_num = origin_seq_num;
    return rreq;
}

uint32_t generateRREQID() {
    static std::mt19937 generator(std::random_device{}());
    static std::uniform_int_distribution<uint32_t> distribution(0, UINT32_MAX);
    return distribution(generator);
}

void handleRREQ(RREQ& rreq, std::unordered_map<std::string, RoutingTableEntry>& routingTable) {
    if (routingTable.find(rreq.dest) != routingTable.end()) {
        // RREPを生成
        std::cout << "Route found: Sending RREP for " << rreq.dest << "\n";
    } else {
        // RREQを転送
        rreq.hop_count++;
        std::cout << "Broadcasting RREQ for " << rreq.dest << "\n";
    }
}

void handleRREP(RREP& rrep, std::unordered_map<std::string, RoutingTableEntry>& routingTable) {
    routingTable[rrep.dest] = RoutingTableEntry(rrep.dest, rrep.src, rrep.seqNo, rrep.hopCount);
    std::cout << "Route established for " << rrep.dest << "\n";
}