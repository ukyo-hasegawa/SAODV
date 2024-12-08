#include <iostream>
#include <unordered_map>
#include <vector>

struct RoutingTableEntry {
    std::string destination;
    std::string nextHop;
    uint32_t sequenceNumber;
    uint8_t hopCount;
    bool active;

    RoutingTableEntry(std::string dest, std::string next, uint32_t seq, uint8_t hop)
        : destination(dest), nextHop(next), sequenceNumber(seq), hopCount(hop), active(true) {}
};

struct RREQ {
    std::string src;
    std::string dest;
    uint32_t seqNo;
    uint8_t hopCount;

    RREQ(std::string s, std::string d, uint32_t seq)
        : src(s), dest(d), seqNo(seq), hopCount(0) {}
};

struct RREP {
    std::string src;
    std::string dest;
    uint32_t seqNo;
    uint8_t hopCount;

    RREP(std::string s, std::string d, uint32_t seq, uint8_t hop)
        : src(s), dest(d), seqNo(seq), hopCount(hop) {}
};

void handleRREQ(RREQ& rreq, std::unordered_map<std::string, RoutingTableEntry>& routingTable) {
    if (routingTable.find(rreq.dest) != routingTable.end()) {
        // RREPを生成
        std::cout << "Route found: Sending RREP for " << rreq.dest << "\n";
    } else {
        // RREQを転送
        rreq.hopCount++;
        std::cout << "Broadcasting RREQ for " << rreq.dest << "\n";
    }
}

void handleRREP(RREP& rrep, std::unordered_map<std::string, RoutingTableEntry>& routingTable) {
    routingTable[rrep.dest] = RoutingTableEntry(rrep.dest, rrep.src, rrep.seqNo, rrep.hopCount);
    std::cout << "Route established for " << rrep.dest << "\n";
}