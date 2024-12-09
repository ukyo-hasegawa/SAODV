#include <vector>
#include <string>
#include <stdint.h>


struct RREQ {
    uint8_t type = 1; // RREQタイプ
    uint8_t flags = 0;
    uint8_t hop_count = 0;
    uint32_t rreq_id;
    uint32_t dest_ip;
    uint32_t dest_seq_num;
    uint32_t origin_ip;
    uint32_t origin_seq_num;
};

struct RREP {
    uint8_t type = 2; // RREPタイプ
    uint8_t flags = 0;
    uint8_t hop_count = 0;
    uint32_t dest_ip;
    uint32_t dest_seq_num;
    uint32_t origin_ip;
    uint32_t lifetime;
};

struct RERR {
    uint8_t type = 3; // RERRタイプ
    uint8_t flags = 0;
    uint8_t dest_count;
    std::vector<uint32_t> unreachable_dest_ips;
    std::vector<uint32_t> unreachable_dest_seq_nums;
};