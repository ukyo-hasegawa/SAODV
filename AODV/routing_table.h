#include <unordered_map>
#include <stdint.h>

struct RoutingEntry {
    uint32_t next_hop;
    uint8_t hop_count;
    uint32_t dest_seq_num;
    uint64_t lifetime; // 有効期限
};

std::unordered_map<uint32_t, RoutingEntry> routing_table;